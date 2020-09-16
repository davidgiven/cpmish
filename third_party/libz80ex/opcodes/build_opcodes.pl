#!/usr/bin/perl -w

use strict;

sub next_t
{
	my $arref = shift;
	my $val = splice @$arref,0,1;
	die "No more timings available!" if(!defined($val));
	return($val);
}

sub get_flag_co
{
	my $fl = shift;
	my $out="";
	my $is_n=0;
	
	if($fl eq 'P') {$fl = 'NS';}
	elsif($fl eq 'M') {$fl = 'S';}
	elsif($fl eq 'PE') {$fl = 'P';}	
	elsif($fl eq 'PO') {$fl = 'NP';}	
	
	$is_n=1 if(substr($fl,0,1) eq 'N');
	
	
	$out.="	if(";
	$out.="!(" if($is_n);
	$out.="F & FLAG_";
	if($is_n) {$out.=substr($fl,1,1);}
	else {$out.=$fl;}
	$out.=")" if($is_n);
	$out.=")";
	
	return($out);
}

sub is_arg_16bit
{
	my $arg = shift;
	my $res=0;

	$res=1 if((length($arg) == 2) and ($arg !~ /^\w+[Hh]/) and ($arg ne '#'));

#print "checking $arg gives $res\n";	
	return($res);
}

# convert one command to C code
sub convert_to_c
{
	my $readbyte="READ_MEM(";
	my $writebyte="WRITE_MEM(";
	
	my $opc = shift;
	my $def = shift;
	my $fnname = shift;
	my $out = '';
	my $head = '';
	my $foot = '';
	my $arg_str = '';
	my @arguments =();
	
	my @ww=();
	my @rr=();
	
	######
	# for commands that writes to memory or port
	my $cmd_mwrite = 0; #cmd writes to memory thru (regpair[+d]) or (adress) 
	my $cmd_pwrite = 0; #cmd writes to port thru (port)
	my $cmd_memmod = 0; #cmd modifyes data in memory, ie read-modify-write
	my $cmd_is_16bit = 0;
	my $cmd_cyclic = 0;
	my $cmd_branch = 0;
	my $cmd_memptr = 0;
	######
	
	my $addr_mptr=0;
	
	######

	if(!$$opc{'asm'}) {return("");}

	if($def eq 'base')
	{
		$$fnname = 'op_'.$$opc{'opcode'};
	}
	else
	{
		$$fnname = 'op_'.uc($def).'_'.$$opc{'opcode'};
	}
	#$$fnname = 'opc_'.$$opc{'asm'}.'_'.$def;
	#$$fnname =~ s/\s/_/g;
	#$$fnname =~ s/\(/_bR_/g;
	#$$fnname =~ s/\)/_Rb_/g;
	#$$fnname =~ s/\+/_plus_/g;
	#$$fnname =~ s/\,/_cm_/g;

	$head.="/*$$opc{asm}*/\n" if($$opc{asm});
	$$opc{'asm'} =~ s/\'/_/g;
	$head.="static void $$fnname(Z80EX_CONTEXT *cpu)\n{\n";
	
	if($$opc{'asm'} =~ /^(LD\sA,R|LD\sR,A|LD\sA,I)$/)
	{
		$$opc{'asm'} =~ s/\s/_/g;
		$$opc{'asm'} =~ s/,/_/g;
	}
	
	if($$opc{rd})
	{
		@rr=split /,/,$$opc{rd};
		
		if($def ne 'base')
		{
			#for prefixed ops: min. time used to fetch the prefix (4) must be substracted		
			for(my $i = 0;$i <= $#rr;++$i)
			{
				$rr[$i]-=4;
			}
		}
	}
	
	if($$opc{wr})
	{
		@ww=split /,/,$$opc{wr};
		
		if($def ne 'base')
		{
			#for prefixed ops: min. time used to fetch the prefix (4) must be substracted		
			for(my $i = 0;$i <= $#ww;++$i)
			{
				$ww[$i]-=4;
			}
		}
	}

	my @pair = split /\s/,$$opc{'asm'};
	#$pair[0] == command name now...

	$pair[0]='IM_' if($pair[0] eq 'IM');

	$cmd_cyclic=1 if($pair[0] =~ /^(LDIR|LDDR|OTIR|OTDR|INIR|INDR|CPDR|CPIR|DJNZ)$/);

	if($pair[0] eq 'shift')
	{
		if($def ne 'base')
		{
			$$fnname = "NULL"; #"opc_".uc($def).$pair[1];
			return("");
		}

		$$fnname = "op_p_".$pair[1];
		return("static void ".$$fnname."(Z80EX_CONTEXT *cpu)\n{\n	cpu->prefix=0x".$pair[1].";\n	cpu->noint_once=1;\n}\n");
	}
	elsif($pair[0] eq 'ignore' or $pair[0] eq 'reset')
	{
		$$fnname = "NULL";
		return("");
	}

	my @tst = split /\//,$$opc{t}; #t-states
	if($def ne 'base')
	{
		#for prefixed ops: min. time used to fetch the prefix (4) must be substracted
		$tst[0]-=4;
		$tst[1]-=4 if($tst[1]);
	}
	
	if(defined($pair[1])) #mnemonic with args
	{
		my $dbus_arg=0; #number of argument that references memory, or 0
		@arguments = split /,/,$pair[1]; #split args

		if(($pair[0] eq 'JP') and ($arguments[0] =~ /^(HL|IX|IY)$/))
		{
			$pair[0] = 'JP_NO_MPTR';
		}

		#support for ld a, rlc(ix+d) -alike constructs
		if($pair[2])
		{
			splice @arguments,$#arguments+1,1,(split /,/,$pair[2]);
		}

		die "err1" if($#arguments > 1 and !$pair[2]);
		
		my $nnn=1;
		foreach my $argg (@arguments)
		{
#print "$arg--";		
			$dbus_arg = $nnn if($argg =~ /^\([\w\+\@\#\$]+\)$/);
			$nnn++;
		}
#print "\n";		

		$cmd_branch=1 if(($#arguments && $pair[0] =~ /^(CALL|JP|JR)$/) || $pair[0] =~ /^RET$/);

		#command works with port or memory data... clarify its type
		if($dbus_arg)
		{
			if($pair[0] eq 'LD')
			{			
				if($arguments[1] =~ /^(RLC|RRC|RL|RR|SLA|SRA|SLL|SRL|SET|RES)$/)
				{
					$cmd_mwrite = 1;
				}
				elsif($dbus_arg == 1) #first arg is (ptr)
				{
					die ("LD with one arg!") if(!$arguments[1]);
					$cmd_mwrite = 1;
					$cmd_is_16bit=is_arg_16bit($arguments[1]);
					
					if(($arguments[0] =~ /^\((BC|DE|\@)\)$/) and ($arguments[1] eq 'A'))
					{
						#ld (nnnn|BC|DE), A
						$pair[0]='LD_A_TO_ADDR_MPTR';
						$cmd_memptr=1;
					}
					elsif($arguments[0] =~ /^\(\@\)$/)
					{
						#ld (nnnn),rp
						$pair[0]='LD_RP_TO_ADDR_MPTR_';
						$cmd_memptr=1;
					}
				}
				else
				{
					$cmd_is_16bit=is_arg_16bit($arguments[0]);
					
					if(($arguments[0] eq 'A') and ($arguments[1] =~ /^\((BC|DE|\@)\)$/))
					{
						#ld a,(BC|DE|nnnn)
						$pair[0]='LD_A_FROM_ADDR_MPTR';
						$cmd_memptr=1;
					}
					elsif($arguments[1] =~ /^\(\@\)$/)
					{
						#ld rp,(nnnn)
						$pair[0]='LD_RP_FROM_ADDR_MPTR_';
						$cmd_memptr=1;
					}
					
				}
			}
			elsif($pair[0] eq 'EX')
			{
				die("no such EX") if ($dbus_arg == 2);
				$cmd_mwrite = 1;
				$cmd_is_16bit = 1;
				$cmd_memmod = 1;
				$pair[0]='EX_MPTR';
			}
			elsif($pair[0] =~ /^(ADD|SUB|ADC|SBC)$/)
			{
				$cmd_mwrite = 0;
			}
			elsif($pair[0] eq 'INC' or $pair[0] eq 'DEC')
			{
				die("no such inc/dec") if ($dbus_arg == 2 or $arguments[1]);
				$cmd_mwrite = 1;
				$cmd_memmod = 1;
			}
			elsif($pair[0] =~ /^(RLC|RRC|RR|RL|SLA|SRA|SLL|SRL)$/)
			{
				die("no such rotate cmd") if ($dbus_arg == 2 or $arguments[1]);
				$cmd_mwrite = 1;
				$cmd_memmod = 1;
			}
			elsif($pair[0] eq 'BIT')
			{
				die("no such bit cmd") if ($dbus_arg == 1);
				$cmd_mwrite = 0;
			}
			elsif($pair[0] eq 'RES' or $pair[0] eq 'SET')
			{
				die("no such set/res cmd") if ($dbus_arg == 1);
				$cmd_mwrite = 1;
				$cmd_memmod = 1;
			}
			elsif($pair[0] eq 'JP')
			{
				$cmd_is_16bit = 1;
				$cmd_mwrite = 0;
			}
			elsif($pair[0] =~ /^(OR|XOR|AND)$/)
			{
				$cmd_mwrite = 0;
			}
			elsif($pair[0] eq 'CP')
			{
				$cmd_mwrite = 0;
			}
			elsif($pair[0] eq 'SUB')
			{
				$cmd_mwrite = 0;
			}
			elsif($pair[0] eq 'IN' or $pair[0] eq 'IN_F')
			{
				$cmd_pwrite = 0;
			}						
			elsif($pair[0] eq 'OUT')
			{
				$cmd_pwrite = 1;
			}									
			else
			{
				die("unknown cmd that references mem adress or port: $pair[0]\n")
			}
		}
		
		die "not all timings given for $$opc{'asm'}!" if((!defined($$opc{rd}) or !defined($$opc{wr})) and $cmd_memmod);
		
		#branching
		if($cmd_branch)
		{
			$out.= get_flag_co($arguments[0])." {\n";
			splice(@arguments,0,1);
		}
				
		#process the rest args (numbers or registers)
		
		my $nn=1;
		my $add_op=0;
		my $add_op_arg='';
		my $skip_next=0;
		#print "-- cmd_mwrite=$cmd_mwrite, dbus_arg=$dbus_arg --\n";		
		foreach my $arg (@arguments)
		{					
			if($skip_next)
			{
				$skip_next=0;
				$nn++;
				next;
			}
			
			if($pair[0] eq 'IM_')
			{
				$arg='IM'.$arg;
			}
					
			if($arg =~ /^(RLC|RRC|RL|RR|SLA|SRA|SLL|SRL)$/)
			{
				$add_op = $arg;
				$nn++;
				next;
			}
			elsif($arg =~ /^(SET|RES)$/)
			{
				$add_op = $arg;
				die "jopa" if(!$arguments[$nn] && !$arguments[$nn+1]);
				$add_op_arg = $arguments[$nn].',';
			
				$nn++;
				$skip_next=1;
				next;
			}
			elsif($arg eq '%') #offset
			{
				$head.="	temp_byte=READ_OP();\n";
				$head.="	temp_byte_s=(temp_byte & 0x80)? -(((~temp_byte) & 0x7f)+1): temp_byte;\n";
				$arg_str.="temp_byte_s";
			}

			elsif($arg =~ /^\#$/) #byte
			{
				$head.="	temp_byte=READ_OP();\n";
				$arg_str.="temp_byte";
			}
			
			elsif($arg =~ /^\@$/) #word
			{
				$head.="	temp_word.b.l=READ_OP();\n";
				$head.="	temp_word.b.h=READ_OP();\n";				
				$arg_str.="temp_word.w";
				
				$addr_mptr="temp_word.w" if($pair[0] =~/^(JP|JR|CALL)$/);
			}			
			elsif($arg =~ /^\(\@\)$/) #memory referenced by word
			{
				die "err22" if(!$dbus_arg or $dbus_arg!=$nn);

				$head.="	temp_addr.b.l=READ_OP();\n";						
				$head.="	temp_addr.b.h=READ_OP();\n";						

				$addr_mptr = "temp_addr.w";

				if($cmd_is_16bit)
				{
					$arg_str.="temp_word.w";

					if(!$cmd_mwrite or $cmd_memmod)
					{
						die "2nd shift-op for 16bit cmd" if($add_op);
					
						$head.="	".$readbyte."temp_word.b.l,temp_addr.w,".next_t(\@rr).");\n";
						$head.="	".$readbyte."temp_word.b.h,temp_addr.w+1,".next_t(\@rr).");\n";
					}
					
					if($cmd_mwrite)
					{
						die "wr isnt given for 16bit memory-write op" if(!$$opc{wr});
				
						$foot.="	".$writebyte."temp_addr.w,temp_word.b.l,".next_t(\@ww).");\n";
						$foot.="	".$writebyte."temp_addr.w+1,temp_word.b.h,".next_t(\@ww).");\n";
					}
				}
				else
				{
					$arg_str.="temp_byte";
					
					if(!$cmd_mwrite or $cmd_memmod)
					{
						$head.="	".$readbyte."temp_byte,temp_addr.w,".next_t(\@rr).");\n";
						$head.="	$add_op(".$add_op_arg."temp_byte,".next_t(\@rr).");\n" if($add_op);
					}
					
					if($cmd_mwrite)
					{
						if($add_op)
						{
							$head.="	".$readbyte."temp_byte,temp_addr.w,".next_t(\@rr).");\n";
							$head.="	$add_op(".$add_op_arg."temp_byte);\n" if($add_op);
						}
					
						$foot.="	".$writebyte."temp_addr.w,temp_byte,".next_t(\@ww).");\n";
					}
				}
			}
			elsif($arg =~ /^\(\#\)$/) #dealing with port
			{
				die "err22" if(!$dbus_arg or $dbus_arg!=$nn);
				die ("(\#) notation, but it isnt 'out' or 'in' command -- $pair[0]") if(!$cmd_pwrite and ($pair[0] !~ /^(IN|IN_F)$/));
				
				$head.="	temp_word.w=(READ_OP() + ( A << 8 ));\n";
				$arg_str.="temp_word.w";
				
				$pair[0].='_A'; # IN A,(nn) and OUT (nn),A behaves somewhat differently
				
			}
			elsif($arg eq '(C)') #dealing with port
			{
				die "err22" if(!$dbus_arg or $dbus_arg!=$nn);
				die ("(C) notation, but it isnt 'out' or 'in' command -- $pair[0]") if(!$cmd_pwrite and ($pair[0] !~ /^(IN|IN_F)$/));

				$arg_str.= "BC";
			}
			elsif($arg =~ /^\([\w\+\$]+\)$/) #memory adressing by regpair (or regpair+d)
			{
				die "err22: dbus_arg=$dbus_arg" if(!$dbus_arg or $dbus_arg!=$nn);
					
				(my $ref) = $arg =~ /^\(([\w\+\$]+)\)$/;			
				die "err66" if(!$ref);
				
				if($ref =~ /\+/) # (IX/IY + $)
				{
					#взять число и приписать его к ref вместо $
					if(($def ne 'ddcb') and ($def ne 'fdcb'))
					{
						$head.="	temp_byte=READ_OP();\n";
						$head.="	temp_byte_s=(temp_byte & 0x80)? -(((~temp_byte) & 0x7f)+1): temp_byte;\n";
					}
					
					(my $regg) = $ref =~ /^(\w+)\+\$$/;
					die "err 889 (ref=$ref)" if(!$regg);
					$ref = $regg.'+temp_byte_s';
					
					if($pair[0] eq 'BIT') #BIT n,(INDEX+$) is a special case...
					{
						$pair[0]="BIT_MPTR";
					}
					
					$head.="	MEMPTR=($ref);\n";
				}
				else
				{
					if(($ref eq 'HL') and ($pair[0] eq 'BIT')) #BIT n,(HL) is a special case...
					{
						$pair[0]="BIT_MPTR";
					}
				}
				
				$addr_mptr = "($ref)";

				#$head.="			temp_addr.w=$ref;\n";						

				if($cmd_is_16bit)
				{
					$arg_str.="temp_word.w";

					if(!$cmd_mwrite or $cmd_memmod)
					{
						die "2nd shift-op for 16bit cmd" if($add_op);
					
						$head.="	".$readbyte."temp_word.b.l,($ref),".next_t(\@rr).");\n";
						$head.="	".$readbyte."temp_word.b.h,($ref+1),".next_t(\@rr).");\n";					
					}

					if($cmd_mwrite)
					{
						die "wr isnt given for 16bit memory-write op" if(!$$opc{wr});


						$foot.="	".$writebyte."($ref),temp_word.b.l,".next_t(\@ww).");\n";
						$foot.="	".$writebyte."($ref+1),temp_word.b.h,".next_t(\@ww).");\n";
					}
				}
				else
				{
					$arg_str.="temp_byte";
					
					if(!$cmd_mwrite or $cmd_memmod)
					{
						$head.="	".$readbyte."temp_byte,($ref),".next_t(\@rr).");\n";
						$head.="	$add_op(".$add_op_arg."temp_byte);\n" if($add_op);

					}					
					
					if($cmd_mwrite)
					{
						if($add_op)
						{
							$head.="	".$readbyte."temp_byte,($ref),".next_t(\@rr).");\n";
							$head.="	$add_op(".$add_op_arg."temp_byte);\n" if($add_op);
						}
					
						$foot.="	".$writebyte."($ref),temp_byte,".next_t(\@ww).");\n";
					}
				}
			}			
			else
			{
				$arg_str.=$arg;
			}
			
			$arg_str.=',' if($nn-1 < $#arguments);
			$nn++;
		}
	}

	if($cmd_memptr) #pass adress to commands that modifyes MEMPTR register
	{
		$arg_str.= ', ' if($arg_str ne '');
		$arg_str.= $addr_mptr;
	}

	if(($pair[0] =~ /^(LD|ADD|ADC|SBC|INC|DEC)$/) or ($pair[0] =~ /^LD_/))
	{
		foreach my $aa (@arguments)
		{
			if(is_arg_16bit($aa))
			{
				$pair[0] = $pair[0].'16';
				last;
			}
		}
	}

		
	######## give timing info for some cmds 
	
	if($cmd_cyclic) #give ciclyc commands full timing info
	{
		die("block/cycle command, but only one timing given!") if(!$tst[0] or !$tst[0]);
		$arg_str.= ', ' if($arg_str ne '');
		$arg_str.= "/*t:*/ /*t1*/$tst[0],/*t2*/$tst[1]";
	}
	
	########

	if($#rr >= 0)
	{
		$arg_str.= ', ' if($arg_str ne '');
		$arg_str.= '/*rd*/';
	
		my $nnum=0;
		foreach my $rarg (@rr)  #it there's rd-timings left, pass them as arguments
		{
			$arg_str.=',' if($nnum);
			$arg_str.=$rarg;
			$nnum++;
		}
	}


	if($#ww >= 0)
	{
		$arg_str.= ', ' if($arg_str ne '');
		$arg_str.= '/*wr*/';
	
		my $nnum=0;
		foreach my $warg (@ww)  #it there's wr-timings left, pass them as arguments
		{
			$arg_str.=',' if($nnum);
			$arg_str.=$warg;
			$nnum++;
		}
	}
	
	
	$out.="	$pair[0]".'('.$arg_str.");\n" if($pair[0] ne 'NOP');
	
	if($cmd_branch)
	{
		die "branching cmd, two timings must be given!" if(!$tst[0] or !$tst[1]);
		$foot.="	T_WAIT_UNTIL($tst[1]);\n	}\n	else { T_WAIT_UNTIL($tst[0]);";
		
		if($cmd_branch and $pair[0] =~ /^(JP|CALL)$/)
		{
			$foot.="MEMPTR=$addr_mptr;"
		}
		
		$foot.="}\n";
	}
	else
	{
		$foot.="	T_WAIT_UNTIL($tst[0]);\n" if(!$cmd_cyclic);
	}

	$foot.="	return;";
	
	return($head.$out.$foot."\n}\n");
}


sub convert_to_c_disasm
{
	
}


sub process_ofile
{
	my $fname = shift;
	my $def = shift;
	my $disasmt = shift;	
	my $to_file = shift;
	my $fnname;
	my $pair;
	
	my @tbl=();
	my @tbl_dasm=();
	my %nm_hash=();
	
	print "/*processing $fname.dat...*/\n\n";
	
	open(NP,"<$fname.dat") or die ("cannot open $fname");
	open(OFILE,'>./opcodes_'.$def.'.c') or die ("cannot create ".'./'.$fname.'.c.inc') if($to_file);
	
	select(OFILE) if($to_file);
	
	my $header = "/* autogenerated from $fname.dat, do not edit */\n\n";
	
	print $header;
	
	while(<NP>)
	{
		my $out = "";
		chomp;
		next if(/^\s*$/);
		next if(/^#/);
		my %opc;

		if(lc($def) eq 'dd' or lc($def) eq 'ddcb')
		{
			$_ =~ s/REGISTER/IX/g;
		}
		elsif(lc($def) eq 'fd' or lc($def) eq 'fdcb')
		{
			$_ =~ s/REGISTER/IY/g;
		}
	
		#my($opcode, $mnemonic, $tstates, $w) = $_ =~ /^(\w+)=\"(.*)\"/;
		foreach $pair (split /\s\s+/)
		{
			die("wrong format -- tab detected") if($pair =~ /\t/);
		
			$pair =~ s/^\s*//;
			$pair =~ s/\s*$//;
			#print "pair = [$pair]\n";
		
			if($pair =~ /^0x\w\w$/)
			{
				$opc{"opcode"}=$pair;
			}
			else
			{
				my($nm,$val) = $pair =~ /^(\w+)=\"(.*)\"$/;
				die "parse error for $pair" if(!$nm or !defined($val));
				if($nm =~ /^(t|wr|rd)$/)
				{
					$opc{"$nm"}=$val;
				}
				else
				{
					$opc{"opcode"}=$nm;
					$opc{"asm"}=$val;
				}
			}
		}
	
		my $c = &convert_to_c(\%opc, $def, \$fnname);
		
		print "$c\n" if(!$nm_hash{$fnname} && $opc{"asm"});
		
		#add to tables
		if($opc{"asm"})
		{
			$tbl[hex($opc{"opcode"})]=$fnname;
			$nm_hash{$fnname}=1;
			
			if($opc{"t"} =~ /\//)
			{
				my @tts = split /\//,$opc{"t"};
				die "bad t format" if($#tts != 1);
				$tts[1]=0 if($tts[1] == $tts[0]);
				$tbl_dasm[hex($opc{"opcode"})]=[($opc{"asm"} =~ /^(ignore|reset)\s/)? '#': $opc{"asm"},$tts[0],$tts[1]];
			}
			else
			{
				$tbl_dasm[hex($opc{"opcode"})]=[($opc{"asm"} =~ /^(ignore|reset)\s/)? '#': $opc{"asm"},$opc{"t"},0];
			}
		}
		else
		{
			$tbl[hex($opc{"opcode"})]='#';
		}
	
		undef(%opc);
	}

	my$ii;
	
	#empty fields means NULL
	for($ii=0;$ii < 256;$ii++)
	{
		if(!defined($tbl[$ii]))
		{
			$tbl[$ii]='NULL';
			$tbl_dasm[$ii] = ['#',0,0];
		}
	}

	# for cloned BIT ddcb/fdcb opcodes 
	my $last;
	for($ii=255;$ii >= 0;$ii--)
	{
		if($tbl[$ii] eq '#')
		{
			$tbl[$ii]=$tbl[$last];
			$tbl_dasm[$ii]=$tbl_dasm[$last];
		}
		
		
		$last=$ii;
	}
	
	
	$$disasmt="\n\n/**/\nstatic const z80ex_opc_dasm dasm_".$def."[0x100] = {\n";
	
	my $footer="\n\n/**/\n";
	$footer .= "static const z80ex_opcode_fn opcodes_".$def."[0x100] = {\n";
	
	for($ii=0;$ii < 256;$ii++)
	{
		$footer.= sprintf(' %-14s',$tbl[$ii]);
		
		my $ddasm = ($tbl_dasm[$ii]->[0] eq '#')? 'NULL': '"'.$tbl_dasm[$ii]->[0].'"';
		
		$ddasm =~ s/AF_/AF'/g;
		
		$$disasmt.=sprintf('{ %-20s, %2d , %2d } /* %02X */',$ddasm,$tbl_dasm[$ii]->[1],$tbl_dasm[$ii]->[2],$ii);
		
		if($ii != 255)
		{
			$footer.=',';
			$$disasmt.=',';
		}
		
		$$disasmt.="\n";
		
		$footer.= "\n" if(($ii+1)/4 == int(($ii+1)/4));
	}
	
	chop $footer;
	$footer.="\n};\n";
	$$disasmt.="\n};\n";
	
	print $footer;
	
	close(OFILE) if($to_file);
	select(STDOUT);
	close(NP);
}

my $to_file = 1;
my $disasmt;

open(DFILE,'>./opcodes_dasm.c') or die ("cannot open dasm.c for writing\n");
print DFILE "/* autogenerated, do not edit */";

&process_ofile('./opcodes_base','base',\$disasmt,$to_file);
print DFILE $disasmt;
&process_ofile('./opcodes_cb','cb',\$disasmt,$to_file);
print DFILE $disasmt;
&process_ofile('./opcodes_ed','ed',\$disasmt,$to_file);
print DFILE $disasmt;
&process_ofile('./opcodes_ddfd','dd',\$disasmt,$to_file);
print DFILE $disasmt;
&process_ofile('./opcodes_ddfd','fd',\$disasmt,$to_file);
print DFILE $disasmt;
&process_ofile('./opcodes_ddfdcb','ddcb',\$disasmt,$to_file);
print DFILE $disasmt;
&process_ofile('./opcodes_ddfdcb','fdcb',\$disasmt,$to_file);
print DFILE $disasmt;

close(DFILE);

