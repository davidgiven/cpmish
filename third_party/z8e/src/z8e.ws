.op
.rm 75




                                    Z8Å

               Z80/HD6418° ASSEMBÌÙ LANGUAGÅ DEBUGGINÇ TOOL

                               USER'Ó MANUAÌ








                              Copyrighô 198´

                               Ricë Surwilo
                             330 Calviî Court
                         Wyckoff, New Jersey 07481


                   Z180/HD6418° portionó Copyright 1988

                               Damon Gibson
                         8201 Castleton Boulevard
                       Indianapolis, Indiana  46256
                              (317) 842-7128


         Thió documenô anä thå softwarå iô describeó ió maintaineä by

                                Joî Saxton
                               Lakå Barrine
                              Queenslanä 4884
                                 Australia
                            tesseract@triton.vg


.paŠ.he                  TABLE OF CONTENTS
.fi z8e.toc
.paŠ.he Z8E REFERENCE MANUAL v 4.1 - February 2015
.pn 1
.fo                                   ­ £ -
.tc INTRODUCTION ............................................... # 
I® INTRODUCTION

Z8Å ió á professionaì qualitù interactivå debugginç tooì designeä tï assisô 
iî  thå testinç oæ assemblù languagå programó foò thå Z8° anä  Z180/HD6418° 
processors.

Originallù writteî aó á standalonå monitor¬ thå Z8° versioî oæ Z8Å waó useä 
iî thå developemenô oæ thå world'ó largesô Toucè-Tonå Input/Voicå  Responså 
system®  No÷ redonå tï ruî iî á CP/Í oò TurboDOÓ environmenô  Z8Å  containó 
morå  featureó  iî  lesó  memorù  thaî  anù  comparablå  softwarå  product® 
Occupyinç jusô oveò 9Ë oæ memory¬ Z8Å includeó thå followinç amonç itó manù 
features:

.rm 70
.pm 3
.lm 6
   - Supporô  foò  thå Hitaché HD6418° (Z180©  instructioî  set®  (Anù 
     mentioî  oæ  thå Z8° iî thió documenô caî bå takeî tï  implù  thå 
     Z18° aó well.)

   - Fulì  screeî animateä displaù oæ thå prograí undeò tesô whilå  iô 
     ió beinç executeä bù thå Z80® 

   - Completå Z8° inlinå assembler¬ witè labels¬ symbols¬ expressions¬ 
     anä directives¬ usinç Ziloç mnemonics

   - Interactivå  disassemblù witè labeló anä symboló  tï  consolå  oò 
     disë allowó thå useò tï specifù  outpuô formató anä adä commentó 

   - Fullù  traceä prograí executioî includinç á fulì   screeî  singlå 
     steğ commanä thaô instructó Z8Å tï  disassemblå codå anä tï  movå 
     thå cursoò tï thå  nexô instructioî tï execute

   - Uğ tï 1¶ useò settablå breakpointó witè optionaì  pasó counts

   - Truå symboliã debugginç usinç thå inpuô froí  multiplå  Microsofô 
     MACRÏ-8°  .PRÎ anä LINË-8°  .SYÍ fileó anä Z80ASÍ .LSÔ anä  SLRNË 
     anä Z80ASÍ .SYÍ fileó froí SLÒ Systems® 

   - Dynamiã relocatioî oæ Z8Å aô loaä timå tï thå toğ  oæ useò memorù 
     regardlesó  oæ  size®  Nï  useò  configuratioî  oæ  anù  kinä  ió 
     required® 

   - Supporô  foò  á  separatå  debugginç  terminal®   Thå   debugginç 
     activitù  caî  bå ruî oî á terminaì attacheä  tï  thå  computer'ó 
     seriaì porô whilå thå applicatioî beinç debuggeä operateó oî  thå 
     console.
.pm
.lm
.rm 75
.paŠ.tc INSTALLATION ............................................... # 
II INSTALLATIOÎ

Thió sectioî describeó ho÷ tï configurå anä builä Z8Å foò youò hardwarå anä 
softwarå environment.

Versionó  prioò tï 4.° alloweä binarù patchinç buô thaô approacè  haó  beeî 
abandoneä oî thå premiså thaô iô ió easieò tï configurå thå prograí aô  thå 
sourcå codå leveì anä reassemble® Anyonå whï ió usinç á prograí likå Z8Å tï 
debuç aô thå assemblù languagå leveì ió surelù goinç tï havå alì thå  tooló 
needeä  tï perforí thió task® Therå ió aî addeä advantage® Binarù  patchinç 
leaveó  fe÷ traceó anä caî bå difficulô tï reproducå wheî á ne÷ versioî  ió 
releaseä buô sourcå patcheó arå easù tï savå anä reapply.

The tasks needed to build Z8E from the source are as follows ...
.tc    Choose the assembler .................................... #
1. Edit Z8E.Z80 to choose the assembler.

Z8Å  ió writteî iî Ziloç mnemonicó sï Microsoft'ó M8° oò SLR'ó  Z80ASÍ  arå 
gooä  choices®  Anotheò candidatå ió thå Cromemcï CDOÓ Z8°  Macrï Assembleò 
whicè  ió availablå  froí  http://www.softsector.hostingsiteforfree.com/cpí 
anä  á slightlù differenô buô fullù functionaì versioî ió iî thå  Tesseracô 
collectioî aó volumå 95®

Withouô  doinç anything¬ thå sourcå wilì builä witè Z80ASÍ sï iæ  yoõ  havå 
thaô  assembleò  theî  yoõ  caî  skiğ thió  step®   (Oæ  courså  otheò  SLÒ 
assembleró  whicè recogniså Ziloç mnemonicó sucè aó SLR18° anä Z80ASMĞ  caî 
bå useä iî lieõ oæ Z80ASM.)

Thå requirementó foò á differenô assembleò arå fairlù simple®  Iô musô havå 
á  basiã macrï facility¬ recogniså Ziloç mnemonics¬ bå ablå tï  reaä  inpuô 
froí externaì fileó anä supporô conditionaì assembly.

Alì threå assembleró yielä .COÍ fileó whicè arå bit-for-biô identical.

.tc    Instruction set and number of terminals ................. #
2. Instructioî seô and number of terminals

Z8Å caî debuç programó foò thå Z8° oò thå Z18° (Hitaché HD64180© anä iô caî 
takå  advantagå  oæ á seconä terminaì iæ onå ió attached® Thaô  makeó  fouò 
combinations:

	Z80	1 terminal		Z80	2 terminals
	Z180	1 terminal		Z180	2 terminals

Havinç twï terminaló ió usefuì wheî debugginç programó whicè displaù á  loô 
oæ  texô oî thå screen® Iô ió convenienô tï havå thå debugginç sessioî  ruî 
oî  á  seconä  screeî  sï  thaô thå  outputó  froí  thå  debuggeò  anä  thå 
applicatioî don'ô treaä alì oveò eacè other®  Iæ yoõ arå runninç CP/Í oî aî 
emulatoò  sucè aó z80pacë oò yazå-aç theî thaô seconä screeî mighô bå  jusô 
anotheò  windo÷ oî thå hosô computeò logicallù attacheä tï thå seriaì  porô 
oæ thå emulateä system.

Four little include files, Z8E1.INC, Z8E2.INC, 18E1.INC and 18E2.INC are
included in this distribution.

	Z8E1.INC		Z80 instructions, one terminal
	Z8E2.INC		Z80 instructions, two terminals
	18E1.INC		HD64180/Z180 instructions, one terminal
	18E2.INC		HD64180/Z180 instructions, two terminals

Copù onå oæ thoså tï havå thå namå CONFIG.INC® Looë aô thå filå anä ediô iô 
iæ necessary.
.cp 6Š.tc    Choose or create the terminal handlers .................. #
3. Choose or create the terminal handlers

Á  fe÷  .TTÙ fileó arå supplied® Eacè oæ theså fileó defineó thå  codå  anä 
datá specifiã tï á particulaò terminal® Tï supporô á differenô terminal:
.cp 3
   a)	make a copy of one of the existing files
   b)	edit the copy to match the characteristics of the new terminal
   c)	edit TERMINAL.INC to add new table entries for the new terminal

.tc    Assemble the program .................................... #
4. Assemble the program

	SLR:	z80asm z8e/a

	M80:	m80 =z8e.z80
		l80 z8e,z8e/n/e

        ZASMBº  zasmâ z8e
                linkâ z8e,z8e/n/e

Alì threå assembleró generatå identicaì .COÍ files.

.tc    Coding caveat ........................................... #
IMPORTANTº Codinç caveat

.lm 6
.rm 70
     Z8Å imposeó onlù onå restrictioî oî thå codå yoõ write®  Iî ordeò 
     tï guaranteå thaô youò routineó caî bå relocateä intï higè memorù 
     bù  Z8Å  dï noô loaä anù 1¶ biô  constantó intï  registeò  pairs» 
     insteaä  dï  twï  ¸  biô loads®  Foò  example¬  dï  noô  uså  thå 
     followinç statåment:

                         ld     de,1234h

     Rather¬ codå iô likå thisº

                         ld     d,12h
                         ld     e,34h

     Witè  lucë  yoõ  won'ô  neeä tï loaä á  constanô  intï  aî  indeø 
     registeò  buô iæ yoõ dï therå arå á fe÷ wayó arounä thå  problem®  
     Thió  onå wilì worë oî á Z8° anä á Z180¬ doeó noô depenä  oî  thå 
     assembleò  chosen¬ doeó noô toucè anù otheò registeò anä  ió  thå 
     shortesô possiblå coding:

     Insteaä ofº    lä      ix,5678è            lä      iy¬5678h

     codeº          defâ    0DDè                defâ    0FDh
                    lä      h,56è               lä      h,56h
                    defâ    0DDè                defâ    0FDh
                    lä      l,78è               lä      l,78h
.lm
.rm 75
.paŠ.tc INVOKING Z8E ............................................... #
III®  INVOKINÇ Z8Å AÔ THÅ CP/Í COMMANÄ LEVEL

Upoî  invocatioî aô thå CP/Í commanä leveì Z8Å loadó aô thå lo÷ enä oæ  thå 
Transienô Prograí Areá (TPA© whicè beginó aô absolutå addresó 100h® Thå TPÁ 
ió thå areá iî memorù wherå useò programó arå loadeä anä run®

Oncå  loadeä  Z8Å determineó thå sizå oæ thå TPÁ bù examininç  thå  addresó 
fielä  oæ thå jumğ instructioî aô locatioî 5®  Thió aädresó representó botè 
thå  entrù poinô intï CP/Í anä thå enä oæ thå TPA® Z8Å loweró thió  addresó 
bù  approximatelù  9Ë byteó anä relocateó intï thió areá bù  adjustinç  alì 
addresseó  withiî itselæ tï reflecô itó ne÷ location® Thå jumğ  instructioî 
aô  locatioî µ ió similiarlù modifieä tï reflecô thå ne÷ sizå oæ  thå  TPA® 
Thuó  alì  programó  whicè  uså thió addresó tï  determinå  thå  amounô  oæ 
availablå  memorù  caî ruî unchanged® Z8Å completeó itó  initialézatioî  bù 
storinç  á  jumğ instructioî tï itó breakpoinô handlinç softwarå  whicè  ió 
normallù aô absolutå addresó 3¸ (hexadecimal).

Symboló whicè arå loadeä froí fileó arå storeä bù Z8Å iî á symboì tablå  aô 
thå  toğ  oæ  thå TPÁ jusô belo÷ Z8E® Z8Å  wilì  dynamicallù  allocatå  thå 
storagå necessarù tï holä alì symboló loadeä froí files» however¬ Z8Å  alsï 
allowó  thå  useò  tï  enteò hió owî symboló  froí  thå  keyboarä  viá  thå 
(A)ssemblå  command®  Z8Å  doeó NOÔ reservå ANÙ spacå iî  memorù  foò  useò 
generateä  symbols®  Thå useò musô explicitlù requesô memorù spacå  oî  thå 
CP/Í  commanä line® Thió ió accomplisheä bù enterinç thå numbeò oæ  symboló 
foò whicè spacå shoulä bå reserveä aó á decimaì number® Thió numbeò musô bå 
encloseä  iî  parentheseó  anä musô appeaò aó thå  firsô  argumenô  oî  thå 
commanä linå aó showî below:

           A>Z8Å (32©

Iî  thió examplå thå useò haó requesteä spacå foò 3² useò defineä  symbols® 
Iæ  MAXLEÎ  haó  bå seô tï ¶ (Seå INSTALLATIOÎ Section©  theî  eacè  symboì 
requireó ¸ byteó oæ storage¬ hence¬ iî thió examplå Z8Å wilì seô asidå  25¶ 
byteó oæ memorù foò useò defineä symbols® 

Subsequenô  actioî ió baseä oî thå formaô oæ thå remaindeò oæ  thå  commanä 
linå aó entereä bù thå user®  Iî thå exampleó thaô follo÷ beaò iî minä thaô 
anù oæ theså commanä lineó maù contaiî thå argumenô requestinç memorù spacå 
foò useò symboì tablå entries® Thå argumenô woulä appeaò immmediatelù afteò 
"Z8E¢ iî everù case.

.cp 3
     1®    A>Z8Å
.rm 70
.lm 16
                
               Z8Å resideó aó á standalonå prograí iî memory.

.lm
.cp 4
     2®    A>Z8Å  USERFILE.COÍ

.rr            L----!----!----!----!----!----!----!----!-------------R
               USERFILE.COÍ ió loadeä aô thå beginninç oæ thå TPÁ  anä 
               ió readù tï bå acteä oî bù Z8Å coí mands® 
.lm

.cp 4
.RR-----------------!----!----!----!----!----!----!----!-------------R
     3®    A>Z8Å  USERFILE.COÍ USERFILE.SYÍ [,biasİ

.rr            L----!----!----!----!----!----!----!----!-------------R
               USERFILE.SYÍ  ió reaä iî bù Z8Å anä alì  symboì   nameó 
               containeä  iî thå filå arå entereä intï á  tablå  whicè 
               beginó  aô  thå startinç addresó oæ   Z8Å  (thå  endinç 
               addresó  oæ  thå "new¢ TPA© anä   extendó  downwarä  iî 
               memory®  Thå optionaì  bias¬ iæ specified¬ ió á 1¶  biô 
               valuå  whicè   wilì  bå addeä tï  thå  1¶  biô  addresó 
               associateä   witè  eacè symboì iî thå  file®  (Iî  thió 
               examplå  á  .SYÍ  filå ió  shown»  however¬  sincå  alì  
               addresseó  appearinç  iî á .SYÍ filå arå  absolutå  thå Š               optionaì biaó woulä probablù noô bå  used.)

               USERFILE.COÍ  ió loadeä aô thå starô oæ thå  TPÁ   onlù 
               afteò thå .SYÍ filå haó beeî reaä anä thå  symboì tablå 
               built®  
.lm

.cp 4
.rr-----------------!----!----!----!----!----!----!----!-------------R
     4®    A>Z8Å  USERFILE.COÍ  USERFILE.PRÎ [,biasİ

.rr            L----!----!----!----!----!----!----!----!-------------R
               Aó  iî thå previouó examplå USERFILE.COÍ ió  loadeä  aô 
               thå beginninç oæ thå TPA¬ buô iî  thió instancå á  .PRÎ 
               filå  ió  useä  tï construcô   thå  symboì  table®  Thå 
               optionaì biaó becomeó  verù usefuì iæ thå .LSÔ oò  .PRÎ 
               filå  representó thå listinç oæ á relocatablå  program®  
               Relocatablå programó linkeä usinç Microsoft'ó   LINË-8° 
               defaulô tï á loaä addresó oæ 103È witè  thå threå byteó 
               oæ  memorù  locateä aô 100È  containinç á jumğ  tï  thå 
               entrù  poinô  oæ thå  program® Therefore¬ iæ  thå  useò 
               supplieó  á   biaó  oæ  10³ iî  thå  commanä  linå  alì 
               relocaô­  ablå symboló iî thå filå wilì  bå  associateä  
               witè  theiò  actuaì  addresseó  iî  memory®  Anù   biaó 
               specifieä  wilì onlù bå addeä tï thoså   symboló  whicè 
               arå flaggeä aó codå relativå iî  thå .PRÎ file® Á  biaó 
               wilì  noô bå addeä tï anù  symboì flaggeä aó  ABSOLUTE¬ 
               EXTERANL¬ OÒ  COMMON.

               USERFILE.COÍ  ió loadeä aô thå starô oæ thå  TPÁ   onlù 
               afteò  thå  .LSÔ oò .PRÎ filå haó beeî  reaä   anä  thå 
               symboì tablå built.
.lm

.cp 4
     5®    A>Z8Å USERFILE.COÍ USERFILE.SYÍ [,biasİ NFILE.LSÔ [,biasİ

.lm 16                              
               Thå  truå  poweò  oæ  Z8E'ó  symboì  loadinç  ió   besô  
               evidenceä  wheî  loadinç multiplå symboì  tableó   froí 
               severaì files® Thå firsô filå ió generallù á .SYÍ  filå 
               specifyinç alì thå globaì  symboì nameó iî thå  prograí 
               tï  bå tested® Thå  subsequenô fileó specifieä  oî  thå 
               commanä  linå   arå usuallù .PRÎ oò .LSÔ fileó  oæ  thå 
               individuaì   sourcå   moduleó  thaô   werå   originallù 
               assembleä  anä  theî linkeä (whicè produceä  thå   .SYÍ 
               file)®  Althougè onlù twï fileó (USERFILÅ   anä  NFILE© 
               arå  showî iî thió examplå thå numbeò oæ .SYÍ anä  .PRÎ 
               fileó specifieä iî thå commanä linå ió limiteä onlù  bù  
               thå sizå oæ Z8E'ó inpuô buffeò whicè ió 8°   characteró 
               long.


               USERFILE.COÍ  ió loadeä aô thå starô oæ thå   TPÁ  onlù 
               afteò alì .SYÍ anä .PRN/.LSÔ fileó  havå beeî reaä  anä 
               thå symboì tablå built.

               Iæ nï biaó ió specified¬ Z8Å wilì uså á biaó  oæ zero® 
.lm
.rm 75

.cp 3
Iæ morå thaî onå .LSÔ oò .PRÎ filå ió beinç loaded¬ theî eacè filå namå caî 
bå  specifieä witè itó owî bias® Thå biaó maù bå entereä iî thå forí  oæ  á 
symboì name¬ hexadecimaì number¬ decimaì number¬ oò anù combinatiioî oæ thå 
threå  iî  aî  expressioî usinç thå « anä ­ operators®  Iæ  thå  individuaì 
modulå  haó á globaì entrù point¬ thå namå oæ whicè waó previouslù  loaded¬ 
thå useò caî biaó alì symboló witè thå valuå associateä witè thió name®  Iî 
thió  waù alì symbols¬ botè absolutå anä relocatable¬ arå  associateä  witè 
theiò actuaì locatioî iî memory.
Š.cp 2
Z8Å  aó presentlù configureä caî builä á symboì tablå froí thå  lisô  fileó 
produceä bù thå followinç programs:

    1® Microsofô   MACRO-8°    V3.3·  .PRÎ fileó  Maù 8¬ 198°
    2® Microsofô   MACRO-8°    V3.4´  .PRÎ fileó  Deã 9¬ 198±
    3® Microsofô   LINK-8°     V3.4´  .SYÍ fileó  Deã 9¬ 198± 
    4® SLÒ Systemó Z80ASÍ      V1.0·  .LSÔ fileó
    5® SLÒ Systemó SLRNË       V1.0·  .SYÍ fileó
    6® SLÒ Systemó Z80ASÍ      V1.³   .PRÎ fileó  

Z80ASÍ anä SLRNË maù bå configureä foò 8° oò 13² columî output®

Z8Å  useó  thå filå namå extensioî (thå threå characteró appearinç  tï  thå 
righô oæ thå period© tï determinå thå formaô oæ thå file® Eacè oæ thå abovå 
filå typeó haó á distinguishinç format® Thå characteriticó oæ eacè typå arå 
describeä iî APPENDIØ A.
         
Durinç  thå loadinç procesó Z8Å displayó statuó anä erroò messageó  oî  thå 
consolå  relatinç  tï  thå activitù iî progresó  aó  showî  belowº

.rm 70
.lm 6
     STATUÓ MESSAGES

     1. Loading: USERFILE.COM

.lm 16                     
               Z8Å  ió  attemptinç tï opeî  thå nameä  filå  (iî  thió  
               case¬ USERFILE.COM)

.lm 6
     2.  Number of symbols loaded:

.lm 16                     
               Followinç  thå loadinç oæ  alì symboló froí  á  listinç  
               filå  oò  á .SYÍ file¬ thå  numbeò  oæ  symboló  loadeä  
               froí  thå  specifieä filå ió  displayeä  aó  á  decimaì  
               number.

.lm 6
     3.   Loaded: 100  YYYY

.lm 16                     
               Z8Å displayó thå startinç Pagesº ZZÚ anä endinç  memorù 
               addresseó oæ thå targeô filå  (thå firsô filå specifieä  
               oî  thå CP/Í commanä linå  anä thå onå whicè  ió  goinç  
               tï bå debugged).

               "Pages:¢ referó tï thå  decimaì numbeò oæ pageó  anä ió 
               thå counô oæ 25¶  bytå pageó iî thå file®  Thió  numbeò 
               maù  subsequentlù bå useä witè thå CP/Í   SAVÅ  commanä 
               oncå thå debuç sessioî ends® 

.lm 6
     ERROÒ MESSAGÅS

     1.   File not found

.lm 16                     
               Thå  filå specifieä iî thå  commanä coulä noô bå  founä  
               oî thå specifieä drive.

.lm 6
     2.   Symbol table not found

.lm 16                     
               Thå  specifieä filå waó  founä buô diä noô  contaiî   á 
               properlù formatteä symboì table.

.lm 6
.cp 4Š     3.   Invalid offset - using 0000

               Thå  useò haó specifieä aî  invaliä offseô tï bå  addeä  
               tï eacè loadeä symbol® Z8Å  wilì continuå tï loaä  thió  
               symboì filå buô wilì noô  adä anù biaó tï thå  symbols® 
               Thió   erroò  maù  havå   occureä  becauså   thå   useò  
               specifieä aî offseô iî thå  forí oæ á symboì whicè  haä  
               noô  beeî  previouslù  loaded¬ oò thå  useò  maù   havå 
               specifieä á numeriã  valuå whicè containeä aî   illegaì 
               character.

.lm 16                     
      4.   Syntax Error

.lm 16                     
               Thå filå namå waó incorrectlù specified® 

.lm
.rm 75
Afteò  alì useò files¬ botè symboì fileó anä thå .COÍ filå tï bå  debugged¬ 
havå beeî loadeä Z8Å displayó currenô memorù usagå aó follows:

     Totaì Symbolsº    XXXØ
     Symboì Tableº     XXXØ ­ XXXØ
     Z8Å relocatedº    XXXØ ­ XXXØ
     Toğ oæ memoryº    XXXØ            

Iô ió importanô tï notå thaô Z8Å expectó thå fileó appearinç iî thå commanä 
linå tï bå appeaò iî á specifiã order® Thå firsô filå namå appearinç iî thå 
commanä  linå ió assumeä tï bå thå targeô filå whicè ió tï bå debugged®  Iô 
ió  alwayó thå lasô filå tï bå loaded® Alì filå nameó followinç thå  targeô 
filå  namå arå assumeä tï bå symboì inpuô fileó anä theù arå loadeä iî  thå 
ordeò iî whicè theù appear.

Thå  firsô  filå  nameä iî thå commanä linå ió alwayó  loadeä  startinç  aô 
addresó 10° hex® Thå "I¢ commanä containó aî optioî tï allo÷ thå filå tï bå 
loadeä  aô á differenô address® Thió featurå ió noô availablå aô  thå  CP/Í 
commanä linå level.

Foò á discussioî oæ thå formaô oæ symboì fileó seå APPENDIØ A®
.paŠ.tc INITIALIZATION ............................................. #
V® INITIALIZATION

Oncå Z8Å haó beeî loaded¬ anä haó iî turî loadeä alì fileó specifieä oî thå 
commanä  line¬  iô initializeó alì useò registeró tï ° witè  thå  followinç 
exceptions:

.rr  L--------------!----!----!----!----!----!----!----!-------------R
     Thå user'ó prograí counteò containó addresó 10° heø  whicè ió thå 
     starô oæ thå TPA® 

     Thå  user'ó stacë pointeò ió seô tï thå startinç addresó  oæ  Z8Å 
     (thå toğ oæ thå TPA© minuó two® Theså  twï byteó arå seô tï  zerï 
     iî accordancå witè CP/Í  convention® Wheî CP/Í loadó á prograí iô 
     initializeó   á  stacë  foò thå loadeä  prograí  bù  pushinç  thå 
     addresó   oæ  thå jumğ tï thå systeí warí booô routinå  ontï  it®  
     Thuó  useò  programó  (STAT.COÍ ió aî  example©  caî  chooså   tï 
     terminatå  themselveó  anä  returî tï CP/Í bù  executinç  aî  REÔ 
     througè  thió  addresó oî thå stack® Z8Å  accomplisheó  thå  samå 
     objectiveº  thå  000° oî thå  stacë permitó thå useò  prograí  tï 
     returî tï CP/Í viá  addresó 000° whicè alwayó containó á jumğ  tï 
     thå  system'ó warí booô routine®  

     Thå useò É (interrupt© registeò ió seô tï thå valuå  containeä iî 
     thå É registeò wheî Z8Å waó loaded®  Modifù aô youò owî risk.

.rr-----------------!----!----!----!----!----!----!----!------------------R
Alì inpuô anä outpuô bù Z8Å ió accomplisheä usinç bufferó containeä  withiî 
itself® Z8Å doeó noô uså thå defaulô DMÁ buffeò aô absolutå locatioî 8° noò 
doeó iô uså thå defaulô Filå Controì Blocë (FCB© aô absolutå locatioî 5C® 

     Noteº                                           

.rr       L---------!----!----!----!----!----!----!----!-------------R
          Wheî  CP/Í finisheó loadinç anù program¬  includinç Z8E¬  iô 
          moveó  thå commanä linå  taiì tï thå defaulô DMÁ  buffeò  aô 
          absolutå   addresó 8° (hex© anä initializeó thå defaulô  FCÂ 
          aô  absolutå addresó 5Ã tï thå  namå oæ thå firsô  filå  (oò 
          firsô twï fileó  iæ twï oò morå arå specified© appearinç  iî  
          thå  commanä  line® Z8Å makeó uså oæ  thió   informatioî  iî 
          ordeò tï loaä thå useò  prograí anä anù symboì files® Iæ thå  
          prograí  tï  bå  testeä alsï expectó aî  iné­  tializeä  FCÂ 
          and/oò  DMÁ  buffeò (aó ió verù  ofteî thå case)¬  theî  thå 
          useò  musô  effecô  thió beforå attemptinç  tï  executå  thå  
          program® 

          Foò  example¬  manù texô editinç programó   arå  invokeä  bù 
          typinç thå namå oæ thå  editoò prograí followeä bù thå  namå 
          oæ  thå   prograí tï ediô oî thå CP/Í commanä line¬   aó  iî 
          hypotheticaì case:

                 A>EDIÔ B:FYL2EDIT.BAÓ

          Oncå  thå prograí EDIT.COÍ ió loadeä iô maù  expecô tï  finä 
          thå  defaulô  FCÂ  tï bå alreadù seô uğ  tï  reaä  thå  filå  
          FYL2EDIT.BAS®  EDIT.COÍ maù alsï expecô  thå DMÁ  buffeò  tï 
          contaiî  thå  numbeò oæ  characteró iî thå commanä  linå  aô 
          addresó   80¬  aó  welì thå thå texô oæ  thå  commanä   linå 
          startinç  aô addresó 81® Iî thió examplå locatioî  8°  woulä 
          contaiî á hexadecimaì Æ (decimaì 15© representinç thå numbeò 
          oæ  characters¬ anä locationó 8±  througè 8Æ  woulä  contaiî 
          thå 1µ characteró  (spacå througè S)® Similiarly¬ thå  firsô  
          bytå  oæ  thå defaulô FCÂ aô addresó 5Ã  woulä  contaiî  thå 
          numbeò  ± (numeriã equé­ valenô oæ drivå B© anä thå nexô  1± 
          byteó   woulä contaiî thå filå namå FYL2EDIÔ iî   ASCII®  Iæ Š          thå  namå FYL2EDIÔ waó shorteò  thaî ¸ characters¬ theî  thå 
          remaindeò  oæ   thå  filå namå fielä iî  thå  FCÂ  woulä  bå  
          filleä  witè ASCIÉ spaces® Thå nexô ³ byteó   woulä  contaiî 
          thå  filå typå iî ASCII» iî  thió examplå thå filå  typå  ió 
          BAS®  Iæ  nï   filå typå waó  specified¬  thió  fielä  woulä  
          contaiî ³ ASCIÉ spaces.

          No÷  iæ thå useò waó tï debuç thå EDIÔ  prograí  usinç  Z8E¬ 
          thió  initialisatioî oæ  thå defaulô DMÁ buffeò anä  defaulô 
          FCÂ  musô bå accomplisheä "bù hand¢ prioò tï  attemptinç  tï 
          debuç EDIT.COÍ owinç tï thå  facô thaô CP/Í haó alreadù  seô 
          uğ  theså  tï bå areaó witè thå datá froí thå  commanä  linå  
          whicè  waó  typeä iî tï loaä Z8E® Iî short¬   EDIÔ  musô  bå 
          trickeä intï believinç iô waó  loadeä bù CP/Í anä noô bù Z8Å 
          anä  thå useò musô perforí thå initializatioî oæ theså   twï 
          areas®   Thå useò maù uså thå "I¢ commanä tï initializå  thå 
          defaulô  commanä  linå buffeò anä FCB®  Furtheò  informatioî  
          regardinç thå formaô oæ thå FCÂ anä DMÁ  buffeò maù må founä 
          iî  Digitaì Research'ó  CP/Í 2.ø INTERFACÅ GUIDÅ oò  iî  thå 
          CP/Í ³ PROGRAMMER'Ó GUIDE.
.paŠ.rr-----------------!----!----!----!----!----!----!----!------------------R
.tc BREAKPOINTS ................................................ #
V. BREAKPOINTS

Breakpointó arå thoså addresseó iî thå prograí undeò tesô aô whicè thå useò 
wisheó  tï suspenä executioî anä returî controì tï Z8E® Thå useò  maù  set¬ 
clear¬ anä displaù breakpointó aô anù time¬ viá thå appropriatå commanä  iî 
responså tï Z8E'ó asterisë prompt® Z8E'ó implementatioî oæ breakpointó doeó 
noô  forcå  thå  useò tï tediouslù enteò breakpoinô  addresseó  everù  timå 
executioî  ió  resumed®  Rather¬ thå useò maù enteò  uğ  tï  1¶  breakpoinô 
addresseó anä eacè breakpoint¬ oncå set¬ ió storeä iî onå oæ Z8E'ó internaì 
tableó  anä remainó iî effecô untiì explicitlù cleareä bù thå useò viá  thå 
Cleaò breakpoinô commanä (seå Ã command).

Z8Å  alsï  allowó  yoõ tï specifù á pasó counô tï bå  associateä  witè  anù 
breakpoinô  thaô  ió  set®  Pasó countó indicatå  thå  numbeò  oæ  timeó  á 
particulaò instructioî musô bå executeä beforå Z8Å wilì regaiî control.

Furthermore¬  Z8Å doeó noô modifù anù codå iî thå useò prograí untiì  á  GÏ 
commanä  ió issueä (seå Ç command)® Thió permitó thå useò tï examinå  code¬ 
anä makå patcheó iæ desired¬ aô anù poinô iî thå debuç session.

Wheî  á breakpoinô ió reacheä iî thå useò prograí anä Z8Å regainó  control¬ 
thå  messageº *BP*XXXØ ió displayeä wherå XXXØ representó  thå  hexadecimaì 
addresó oæ thå breakpoint® Iî addition¬ Z8Å wilì displaù thå symboliã  namå 
oæ thió addresó iæ onå existó iî thå symboì table® Z8Å followó thió witè  á 
displaù  oæ  thå asterisë prompô indicatinç iô ió readù readù  foò  commanä 
processing.

Thå  messageº  *ERROR*BP*XXXØ  ió displayeä oî  thå  consolå  wheneveò  Z8Å 
determineó thaô controì haó beeî regaineä withouô á valiä breakpoinô havinç 
beeî reached® Thió ió generallù causeä bù á useò prograí whicè haó gonå ofæ 
thå  deeğ end® Iæ thå useò examineó thå currenô contentó oæ  thå  registeró 
(viá thå Ø command© thå currenô prograí counteò wilì mosô assuredlù contaiî 
aî  addresó  whicè haä noô previouslù beeî seô aó á breakpoint®  Thingó  tï 
looë foò wheî thió situatioî ariseó includeº á prograí thaô ble÷ itó stack¬ 
á prograí thaô performeä á ² 1/² gaineò witè á fulì twisô indirecô  througè 
á  register»  ie® JĞ (HL© intï thå greaô unknown¬ anä attemptinç  tï  tracå 
wherå wiså meî feaò tï treaä (BIOÓ anä BDOÓ I/Ï routines).

Z8Å  wilì allo÷ yoõ tï singlå steğ (trace© anä seô breakpointó anywherå  iî 
memory®  However¬  beaò  iî  minä  thaô aó yoõ  enteò  thå  BIOÓ  anä  BDOÓ 
netherworlä  youò  stacë pointeò wilì aô somå poinô bå  saveä  directlù  iî 
memorù  aó CP/Í switcheó tï itó owî stacë (youò stacë pointeò ió noô  saveä 
oî  á  stacë  bù  CP/M)® Iæ á breakpoinô haó beeî  seô  aô  aî  instructioî 
somewherå iî BDOÓ oò iî thå BIOÓ (afteò thió savå oæ youò stacë pointeò haó 
occured©  anä  thió  breakpoinô ió reached¬ Z8Å wilì  itselæ  calì  á  BDOÓ 
routinå  iî aî attempô tï displaù thå *BP*XXXØ messagå oî thå  console®  Aô 
thió poinô CP/Í wilì savå Z8E'ó stacë pointeò anä overlaù youró iî  memory® 
Wheî  BDOÓ  eventuallù  restoreó  thå stacë  pointeò  anä  executeó  á  REÔ 
instructioî yoõ wilì noô returî tï youò prograí anä youò stacë pointeò wilì 
bå gone® Theså routineó caî bå traced¬ albeiô witè difficulty¬ buô yoõ musô 
keeğ aî eyå oî whaô CP/Í ió doinç witè thå stacë pointer.

Aó distributed¬ Z8Å useó memorù locatioî 38è aó itó breakpoinô vector®   Iæ 
youò systeí softwarå useó 38è foò itó owî purposeó oò iæ yoõ arå  debugginç 
anotheò prograí whicè vectoró througè 38è theî yoõ maù wanô tï patcè Z8Å tï 
uså  á differenô vectoò address®  Changå thå valuå oæ RSTVEÃ tï onå oæ  thå 
otheò legaì values®  Instructionó foò doinç thió arå giveî iî sectioî 2.
.paŠ.tc COMMAND INPUT .............................................. #
.lm
.rm 75
VI®  COMMANÄ INPUT

Oncå  filå  anä symboì tablå loadinç haó beeî completed¬  Z8Å  promptó  thå 
operatoò  foò commanä inpuô bù displayinç thå "*¢ character®  Thå  operatoò 
caî theî typå anù oæ Z8E'ó singlå letteò commands® Somå commandó requirå nï 
argumentó  whilå otheró råquirå betweeî onå anä four® Argumentó maù bå  iî 
anù  oæ thå formó listeä belo÷ (excepô aó noteä iî thå descriptioî  oæ  thå 
indivéduaì commands)º 

.pm 6
.lm 21
.rm 70
      SYMBOL:       Anù   symboì  previouslù  loadeä   oò   previouslù  
                    entereä  viá  thå  keyboarä (seå  Á  command©  maù  
                    appeaò  aó  á commanä argument®  Alì  symboló  arå  
                    treateä aó 1¶ biô values.

        HEX:        Á  1¶  biô  heø  numbeò  maù  bå  entereä  aó   aî  
                    argument®  Onlù thå lasô fouò  characteró  entereä 
                    arå  treateä  aó  significanô  inpuô  iæ  Z8Å   ió 
                    expectinç  á 1¶ biô argument® Iî thoså   instanceó 
                    wherå Z8Å expectó á ¸ biô argument¬  onlù thå lasô 
                    twï characteró arå significant®  Aó such¬ thå useò 
                    maù   elecô   tï  correcô   mistakeó   bù   eitheò 
                    backspacinç  anä  retyping¬ oò  bù  continuinç  tï 
                    enteò  thå numbeò anä ensurinç thaô thå  erroneouó 
                    digiô doeó noô ağ­ peaò iî thå rightmosô fouò  (oò 
                    two© characteró aó showî iî thå followinç example:

                        *Å 1E21F´

.pm
                    Iæ á 1¶ biô argumenô ió expecteä  Z8Å woulä ignorå 
                    thå firsô twï  digitó (± anä E© anä woulä  examinå  
                    thå contentó oæ memorù locatioî  21F4.


                    Iæ  nï  symboì  tablå ió presenô  iî  memorù  theî  
                    hexadecimaì numberó (¸ oò 1¶ bitó iî length©   maù 
                    begiî witè anù digiô ° ­ F® However¬ iæ  á  symboì 
                    tablå  ió iî memorù theî alì  hexádecimaì  numberó 
                    whicè  begiî witè á digiô iî thå  rangå Á ­ Æ  arå 
                    evaluateä   firsô   aó  symboì    names®   Iæ   nï 
                    correspondinç  namå ió founä iî  thå symboì  tablå 
                    theî  Z8Å  attemptó tï rå-evaluatå thå namå  aó  á 
                    hexadecimaì  number® Foò  example¬ thå tokeî  DEAÄ 
                    ió á valiä symboì  nameó aó welì aó á valiä  hexá
                    decimaì number®  Iæ á symboì tablå ió presenô theî 
                    Z8Å  firsô  searcheó thå symboì tablå lookinç  foò 
                    thå   strinç  DEAD® Iæ nï matcè  occuró  theî  Z8Å  
                    treató  DEAÄ aó thå hexádecimaì numbeò 0DEAD®   Tï 
                    forcå   Z8Å   tï  evaluatå  aî   argumenô   aó   á  
                    hexadecimaì  numbeò  prefiø thå  argumenô  witè  á  
                    leadinç zerï aó iî 0DEAD® 


.pm 6
     REGISTER:      Valiä  Z8° 1¶ biô registeò nameó arå permitteä  aó 
                    arguments® Iæ á 1¶ biô registeò namå  ió  entered¬ 
                    Z8Å useó thå 1¶ biô valuå  currentlù containeä  iî 
                    thå   specifieä  registeò   paiò  iî  thå   user'ó 
                    registeò seô aó aî argument.
 
                         *Ä HÌ ¸

                    instructó  Z8Å tï dumğ thå firsô  eighô oæ  memorù Š                    byteó whicè arå  locateä aô thå addresó  containeä 
                    iî  thå user'ó HÌ registeò pair


                    Valiä 1¶ biô registeò namesº
                              
                              AÆ ­ Accumulatoò anä Flaç
                              BÃ ­ BÃ registeò paiò
                              DÅ ­ DÅ registeò paiò
                              HÌ ­ HÌ registeò paiò
                              SĞ ­ Stacë Pointeò
                              Ğ  ­ Prograí Counteò
                              PÃ ­ Prograí Counteò
                              IØ ­ IØ indeø registeò 
                              IÙ ­ IÙ indeø registeò

                    Notå thaô thå prograí counteò maù bå specifieä  iî 
                    eitheò  oæ twï ways® Thå singlå characteò "P¢  caî 
                    bå  useä tï specifù thå prograí  counteò  provideä 
                    iô  doeó noô appeaò iî aî  expression® Tï  includå 
                    thå  currenô valuå oæ  thå user'ó prograí  counteò 
                    iî aî expressioî  thå mnemoniã "PC¢ musô bå used.

                    Iæ  aî expressioî useä aó aî argumenô  containó  á 
                    registeò  paiò aó onå oæ itó terms¬  thå  registeò 
                    paiò  musô  bå  thå firsô  term®  Also¬  onlù  onå 
                    registeò paiò maù bå includeä iî aî  expression:

.rm 65
.pm 36
.lm 26
                         HL+´      valiä expressioî

                         5+DÅ      invaliä expressioî ­ registeò 
                                   paiò ió noô thå firsô terí

                        HL+BC      invaliä   expressioî   - morå 
                                   thaî  onå  registeò paiò  waó  
                                   specified

                         P-³       invaliä   expressioî   ­ "PC¢ 
                                   musô  bå useä tï includå  thå 
                                   currenô  valuå oæ thå prograí 
                                   counteò iî aî expressioî

.lm 21
.rm 70
                    Tï differentiatå betweeî thå hexadecimaì   numberó 
                    AF¬ BC¬ anä DÅ anä thå Z8° registeò  pairó oæ  thå 
                    samå namå bå surå tï prefiø thå  numericaì versioî 
                    witè  á leadinç 0®  Notå alsï thaô thå  Z8°  primå 
                    registeò  nameó   arå  noô  alloweä  aó  argumentó 
                    excepô iî thå Ò  command.

.cp 3
.pm 6
     REGISTER       Z8Å allowó thå useò tï specifù thå datá  contained
.pm 6
     INDIRECT:      iî  thå memorù locatioî pointeä tï bù á   registeò 
                    paiò aó aî argument® Foò instance¬  iæ thå  user'ó 
                    HÌ registeò paiò containeä 18EÅ  anä thå addresseó 
                    18EÅ  anä  18EÆ  containeä thå  byteó  4²  anä  6± 
                    respectively¬  theî  thå  commanä  *Å  (HL©  woulä 
                    examinå  thå  contentó  oæ memorù  locatioî  6142® 
                    Notå thaô registeò  indirecô memorù referenceó arå 
                    indicateä bù  enclosinç thå registeò paiò namå  iî 
                    PARENTHESEÓ  whicè  followó  thå  ZILOÇ   mnemoniã 
                    methoä oæ signifyinç "thå contentó of".
Š                    Thå  mosô usefuì applicatioî oæ registeò  indirecô 
                    argumentó  ió  tï seô breakpointó  aô   subroutinå 
                    returî  addresses®  Consideò thå  situatioî  oæ  á 
                    prograí  whicè  ió  currentlù   suspendeä  viá   á 
                    breakpoinô   somewherå   iî  thå   middlå   oæ   á 
                    subroutine®  Thå  useò ió  nï   longeò  interesteä 
                    debugginç  thå  bodù oæ thå  subroutine»  hå  onlù 
                    careó abouô gettinç bacë  tï thå instructioî  thaô 
                    followó   thå   CALÌ  thaô   goô  hií   intï   thå 
                    subroutine® Registeò indirecô formaô allowó hií tï 
                    enter:

                                     *Â (SP©

.cp 2
.pm
                    Thió  informó  Z8Å  tï seô  á  breakpoinô  aô  thå  
                    addresó pointeä tï bù thå stacë pointeò  register.


.pm 6
     DECIMAL:       Decimaì  numberó  iî thå rangå ° ­  6553µ  maù  bå  
                    entereä  aó  arguments® Alì digitó oæ  thå  numbeò 
                    musô  bå iî thå rangå °-9® Á decimaì  numbeò  musô 
                    bå  followeä  bù á "#¢ character¬   otherwiså  Z8Å 
                    wilì  treaô  iô aó á heø  number®   Thå  followinç 
                    examplå  showó  á decimaì numbeò  beinç  inpuô  aó 
                    parô oæ thå Å command:

                         *Å 512£  
                      
                              instructó  Z8Å  tï  examinå  memorù 
                              locatioî  51² decimaì (20° hex©


      LITERAL:      ASCIÉ  literaló  uğ  tï 7¸  byteó  iî  lengtè  arå  
                    permitteä aó argumentó (Z8E'ó inpuô buffeò ió   8° 
                    characteró  lonç  lesó thå openinç  anä   trailinç 
                    quotå   characters)®  ASCIÉ  literaló    musô   bå 
                    encloseä iî quotes® Thå quotå characteò itselæ  ió 
                    thå  onlù characteò noô peò­ mitteä aó á  literal® 
                    Commandó whicè dï noô  permiô thå uså oæ ARGUMENÔ-
                    STRINGó (seå below© wilì stilì accepô inpuô iî thå 
                    forí  oæ  quoteä strings® Iî sucè á caså Z8Å  wilì  
                    ignorå  alì  buô thå lasô twï  characteró  oæ  thå  
                    quoteä  literal¬ treatinç thå inpuô aó á  1¶   biô 
                    number® Foò examplå iæ thå useò entered:

                                    *Ú 'ABCD§

.pm
                    Z8Å woulä treaô 'BC§ aó á 1¶ biô numbeò anä  begiî 
                    disassemblinç aô addresó aô 4243.

.cp 3
.pm 6
     ARGUMENT-      Thå  Æ  (find)¬ Å (examinå memory)¬ Î  (querù  I/Ï 
     STRINGS:       portó withouô prå-read)¬ Ñ (querù I/Ï  ports)¬ anä 
                    Ù  (filì  memory©  commandó  permiô   thå  uså  oæ 
                    ARGUMENÔ-STRINGS¬  whicè arå simplù   combinationó 
                    oæ alì valiä argumenô typeó  separateä bù  commas® 
                    ARGUMENÔ-STRINGó  maù  bå  anù lengtè  uğ  tï  thå 
                    limiô  oæ  Z8E'ó inpuô  buffeò whicè ió  8°  byteó 
                    long® ARGUMENÔ-STRINGó maù bå terminateä bù eitheò 
                    á carriagå returî oò thå firsô spacå characteò noô  
                    appearinç   iî  betweeî  quotå   characters®   Thå  
                    followinç  ió aî examplå oæ á 1µ  bytå   ARGUMENÔ-Š                    STRINGÓ  strinç whicè combineó SYMBOLS¬  LITERALS¬ 
                    HEX¬ anä DECIMAÌ numbers:

                       SYMBOL,'xyZ',4F,12E4,9,21#,511#,'ABc§

.pm
                    Assuminç  thaô SYMBOÌ ió equaì tï 177È  theî   thå 
                    abovå ARGUMENÔ-STRINÇ woulä evaluatå toº  0± 7· 7¸ 
                    7¹ 5Á 4Æ 1² E´ 0¹ 1µ 0± FÆ 4± 4² 6³ 

                    Again¬ ARGUMENÔ-STRINGÓ arå terminateä bù eitheò á 
                    carriagå  returî oò bù thå firsô  spacå  characteò 
                    thaô doeó noô appeaò iî á quoteä literaì string.

.lm
.rm 75
Z8Å permitó expressionó usinç thå « anä ­ operators®  Anù argumenô typå maù 
bå  combineä  witè anù otheò type®  Thå lengtè oæ aî expressioî ió  limiteä 
onlù  bù thå sizå oæ thå inpuô buffer®  Expressionó arå evaluateä froí lefô 
tï righô anä thå uså oæ parentheseó ió noô permitted® 

Z8Å  indicateó argumenô erroró bù printinç á questioî mark®

Argumentó maù bå linå-editeä usinç thå standarä CP/Í controì characters:

               backspaceº  eraså thå lasô characteò typeä
               controì Xº  eraså thå entirå linå 
               controì Cº  returî tï CP/Í viá warí booô

Wheî  enterinç  argumentó  controì-Ø clearó thå  argumenô  list»  á  seconä 
controì-Ø clearó thå commanä linå anä causeó Z8Å tï reprompô foò á command.

Alì  inpuô  ió  truncateä tï thå sizå oæ Z8E'ó inpuô  buffeò  whicè  ió  8° 
characteró long® 

Alì alphabetiã inpuô tï Z8Å maù bå iî uppercaså oò lowercase® Alì outpuô bù 
Z8Å  followó  thå  dictateó oæ thå CASÅ bytå aó patcheä bù  thå  useò  (seå 
INSTALLATION).

Iî  thió  manuaì thå appearancå oæ squarå bracketó Û İ arounä  aî  argumenô 
alwayó indicateó thaô thå argumenô ió optional.

Afteò  loadinç  anù symboló anä thå applicatioî prograí Z8Å waitó  oî  inó
tructionó froí thå operator®  Thoså instructionó compriså  singlå-characteò 
commands®   Dependinç  oî thå specifiã command¬ operandó maù  bå  required®  
Agaiî dependinç oî thå command¬ pressinç thå 'return§ keù maù bå sufficienô 
tï  continuå thå command®  Wheî continuatioî ió possible¬ thå generaì  rulå 
ió  thaô 'return§ performó it¬ anù otheò keù terminateó thå  command®

Thå followinç pageó describå alì thå commandó understooä bù Z8E.
.paŠ.tc     A    Assemble .......................................... #
Á    Assemble
________________________________________________________________ß


Thå  Á commanä permitó thå useò tï effecô inlinå assemblù oæ Z8°  assembleò 
sourcå  code¬ includinç labeló anä symbols¬ usinç thå fulì Z8°  instructioî 
set® Iî addition¬ thå assembleò acceptó standarä Ziloç mnemonicó  (APPENDIØ 
B)¬ expressionó usinç thå « anä ­ operators¬ aó welì aó thå followinç  fivå 
assembleò  directivesº  ORG¬ DEFB¬ DDB¬ EQU¬ anä DEFW® Thå  formaô  oæ  thå 
commanä is:

        *Á  ARG±  <cr¾

          wherå  ARG±  representó thå startinç addresó  aô  whicè 
          assemblù wilì takå placå

          ARG± maù bå oæ anù typå


Z8Å  initiallù promptó thå useò bù firsô disassemblinç anä  displayinç  thå 
instructioî  currentlù  locateä aô thå addresó specifieä bù ARG1®  Thió  ió 
donå aó á conveniencå tï permiô thå useò tï ensurå thaô anù patcheó wilì bå 
assembleä intï memorù aô thå intendeä location® Z8Å theî outputó á carriagå 
return/linå feed¬ displayó thå addresó specifieä aó ARG1¬ anä awaitó input® 
Z8Å  wilì noô disassemblå beforå everù linå oæ sourcå codå entereä  bù  thå 
user¬ onlù beforå thå firsô one® 


Z8Å expectó assembleò inpuô iî thå followinç formatº


             LABELº  opcodå  [operand1İ [,operand2İ


Thå  labeì fielä ió alwayó optional¬ thå opcodå fielä ió mandatorù onlù  iæ 
nï labeì waó entered¬ anä thå operanä fielä musô naturallù bå includeä  foò 
thoså Z8° instructionó whicè requirå one® Thå threå fieldó maù bå separateä 
froí onå anotheò bù spaceó oò taâ characters.

Z8Å  doeó noô automaticallù reservå spacå withiî itselæ foò  useò  supplieä 
symboì names® Useò supplieä symbols¬ aó opposeä tï thoså loadeä froí files¬ 
arå  entereä  froí  thå keyboarä iî thå labeì fielä  usinç  thå  (A)ssemblå 
command®  Symboì  tablå spacå tï holä useò supplieä symboì  nameó  musô  bå 
explicitlù  requesteä oî thå CP/Í commanä linå aó explaineä iî thå  sectioî 
"INVOKINÇ  Z8Å aô oî thå CP/Í COMMANÄ LEVEL"® Theså useò supplieä  symbols¬ 
oncå  entered¬  maù bå referenceä iî thå operanä fielä  oæ  anù  subsequenô 
assemblù  statemenô  oò  iî thå argumenô fielä oæ anù  Z8Å  command®  Theså 
symboló  comå  iî handù wheî disassemblinç .COÍ fileó foò whicè  nï  sourcå 
listinç existó anä alsï wheî patchinç code.

.cp2
Thå  assembleò  ió á onå pasó assembleò anä forwarä referenceó  tï  symboló 
whicè  dï  noô alreadù appeaò iî thå symboì tablå arå  flaggeä  aó  errors® 
However¬  Z8Å  allowó thå uså oæ thå ORÇ directivå (seå  discussioî  below© 
whicè allowó thå useò tï manipulatå thå assembler'ó locatioî counter¬ whicè 
helpó tï minimizå thå nï forwarä referencå limitation.

Labeló maù begiî iî anù column¬ buô alì labeló musô bå followeä bù á  coloî 
eveî  thoså appearinç iî aî EQÕ statement® Labeló maù bå oæ anù lengtè  buô 
onlù thå firsô ¶ characteró arå significant® Z8Å alwayó assignó thå 1¶  biô 
valuå  oæ  thå  assembler'ó currenô locatioî counteò  tï  thå  labeì  beinç 
entered¬  unlesó  thå  statemenô ió aî EQÕ directive® Labeló  neeä  noô  bå Šfolloweä  bù aî opcodå anä thió (aó welì aó thå EQÕ directive©  provideó  á 
convenienô  waù  tï  assigî  á  valuå tï á  symboì  name®  Merelù  seô  thå 
assembleró  locatioî  counteò  (viá thå ORÇ directivå oò  aó  ARG±  iî  thå 
commanä  line© tï thå valuå yoõ wisè tï assign¬ theî typå thå  symboì  namå 
followeä  bù  á  carriagå return® Nï objecô codå ió produceä  anä  nï  useò 
memorù  areaó  arå  modifieä buô thå symboì anä itó  associateä  valuå  arå 
entereä  intï  thå useò symboì table® Z8Å doeó noô treaô  duplicatå  symboì 
nameó  aó  errors® Rather¬ iæ thå useò enteró á symboì namå  whicè  alreadù 
appearó  iî  thå  symboì table¬ thå valuå associateä witè  thå  ne÷  symboì 
replaceó thå onå associateä witè thå old® Foò example¬ iæ thå symboì ENTRYĞ 
existó  iî thå symboì tablå anä ió associateä witè thå valuå 23DÁ  anä  thå 
useò assembleó thå followinç instruction:

     41FÆ 0Å 0´       ENTRYPº LÄ C,´

Z8Å woulä replacå 23DÁ witè 41FF®

Assembleò  statementó whicè dï noô contaiî labeló maù begiî iî anù  column¬ 
includinç  columî  one® Therå ió NÏ neeä tï inserô á leadinç spacå  oò  taâ 
beforå aî opcodå iæ thå opcodå ió noô precedeä bù á label.

Operandó appearinç iî thå operanä fielä oæ thå instructioî tï bå  assembleä 
maù  bå anù oæ thå followinç typeó subjecô onlù tï thå provisï thaô 1¶  biô 
valueó cannoô appeaò aó operanä foò thoså Z8° instructionó whicè requirå  ¸ 
biô values® Expressionó combininç anù oæ thå followinç fouò typeó (witè thå 
« anä ­ operators© arå alsï permissible® 

               SYMBOÌ  (froí symboì table©
               HEØ
               LITERAÌ (twï byteó maximum©
               DECIMAÌ


Iî  additioî thå dollaò sigî ($© maù alsï appeaò iî botè thå operanä  fielä 
oæ  anù instructioî iî whicè á 1¶ biô operanä ió allowed¬ anä alsï  iî  thå 
operanä fielä oæ anù relativå jumğ instruction® Thå dollaò sigî  representó 
thå currenô valuå oæ thå assembler'ó locatioî counter¬ thaô is¬ thå addresó 
appearinç  oî thå linå aô whicè thå assemblù ió takinç place®  Thå  operanä 
fielä oæ á relativå jumğ instructioî caî bå entereä iî eitheò oæ twï  ways® 
Thå  useò maù codå thå operanä usinç thå dollaò sigî mentioneä abovå aó  iî 
thå followinç examples:


          JÒ  NZ,$+1±       ;jumğ tï addresó PC+1± (hex©     
          
          DJNÚ  $-24£       ;jumğ tï addresó PC-2´ (decimal©

Thå useò maù alternativelù specifù á 1¶ biô valuå iî thå operanä fielä oæ á 
relativå  jumğ instructioî anä leô Z8Å calculatå thå relativå  displacemenô 
froí thå assembler'ó prograí counteò aó showî below:

          JÒ    C,LABEÌ      Assuminç LABEÌ exists¬ iî thå symboì  
                             tablå Z8Å wilì calculatå thå offset® 
                             LABEÌ  musô  bå withiî +12¹ oò  -12¶  
                             byteó froí thå assembler'ó  locatioî  
                             counteò  oò  aî assemblù erroò  wilì  
                             result®

          JÒ      NZ,108°    Z8Å  calculateó thå displacemenô be­ 
                             tweeî thå assembler'ó currenô  loca­
                             tioî  counteò  anä thå addresó  108°  Š                             (hex)®
   
Z8Å  indicateó  erroò-freå inpuô bù firsô displayinç thå  resultanô  objecô 
codå  anä  theî  displayinç (oî thå nexô line© thå nexô  addresó  aô  whicè 
assemblù wilì takå place® 

Assemblù  erroró  arå alwayó indicateä bù á doublå paiò oæ  questioî  markó 
whicè appeaò followinç thå locatioî counter® Aî erroò flaç ió alsï  printeä 
anä wilì bå onå oæ thå following:

                ERROÒ FLAÇ               MEANINÇ

                    Ì         Labeì startó witè numeriã characteò
                                 
                    Ï         Invaliä opcodå

                    Ó         Syntaø erroò

                    Ô         Symboì tablå fulì                  

                    Õ         Instructioî referenceó aî undefineä
                              symboì namå

                    Ö         Valuå erroò ­ á 1¶ biô  valuå 
                              waó specifieä aó aî operanä foò 
                              aî  instructioî  whicè  permitó 
                              onlù ¸ biô numbers®

Iæ  aî  erroò  occurs¬ Z8Å wilì reprompô thå useò witè  thå  samå  locatioî 
counteò address.

Aó  waó  mentioneä  previouslù  thå  Z8Å  assmebleò  useó  standarä   Ziloç 
mnemonics®  Thå  onå  exceptioî tï thió ió thå EØ  AF,AF§  instruction®  Tï 
assemblå thió instructioî thå trailinç quotå characteò musô bå omitted.

Z8Å supportó thå ORÇ directivå whicè allowó thå useò tï changå thå valuå oæ 
thå  assemblù locatioî counter® Thå operanä fielä oæ thå ORÇ directivå  maù 
bå  á 1¶ biô argumenô oæ anù type® Afteò settinç thå ne÷ assemblù  locatioî 
counteò Z8Å displayó thå disassembleä instructioî aô thå ne÷ address.

Z8Å  supportó  thå DEFB¬ DEFW¬ anä DDÂ directiveó whicè givå thå  useò  thå 
abilitù  tï  assemblå  datá constantó intï memory®  DEFÂ acceptó aî  ¸  biô 
operand thå  valuå oæ whicè  is placeä intï  memorù aô thå addresó  oæ  thå 
assembler'ó currenô locatioî counter®  DEF× allowó thå useò tï specifù á 1¶ 
biô operanä value¬ thå lo÷ ordeò bytå oæ whicè ió placeä intï memorù aô thå 
addresó  oæ thå assembler'ó currenô locatioî counter¬ whilå thå higè  ordeò 
bytå oæ thå operanä ió placeä intï memorù aô thå addresó oæ thå assembler'ó 
currenô locatioî counteò pluó one® Thió ió iî accordancå witè thå  8080/Z8° 
conventioî  oæ storinç thå higè ordeò bytå oæ 1¶ biô datá towarä  thå  higè 
enä  oæ memory® Thå DDÂ (definå doublå byte© directivå allowó thå  useò  tï 
specifù á 1¶ biô valuå which¬ iî contrasô tï thå DEF× directive¬ ió  storeä 
iî memorù witè thå higè ordeò bytå towarä thå lo÷ enä oæ memory® Thaô is¬ á 
DDÂ  directivå instructó Z8Å tï storå thå mosô significanô bytå oæ  thå  1¶ 
biô  operanä  valuå  iî memorù aô thå addresó oæ  thå  assembler'ó  currenô 
locatioî counter¬ anä thå leasô significanô (lo÷ order© bytå ió placeä intï 
memorù aô thå addresó oæ thå assembler'ó currenô locatioî counteò pluó one® 

Thå  EQÕ  directivå allowó thå useò tï assigî á valuå tï á symbol®  Aî  EQÕ 
directivå  doeó  noô  generatå objecô code® Iô merelù allowó  thå  useò  tï 
referencå  á  numeriã  valuå  bù á symboliã  namå  iî  subsequenô  assemblù 
statementó oò monitoò commands® Iô ió especiallù usefuì wheî useä prioò  tï Šdisassemblinç  (seå Ú command© codå foò whicè nï symboì tablå  exists®  Thå 
EQÕ  directivå  requireó thå useò tï supplù á symboliã namå  iî  thå  labeì 
fielä  oæ thå instruction® Iæ Z8Å indicateó erroró iî aî EQÕ  statemenô  bù 
printinç questioî marks® Iæ aî EQÕ statemenô ió correctlù assembleä bù Z8E¬ 
thå addresó oæ thå assembler'ó currenô locatioî counteò ió eraseä sincå  aî 
EQÕ  statemenô  generateó  nï  objecô  code®  Operandó  appearinç  iî   EQÕ 
statementó  arå evaluateä tï á 1¶ biô value® Z8Å wilì displaù thå valuå  oæ 
thió  1¶  biô  numbeò aó fouò heø digitó iî thå objecô codå  fielä  oî  thå 
console®
.paŠ.tc     B    Set Breakpoint .................................... #
Â    Seô Breakpoint
________________________________________________________________ß


Breakpointó  arå  thoså  addresseó  aô whicè thå  useò  wisheó  tï  suspenä 
executioî oæ thå prograí undeò test® Breakpointó maù bå seô aô anù timå  iî 
responså  tï  Z8E'ó asterisë prompt® Z8Å allowó thå useò tï seô  uğ  tï  1¶ 
individuaì breakpointó iî hió program® Z8Å alsï allowó thå useò tï  specifù 
á  pasó counô tï bå associateä witè anù breakpoint® Thå commanä ió  invokeä 
aó follows:


     *Â ARG1[,pasó countİ [ARG2..® ARGnİ <cr¾

          wherå eacè argumenô representó thå addresó iî thå  useò 
          prograí aô whicè á breakpoinô ió tï bå seô 

Normally¬  thaô  ió wheî nï pasó counô ió specifed¬ executioî oæ  thå  useò 
prograí  stopó  anä controì returnó tï thå Z8Å commanä leveì aó sooî  aó  á 
breakpoinô ió reached® Pasó countó arå useä tï inforí Z8Å thaô executioî oæ 
thå useò prograí shoulä halô onlù wheî thå specifieä breakpoinô ió  reacheä 
thå numbeò oæ timeó timeó indicateä bù thå pasó count® 

Pasó countó arå specifieä bù followinç thå breakpoinô addresó witè á  commá 
anä theî enterinç á pasó counô immediatelù followinç thå comma.

Aî  existinç pasó counô maù bå changeä tï á differenô valuå  bù  reenterinç 
thå samå breakpoinô address¬ followinç iô witè á comma¬ anä theî specifyinç 
thå  ne÷ pasó count® Tï breaë oî á multé-bytå Z8° instructioî  thå  addresó 
specifieä  aó thå breakpoinô addresó musô bå thaô oæ thå firsô bytå oæ  thå 
instruction® Useró whï faiì tï observå thió rulå wilì generallù finä  theiò 
programó hoppinç thå nexô buó tï neveò-neveò land® Iæ á patcè ió madå aô aî 
addresó oæ á breakpoinô currentlù iî effecô bå surå thå breakpoinô  addresó 
ió stilì pointinç aô thå firsô bytå oæ thå ne÷ instruction® 

Multiplå breakpointó maù bå seô witè thå samå Â commanä bù separatinç  eacè 
onå  witè  á singlå space® Iæ multiplå breakpointó arå  specifieä  anä  Z8Å 
detectó  aî  erroneouó  argumenô  (á noî-existenô  symboì  foò  example©  á 
questioî  marë  wilì  bå printed¬ anä thå  commanä  terminates®  Alì  valiä 
breakpointó specifieä uğ tï thå invaliä onå wilì bå set® 

Z8Å  displayó á questioî marë wheî á attempô ió madå tï seô  á  seventeentè 
breakpoint.
.paŠ.tc     C    Clear Breakpoint .................................. #
Ã    Cleaò Breakpoint
________________________________________________________________ß


Thå Ã commanä clearó individuaì breakpointó previouslù seô bù á Â  command® 
Thå formaô oæ thå commanä is:


          *Ã  ARG± [ARG2...ARGnİ <cr¾

               wherå eacè arç maù bå anù valiä argumenô typå 
               whicè  evaluateó tï aî addresó previouslù seô 
               aó á breakpoinô


Multiplå  breakpointó  maù bå cleareä bù thå samå Ã commanä  bù  separatinç 
eacè argumenô witè á singlå space.

Z8Å  displayó  á  questioî marë wheî aî attempô ió madå  tï  cleaò  á  noî-
existenô breakpoint.

Tï cleaò ALÌ breakpointó enterº *Ã ª wherå thå asterisë indicateó ALL.
.paŠ.tc     D    Dump .............................................. #
Ä    Dump
________________________________________________________________ß


Thå Ä commanä allowó thå useò tï dumğ memorù iî botè hexadecimaì anä  ASCIÉ 
tï thå consolå iî useò specifieä blocë sizes® Thå formaô oæ thå commanä is:

          *Ä  [ARG1İ  [ARG2İ  <cr¾

               wherå    ARG± ½  thå startinç addresó tï dumğ
                        
                        ARG2 =  dictateó thå dumğ formaô  dependinç oî 
                                itó value® Iæ ARG²  ió iî thå rangå  ° 
                                ­  25µ theî iô  ió treateä aó á  blocë 
                                sizå  anä   representó thå  numbeò  oæ 
                                byteó  tï  bå displayeä (° ió  treateä 
                                aó  256)® Iæ ARG² ió greateò thaî  25µ  
                                theî  ARG²  ió treateä  aó  aî  endinç  
                                addresó  anä  memorù  wilì  bå  dumpeä  
                                noî-interactivelù tï thå console.

               ARG± anä ARG² maù bå oæ anù argumenô type®

Iæ ARG± ió omitteä theî thå dumğ resumeó froí thå lasô memorù address+±  aó 
displayeä  viá thå previouó invocatioî oæ thå Ä command® Iæ nï  previouó  Ä 
commanä haä beeî giveî theî memorù ió dumpeä startinç aô addresó 100H.

Iæ  ARG²  ió omitteä theî thå mosô recenô valuå oæ ARG² (froí  thå  lasô  Ä 
command©  ió  used®  Thå dumğ commanä displayó thå contentó  oæ  memorù  iî 
hexadecimaì  oî thå lefô sidå oæ thå consolå whilå thå ASCIÉ equivalenô  oæ 
eacè bytå ió showî oî thå righô side® 

Durinç  á blocë bù blocë dumğ (ARG² ¼ 25¶ signifieó á blocë bù blocë  dump© 
Z8Å  waitó foò useò inpuô afteò eacè blocë ió displayed® Á carriagå  returî 
entereä  bù  thå  useò  causeó thå commanä tï  terminatå  whilå  anù  otheò 
characteò causeó thå nexô sequentiaì blocë tï bå dumped.

Foò noî-interactivå dumps¬ startinç addresó tï endinç address¬ pressinç anù 
keù terminateó thå dump.

Thå dumğ commanä provideó aî especiallù easù waù oæ examininç tabulaò data¬ 
foò  examplå iî scanninç thå disë parameteò headeró iî youò BIOS® Thaô  is¬ 
bù specifyinç thå baså addresó aó ARG± anä thå tablå sizå aó ARG² thå  useò 
caî walë througè memory¬ tablå bù table.
.paŠ.tc     E    Examine Memory .................................... #
Å    Examinå Memory
________________________________________________________________ß


Thå Å commanä allowó thå useò tï examinå anä optionallù modifù thå contentó 
oæ memory® Thå formaô oæ thå commanä is:

     *Å   ARG±  <cr¾

               wherå  ARG± ió thå addresó oæ thå firsô bytå  tï 
               examinå

               ARG± maù bå anù symboì typå 

Upoî  receipô  oæ ARG± Z8Å wilì reaä thå contentó oæ thå  specifieä  memorù 
addresó anä displaù thå bytå iî botè heø anä ASCII® Aô thió poinô thå  useò 
haó  twï  options® Thå useò maù specifù replacemenô datá tï bå  writteî  tï 
memorù  startinç  aô thå currenô address¬ oò hå maù chooså tï  continuå  tï 
passivelù  examinå  memory® Thå choicå ió determineä  bù  thå  character(s© 
whicè arå inpuô afteò thå contentó oæ aî addresó arå displayed® 

Iæ thå useò wisheó tï modifù memorù startinç aô thå currenô memorù address¬ 
theî  aî ARGUMENÔ-STRINÇ maù bå entereä followinç thå displayeä  byte®  Z8Å 
wilì  evaluatå thå entirå strinç anä writå thå evaluateä equivalenô oæ  thå 
strinç  intï consecutivå memorù locationó startinç witè thå currenô  memorù 
address® Foò examplå thå useò coulä enteò thå followinç ARGUMENÔ-STRINGº 

     *Å 45F¹
     45F¹  4²  Â  'Thió ió á string',0D,0A,13,4F,9,'Morå Text',0µ
                  Ş                                             Ş  

                  Thå useò inpuô apprearó betweeî thå arrowó  anä 
                  woulä bå evaluateä tï thå followinç 3± bytesº 
                    
                  5´ 6¸ 6¹ 7³ 2° 4¹ 7³ 2° 6± 2° 7³ 7´ 7² 6¹ 6Å 6· 
                  0Ä 0Á 1³ 4Æ 0¹ 4Ä 6Æ 6² 6µ 2° 7´ 6µ 7¸ 7´ 0µ  

                  Theså  3±  byteó  woulä bå storeä  intï  memorù 
                  locationó  45F¹  tï 461· anä thå  nexô  addresó 
                  displayeä oî thå screeî woulä bå 4618®

     461¸  2³  £

Remembeò  thaô  ARGUMENÔ-STRINGÓ  maù bå terminateä bù  eitheò  á  carriagå 
returî  oò bù thå firsô spacå characteò whicè doeó noô appeaò iî  á  quoteä 
literaì string® Thå choicå oæ terminatoò determineó thå whicè addresó  wilì 
bå displayeä next® Iæ á carriagå returî ió useä tï terminatå thå  ARGUMENÔ-
STRING¬  theî  Z8Å  wilì displaù thå nexô sequentiaì  memorù  address®  Foò 
example:

          *Å 100²
          100²  4µ  Å  12,8F,0° <cr¾
          100µ  2¸  (

Thå  useò  entereä  aî ARGUMENÔ-STRINÇ 12,8F,0° whicè waó  evaluateä  tï  ³ 
bytes®  Sincå thå ARGUMENÔ-STRINÇ waó terminateä bù á carriagå  returî  thå 
nexô addresó displayeä waó 1002+³ oò 1005® 

Bù  terminatinç  thå ARGUMENÔ-STRINÇ witè á spacå thå useò caî  verifù  thå 
contentó  oæ memorù jusô modified®  ARGUMENÔ-STRINGÓ terminateä bù á  spacå 
causå  Z8Å  tï  redisplaù thå startinç address» thió makeó  thå  datá  jusô Šentereä availalbå foò reinspection:

          *Å 100²
          100²  4µ  Å  12,8F,0°  <space¾
          100²  1²  ş

Iæ thå useò doeó noô wanô tï writå anù datá tï thå currenô memorù  address¬ 
theî  thå characteò entereä shoulä bå á spacå character¬ uğ  arro÷  (caret© 
character¬ oò á carriagå return.

                  CHARACTEÒ                     ACTIOÎ
               
                    spacå          reaä nexô sequentiaì memorù addresó 
                    
                  uğ arro÷         reaä previouó memorù addresó 
                                             
                    <cr>           reaä nexô sequentiaì memorù addresó 
                                   commanä 

                   period          terminatå command

Thå  useò maù alsï changå thå currenô memorù addresó bù enterinç  aî  equaì 
sigî  "=¢ followeä bù á valiä argument® Thå addresó obtaineä bù  evaluatinç 
thió argumenô becomeó thå ne÷ currenô memorù addresó aó showî belowº 

     *Å 134´
     134´  8¹  ş  <cr¾
     134µ  6Æ  ï  <cr¾
     134¶  5²  Ò  =9F3´ <cr¾
     9F3´  6³  ã
.paŠ.tc     Æ    Find .............................................® #
Æ    Find
________________________________________________________________ß


Thå finä commanä allowó thå useò tï searcè memorù foò multé-bytå stringó iî 
memory® Thå formaô oæ thå commanä is:

     *Æ   ARG±  ARG² <cr¾

          where  ARG1  =  thå startinç addresó aô whicè tï begiî   thå 
                          search¬ iô maù bå oæ anù typå.
                 ARG2 =   ió   aî  ARGUMENÔ-STRINÇ  representinç   thå  
                          patterî tï searcè for» thå useò maù  specifù 
                          anù  combinatioî oæ argumentó  separateä  bù 
                          commaó  oò spaceó uğ tï thå  limiô oæ  Z8E'ó 
                          commanä linå buffeò  whicè ió 8° byteó long® 
                          Thå  actuaì   numbeò oæ byteó  searcheä  foò 
                          dependó  oî   ho÷ thå strinç  ió  ultimatelù 
                          evaluated.

                              
Z8Å  wilì  displaù  everù addresó whicè containó datá  matchinç  ARG2®  Thå 
searcè continueó untiì thå enä oæ memorù ió reached.

Thå  useò maù elecô tï canceì thå searcè aô anù timå bù depressinç anù  keù 
oî thå keyboard.

Iæ ARG² ió á singlå argumenô (aó opposeä tï aî argumenô string© anä iæ thió 
argumenô ió á symboì namå theî Z8Å wilì reverså thå ordeò oæ thå twï  byteó 
comprisinç  thå  1¶  biô operand® Mosô 1¶ biô valueó iî  Z8°  programó  arå 
storeä  witè  thå leasô significanô bytå aô á giveî addresó  anä  thå  mosô 
significanô  bytå aô thå giveî address+± (towarä thå higè enä  oæ  memory)® 
Thió  ió  iî  accordancå  witè  thå Z8°  conventioî  oæ  storinç  thå  mosô 
significanô bytå oæ á 1¶ biô argumenô towarä thå higè enä oæ memory.

Thå followinç arå exampleó oæ thå FINÄ commandº

     *Æ ° SYMBOÌ

     Assuminç  thaô  thå symboì "SYMBOL¢ ió associateä witè   thå  heø 
     valuå 3BÆ theî Z8Å woulä attempô tï finä alì  addresó  containinç 
     thå bytå paiò BÆ anä 0³ iî thaô  order¬ witè thå searcè beginninç 
     aô  addresó  0000®  Notå   thaô thå ordeò oæ  thå  twï  byteó  ió 
     reverseä becauså thå  symboì "SYMBOL¢ existó iî thå symboì table® 
     Tï  searcè   foò thå bytå paiò 0³ anä BÆ iî thaô ordeò  thå  useò  
     shoulä enteò thå argumenô aó eitheò á 1¶ biô heø numbeò  (3BF© oò 
     aó twï ¸ biô heø numberó (03,BF)® 


     *Æ 10° 87,32#,'ABCD',0C3,symbol,'p',271Æ

     Assuminç  thaô  thå symboì "symbol¢ ió associateä  witè  thå  heø 
     valuå 3BÆ theî Z8Å woulä attempô tï finä alì  startinç  addresseó 
     oæ thå followinç 1² bytå string:

             8· 2° 4± 4² 4³ 4´ C³ 0³ BÆ 7° 2· 1Æ

     Noticå  thaô Z8Å woulä searcè foò thå twï bytå patterî  0³ BÆ  aó 
     thå  valuå  foò "symbol"® Iæ thå useò happeneä  tï bå  tryinç  tï 
     finä thå instructioî JĞ symboì  thå searcè woulä faiì becauså  aó 
     mentioneä  abovå 1¶ biô  valueó arå storeä lo÷ ordeò bytå  first® Š     Thå useò  shoulä havå entereä C³ BÆ 03.

     Thå  twï  byteó whicè represenô thå addresó oæ  symboì  arå   noô 
     reverseä aó iî thå examplå abovå becauså ARG² ió  specifieä aó aî 
     ARGUMENÔ-STRINÇ aó opposeä tï á singlå  argument.

     Z8Å woulä begiî itó searcè aô addresó 10° (ARG1)® 
.paŠ.tc     G    Go ................................................ #
Ç    Go
________________________________________________________________ß


Thå  Ç  commanä  instructó Z8Å tï begiî oò resumå  executioî  oæ  thå  useò 
program® Thå formaô oæ thå commanä is:

          *Ç   [ARG±]  <cr¾

               wherå      ARG±   ½   thå  addresó  oæ  thå  firsô 
                                     instructioî thå useò wisheó
                                     tï execute®

               ARG±  maù bå anù argumenô type®   Iæ omitteä  theî 
               the current program counter value is assumed.


Upoî  receipô oæ thió commanä Z8Å initializeó alì breakpointó iî  thå  useò 
program¬  restoreó  alì useò registers¬ anä transferó controì tï  thå  useò 
prograí  undeò tesô aô thå addresó specifieä iî ARG1® Executioî withiî  thå 
useò prograí wilì continuå untiì thå useò prograí reacheó á breakpoint¬  aô 
whicè  poinô controì wilì returî tï Z8E® Thió ió thå onlù waù thå  useò  ió 
ablå tï returî controì tï Z8Å oncå thå GÏ commanä ió issued.

Z8Å breakpoinô techniquå haó beeî designeä sucè thaô Z8Å wilì noô  directlù 
initializå á breakpoinô aô thå addresó specifieä iî ARG1® Iî actualiitù  iô 
woulä  bå  impossiblå  tï dï sï sincå aî attempô woulä bå  madå  tï  resumå 
executioî  aô  thió  address¬  á breakpoinô woulä havå  beeî  seô  aô  thió 
address¬  anä controì woulä immediatelù returî tï thå monitoò withouô  thió 
instructioî  eveò havinç beeî executed®  Thió limitatioî haó beeî  overcomå 
iî Z8Å bù actuallù copyinç thå singlå instructioî locateä aô ARG± tï  Z8E'ó 
memory¬  THEÎ  settinç  thå breakpoinô aô thå  ARG±  address¬  anä  finallù 
executinç  thå  "moved¢ versioî oæ thå instructioî iî Z8E'ó  memorù  ratheò 
thaî  iî thå useò program® Z8Å compensateó foò thå thaô CALÌ  anä  RELATIVÅ 
JUMĞ  instructionó arå affecteä bù thå addresó aô whicè theù arå  executed® 
Thió entirå procesó ió totallù transparenô anä iô allowó thå useò tï  debuç 
loopó  bù  settinç  onlù á singlå breakpoinô withiî thå rangå  oæ  á  loop¬ 
obviateó thå neeä tï cleaò anù breakpointó whicè arå locateä aô thå addresó 
wherå  executioî  ió  tï resume¬ anä eveî allowó breakpointó aô  á  DJNÚ  ¤ 
instructions!


HINTº
     Wheî proceedinç froí á breakpoinô iô ió simplesô tï uså  thå 
     parameterlesó  forí oæ thå GÏ commandº  *Ç <cr¾ whicè  telló 
     Z8Å  tï  resumå  executioî aô thå addresó specifieä  bù  thå 
     user'ó currenô prograí counter® 
.paŠ.tc     H    Display Symbol Table .............................. #
È    Displaù Symboì Table
________________________________________________________________ß


Thå È commanä allowó thå useò tï vie÷ thå symboì tablå oî thå console®  Thå 
formaô oæ thå commanä is:

          *È   [ARG1İ  <cr¾

               wherå ARG± musô bå á symboì namå


Iæ  ARG± ió omitteä Z8Å wilì displaù thå entirå symboì tablå startinç  witè 
thå  firsô  symboì  iî thå table® Iæ ARG± ió presenô  Z8Å  wilì  begiî  thå 
displaù witè thaô symbol® Z8Å displayó á blocë oæ 3² symboló theî waitó foò 
useò  input®  Iæ  thå  useò  enteró á  carriagå  returî  theî  thå  commanä 
terminates®  Iæ  thå useò enteró anù otheò characteò thå nexô blocë  oæ  3² 
symboló ió displayed.

Iæ  á symboì namå entereä aó ARG± cannoô bå founä iî thå symboì  tablå  Z8Å 
printó á questioî mark.
.paŠ.tc     I    Initialise FCB and command tail ................... #
É    Initialiså FCÂ anä commanä tail
________________________________________________________________ß


Thå É commanä initializeó thå commanä taiì aô 80è anä thå filå namå  fieldó 
iî thå defaulô FCÂ (5Cè anä 6Ch).

          *É  [ARG1İ [ARG2İ ..® [ARGnİ <cr¾

               wherå eacè arç maù bå anù string®

Thå commanä taiì iî thå defaulô DMÁ buffeò aô 80è wilì bå filleä iî witè  á 
standarä  formaô  commanä buffeò comprisinç á lengtè bytå followeä  bù  thå 
strinç formeä bù translatinç alì thå ARGó tï uppeò case® Á zerï ió appendeä 
afteò thå lasô ARG.

Thå  firsô twï ARGó arå treateä aó (possiblù ambiguous© filå nameó anä  arå 
copieä  witè appropriatå paddinç intï thå filå namå fieldó iî  thå  defaulô 
FCÂ area.

Iæ nï ARG² ió presenô theî thå filå namå fielä aô 6Cè wilì bå  blanë-filleä 
froí 6Dè tï 77h® Iæ nï ARG± ió presenô theî thå filå namå fielä aô 5Cè wilì 
bå blanë-filleä froí 5Dè tï 67h.

Sincå  thå "I¢ commanä witè nï argumentó ió á valiä command¬ iô  cannoô  bå 
aborted.

Notå  thaô  Z8Å doeó noô automaticallù inserô á spacå aô thå  heaä  oæ  thå 
commanä  tail® Iæ yoõ arå debugginç routineó tï parså thå commanä linå  yoõ 
shoulä  inserô á taâ oò spacå yourselæ tï makå thå commanä taiì  realistic® 
Foò example¬ thå Z8Å command:

                 é   square.peç

generateó thå commanä tailº

       0Á 5³ 5± 5µ 4± 5² 4µ 2± 5° 4µ 4· 0°
       1°  Ó  Ñ  Õ  Á  Ò  Å  ®  Ğ  Å  Ç 0°

whereaó thå commanä taiì tï prograí 'fit§ generateä aô thå CP/Í leveì by:

                 fiô square.peç

woulä beº

       0Â 2° 5³ 5± 5µ 4± 5² 4µ 2± 5° 4µ 4· 0°
       1±     Ó  Ñ  Õ  Á  Ò  Å  ®  Ğ  Å  Ç 0°

anä  yoõ caî emulatå thió forí bù puttinç thå spacå iî yourselæ beforå  thå 
worä 'square'.
.paŠ.tc     J    Animated Full Screen Debugger ..................... #
Ê    Animateä Fulì Screeî Debuggeò
_______________________________________________________________ß

Thå Ê commanä provideó thå useò witè thå abilitù tï "see¢ insidå thå Z8° aó 
iô  executeó á program® Thå Z8Å animateä debuggeò allowó thå useò  tï  vie÷ 
registers¬  memory¬  anä  instructionó  whilå  thå  Z8°  ió  simultaneouslù 
executinç code® Iî addtioî thå Ê commanä provideó thå useò witè thå abilitù 
tï  interactivelù  singlå-steğ  througè á prograí  usinç  thå  fulì  screeî 
facilitieó oæ thå command® Thå formaô oæ thå Ê commanä is:

     *Ê   [/İ [*İ [ARG1İ [ARG2İ
or   *J   #

          USÅ  OÆ THÅ Ê COMMANÄ FOÒ SINGLÅ STEPPINÇ IÓ   DESCRIBEÄ  AÔ 
          THÅ  ENÄ OÆ THIÓ SECTION® THIÓ  SECTIOÎ DESCRIBEÓ  THÅ  NOÎ-
          INTERACTIVÅ VERSIOÎ OÆ  THÅ Ê COMMANÄ DURINÇ WHICÈ THÅ  USEÒ 
          TURNÓ OVEÒ  COMPLETÅ CONTROÌ OÆ THÅ EXECUTIOÎ OÆ THÅ PROGRAÍ  
          UNDEÒ TESÔ TÏ Z8E® 

          ARG±  ió thå startinç addresó oæ thå displaù anä  maù bå  oæ 
          anù valiä argumenô type® Foò example¬  thå useò maù  specifù 
          *Ê  Ğ  tï  resumå  executioî  aô  thå  poinô  wherå  iô  haä 
          previoulù beeî stopped® 

          Thå slasè anä staò controì subroutinå tracinç aó  follows:

               "/¢  Slasè  informó  Z8Å noô tï tracå anù  subrou­
                    tineó aô all®
         
               "*¢  Asterisë informó Z8Å noô tï tracå anù subrou­
                    tinå  calló tï addresseó locateä iî thå rangå 
                    ° tï FF®  Thió featurå ió intendeä tï  screeî 
                    ouô  calló  tï locatioî µ (BDOS© iî ordeò  tï 
                    prevenô  Z8E'ó  anä  thå  user'ó  stacë  froí 
                    becominç hopelesslù entangled®
               
          ARG² representó aî optionaì timeouô paramteò whicè   affectó 
          thå  speeä aô whicè instructionó arå executed®  Thió  numbeò 
          maù  bå  iî  thå rangå ° ­ 255¬  witè 1°  (decimal©  aó  thå 
          defaulô  iæ  nï  valuå ió  entered® Á timeouô  valuå  oæ  1° 
          yieldó  approximatelù  á onå halæ seconä delaù  betweeî  thå 
          executioî    oæ  sequentiaì  instructions®  Á  valuå  oæ   ° 
          representó  NÏ  timå delaù anä ió iî actualitù  thå  fastesô 
          ratå á whicè thå Ê commanä caî run.

Oncå thå Ê commanä commences¬ Z8Å takeó oveò thå Z8° anä furnisheó thå useò 
witè  á "peephole¢ intï thå CPU® Z8Å executeó onå instructioî aô á timå  iî 
thå  useò prograí pausinç afteò eacè onå tï dynamicallù updatå  thå  screeî 
display®  Thå Ê commanä divideó thå screeî intï threå areasº registeò  map¬ 
disassembleä  code¬  anä  memorù window®  Thå  registeò  mağ  displayó  alì 
registeró oî thå toğ twï lineó oæ thå screeî alonç witè thå contentó oæ thå 
Æ  registeò  whicè  ió showî iî mnemoniã form®  Z8Å  alsï  disassembleó  1¸ 
instructionó baseä oî thå currenô PÃ valuå anä displayó theí oî thå screen» 
finally¬  usinç  thå parameteró entereä iî thå × command¬ Z8Å  snapshotó  á 
blocë oæ memorù anä displayó iô aó á windo÷ oî thå screen® 

Executioî  oæ thå useò prograí continueó untiì anù noî-numeriã keù  oî  thå 
keyboarä  ió presseä whicè endó thå command® Iæ á numeriã keù  ió  pressed¬ 
theî  Z8Å respondó bù changinç thå timeouô parameteò oî thå fly®  Thå  useò 
maù  uså thå keyó ° ­ ¹ aó á throttlå tï goverî thå executioî  speed®  Zerï 
beinç thå fastest» ninå beinç thå slowest.Š
Thå commanä alsï terminateó wheneveò á useò defineä breakpoinô ió  reached® 
Thaô  is¬  iæ  thå useò haä seô á breakpoinô viá thå  Â  commanä  anä  thió 
addresó ió reacheä thå Ê commanä endó anä Z8Å promptó thå useò foò thå nexô 
command®  Iæ thå breakpoinô haä á pasó counô associateä witè it¬  thå  pasó 
counô musô reacè zerï beforå thå Ê commanä wilì terminate.

USINÇ THÅ Ê COMMANÄ FOÒ SINGLÅ STEPPINÇ

Z8Å  permitó  thå useò tï singlå-steğ througè á prograí  whilå  allowinç  á 
continuouó fulì-screeî vie÷ oæ thå registers¬ codå beinç executed¬ anä  thå 
contentó  oæ á blocë oæ memorù aó specifieä bù thå Ë command® Iî  ordeò  tï 
invokå thå fulì screeî singlå-steğ thå useò enteró thå followinç command:

     *Ê   [/İ  [*İ 

          ¯ instructó Z8Å noô tï tracå anù subroutineó  aô all

          ª instructó Z8Å noô tï tracå anù subroutineó  locatioî belo÷ 
          addresó 100È anä ió  specificallù designeä tï allo÷ thå useò 
          thå  optioî oæ noô becominç tangleä iî BDOÓ anä  BIOS.

          Notå  thaô thió versioî oæ thå Ê commanä ió   differentiateä 
          froí  thå  noî-interactivå  versioî bù thå  absencå  oæ  anù 
          argumenô indicatinç án executioî address® 

Thió versioî allowó thå useò tï executå onå instructioî iî hió prograí  anä 
theî  regaiî  controì  aô  thå Z8Å commanä  level®  Z8Å  wilì  executå  thå 
instructioî  pointeä tï bù thå user'ó currenô PC® Afteò thå instructioî  ió 
executeä aî ARRO× ¨ =¾ © pointó tï thå nexô instructioî tï bå executed.

Thå  ¯ anä ª optionó arå onlù valiä iæ thå nexô instructioî tï bå  executeä 
ió á CALL® Iæ thå prograí counteò ió pointinç aô anù otheò instructioî theî 
thå ¯ anä ª havå nï immediatå effect®  Howeveò thå meaninç ió remembereä sï 
thaô wheî á CALÌ instructioî ió encountereä thå appropriatå actioî (oò noî-
action© wilì bå taken.

Afteò  á Ê commanä haó beeî issueä pressinç <cr¾ ió equivalenô  tï  issuinç 
anotheò Ê command®  Thió allowó yoõ tï manuallù steğ througè á prograí verù 
quickly.

REFRESHING THE SCREEN

Z8E'ó  orderlù displaù caî bå disturbeä bù consolå outpuô generateä bù  thå 
prograí beinç debugged®  Iæ thaô happenó theî uså commanä forí *Ê  £     tï 
refresè thå screen®  Nï otheò argumenô ió permitted.
.paŠ.tc     K    Set Memory Window Parameters ...................... #
Ë    Seô Memorù Windo÷ Parameteró foò uså Witè thå Ê command
_______________________________________________________________ß

Thå Ë commanä setó thå startinç addresó anä blocë sizå oæ thå memorù windo÷ 
displaù durinç thå Ê command® Thå formaô oæ thå commanä is:

          *Ë  ARG± [,ARG2İ

          ARG±  representó  thå startinç addresó  oæ  thå  memorù 
          block®

          ARG² ió aî optionaì sizå paramter¬ iæ omitteä thå blocë 
          sizå defaultó tï thå maximum®

Thå maximuí blocë sizå ió 14´ decimaì whicè ió 9° hex® Thå startinç addresó 
oæ  thå  memorù  blocë caî bå anywherå iî memory» iô doeó noô  havå  tï  bå 
withiî thå confineó oæ thå useò program.
.paŠ.tc     L    Load file ......................................... #
Ì    Loaä file
________________________________________________________________ß


Thå  Ì commanä allowó thå useò tï loaä fileó intï thå TPÁ afteò  thå  debuç 
sessioî haó started® Thå formaô oæ thå commanä is:

     *Ì   ARG1[,ARG2İ  <cr¾

               ARG±  ió á singlå unambiguouó filå namå  conforminç  tï 
               standarä  CP/Í  syntaø rulesº

            -  optionaì  drivå namå followeä bù á coloî   ­  mandatorù 
               primarù filå name

            -  optionaì secondarù filå namå precedeä bù á  perioä 

               ARG²  ió  aî  optionaì loaä address®  Iæ  ARG²  ió  noô  
               specifieä thå nameä filå ió loadeä aô thå starô oæ  thå 
               TPÁ (addresó 10° hex)® Iæ ARG² ió giveî thå  filå  wilì 
               bå  loadeä  aô  thió address® Z8Å  wilì  NOÔ   relocatå 
               individuaì  addresseó withiî thå filå tï   reflecô  thå 
               ne÷ loaä address® ARG² maù bå oæ anù  type.

               NOTEº  Iæ  nï argumentó arå entereä theî Z8Å  wilì 
                      redisplaù thå startinç address¬  endinç ad­
                      dress¬  anä thå numbeò oæ 25¶ bytå pageó oæ 
                      thå lasô filå loaded®  


Iæ  Z8Å detectó á erroò iî thå filå namå specificatioî thå messagå  "Syntaø 
error¢ ió printeä oî thå consolå anä thå commanä terminates.

Iæ  Z8Å  ió unablå tï locatå thå filå oî thå specifieä  drivå  thå  messagå 
"Filå noô found¢ ió printeä oî thå consolå anä thå commanä terminates.

Z8Å  containó  nï faciltieó foò convertinç .HEØ (Inteì Heø  format©  objecô 
fileó  tï loadablå memorù image® Alì files¬ regardlesó oæ type¬ arå  loadeä 
intï  memorù  iî exactlù thå samå forí aó theù appeaò oî disk® Tï  debuç  á 
.HEØ  filå thå useò shoulä firsô loaä thå filå witè thå CP/Í  LOAÄ  commanä 
anä  savå  thå filå witè thå CP/Í SAVÅ commanä whicè produceó  aî  absolutå 
memorù  imagå  loaä-ablå bù Z8E® Alì .COÍ fileó arå oæ  courså  alreadù  iî 
loadablå forí anä nï LOADinç anä SAVEinç ió required.

Iæ thå filå wilì noô fiô intï thå TPA¬ Z8Å wilì prinô thå message:

     Ouô oæ memorù ­ Continue¿

Iæ  thå useò answeró "Y"¬ Z8Å wilì resumå loadinç thå filå aô  addresó  10° 
heø  iæ ARG² waó noô entered¬ oò aô thå addresó specifieä aó ARG2®  Iæ  thå 
useò  typeó  anù  otheò response¬ thå loadinç procesó  terminateó  anä  Z8Å 
returnó tï thå commanä level® However¬ thå useò maù resumå loadinç thå filå 
aô  á lateò timå bù issuinç thå Ì commanä anä specifyinç thå filå namå  ".¢ 
(á  singlå  period)®  Thå useò maù chooså tï specifù á  ne÷  startinç  loaä 
addresó  followinç  thå period» iæ ARG² ió omitteä theî  thå  loaä  addresó 
defaultó  bacë tï 10° hex¬ thå starô oæ thå TPA® Iæ thå useò haó  donå  anù 
subsequenô  disë I/Ï (sucè aó loadinç á ne÷ filå oæ disassemblinç tï  disk© 
iî  betweeî  thå timå loadinç waó suspendeä anä theî  restarted¬  Z8Å  wilì 
treaô thå filå namå ".¢ aó á syntaø error® 

Thå  useò maù occasionallù neeä tï overlaù á sectioî oæ codå iî  á  prograí 
whicè alreadù resideó iî memorù witè inpuô froí á filå oî disk¬ foò examplå Šiî modifyinç á BIOÓ iî preparatioî foò MOVCPM® Whilå thió ió possiblå  witè 
loaderó  whicè procesó .HEØ objecô files¬ iô ió noô feasiblå witè Z8E®  Thå 
useò  caî circumvenô thió limitatioî bù loadinç thå filå froí disë intï  aî 
unuseä sectioî oæ memorù anä theî usinç Z8E'ó movå commanä tï movå onlù thå 
datá needeä tï accomplisè thå overlay.
.paŠ.tc     M    Move Memory ....................................... #
Í    Movå Memory
________________________________________________________________ß


Thå  Í commanä allowó thå useò tï movå blockó oæ datå froí anù  addresó  iî 
memorù tï anù otheò addresó iî memory® Thå formaô oæ thå commanä is:

          *Í   ARG±  ARG²  ARG³  

               wherå   ARG±  ½  thå   startinç  addresó  oæ   thå 
                                sourcå datá blocë        
                       
                       ARG²  ½  thå  endinç addresó oæ thå sourcå 
                                datá blocë

                       ARG³  ½  thå   startinç  addresó  oæ   thå 
                                destinatioî datá blocë   


                                argumentó maù bå oæ anù typå

Z8Å  automaticallù decideó whetheò á heaä-tï-heaä oò taiì-tï-taiì  movå  ió 
requireä  baseä oî thå threå argumentó entered® Iæ á heaä-tï-heaä  movå  ió 
needeä theî thå firsô bytå oæ thå sourcå datá blocë wilì bå writteî tï  thå 
firsô  bytå positioî oæ thå destinatioî datá block» thå seconä bytå oæ  thå 
sourcå  datá  blocë  wilì bå writteî tï thå seconä  bytå  positioî  oæ  thå 
destinatioî  datá  block¬  anä  sï  oî untiì  thå  endinç  addresó  oæ  thå 
destinatioî datá blocë ió reached® 

Oî  thå otheò hand¬ iæ á taiì-tï-taiì movå ió necessarù Z8Å wilì  movå  thå 
lasô  bytå  oæ  thå  sourcå datá blocë tï thå lasô  bytå  positioî  oæ  thå 
destinatioî  datá block¬ followeä bù thå seconä tï lasô bytå oæ thå  sourcå 
datá  blocë  tï thå seconä tï lasô bytå positioî oæ  thå  destinatioî  datá 
block¬  anä  sï oî untiì thå startinç addresó oæ thå destinatioî  blocë  ió 
reached® 

Á taiì tï taiì movå woulä bå necessarù iî thå followinç examplå tï  prevenô 
thå overwritinç oæ thå destinatioî datá block:

          *Í  100° 100Æ 100¸
.paŠ.tc     N    Output to I/O Ports without pre-read .............. #
Î    Outpuô tï I/Ï Portó rithouô pre-read
________________________________________________________________ß

Thió  commanä allowó thå useò tï outpuô datá tï aî I/Ï porô  withouô  firsô 
readinç  thå porô (aó occuró iî thå Ñ command)® Thå formaô oæ  thå  commanä 
is:

     *Î   [ARG1İ

          wherå  ARG±  ió thå porô numbeò tï whicè thå  datá  wilì  bå 
          written.

          Iæ  ARG±  ió omitteä theî Z8Å useó thå   lasô  porô  addresó 
          whicè haä beeî  inpuô bù á previouó Î oò Ñ command® 

Z8Å wilì prompô thå useò bù displayinç thå currenô porô numbeò oî thå  lefô 
hanä  sidå  oæ  thå consolå anä postioninç thå cursoò twï  spaceó  thå  thå 
right® Aô thió poinô thå useò caî enteò thå datá tï bå senô tï thå porô  iî 
thå forí oæ aî ARGUMENÔ-STRING® Thå ARGUMENÔ-STRINÇ allowó thå useò tï  miø 
variouó  argumenô  typeó  sucè aó heø datá anä ASCIÉ  literaì  strings®  Oæ 
courså  thå useò caî elecô tï merelù outpuô singlå byteó iæ desired® Thå  Î 
commanä  ió  particularlù usefuì wheî programminç  variouó  Z8°  peripheraì 
chipó  sucè aó thå DMÁ anä SIÏ chipó whicè expecô initializatioî  byteó  tï 
arrivå iî á streaí withouô interveninç reads.

          *Î 8°
          8°  'T',00,12#,99¸

          Thió  ARGUMENÔ-STRINÇ woulä bå evaluateä intï thå  µ  bytesº 
          5´  0° 0Ã 0¹ 98® Theså fivå byteó woulä  bå senô tï porô  8° 
          viá aî OTIÒ instruction® Nï  delaù occuró betweeî successivå 
          bytes® 

Afteò thå datá haó beeî entereä anä afteò iô haó beeî senô tï thå I/Ï  porô 
Z8Å  repromptó thå useò bù displayinç thå samå porô number® Thió giveó  thå 
useò  tï oppportunitù tï senä addtionaì datá tï thå samå port® However¬  bù 
noô enterinç datá thå useò caî changå thå currenô porô addresó bù  enterinç 
anù oæ thå following:

        CARRIAGÅ    Thå nexô sequentiaì porô numbeò iî  ascendinç 
         RETURÎ     ordeò becomeó thå currenô porô address®

        UĞ ARRO×    Thå nexô sequentiaì porô numbeò iî descendinç 
                    ordeò becomeó thå currenô porô address®

         =ARG       Anù argumenô appearinç immediatelù afteò  thå  
                    equaì   sigî  (nï  interveninç   spaces©   ió 
                    evaluateä aó aî ¸ biô number¬ anä iæ founä tï 
                    bå   valiä  theî iô becomeó thå  ne÷  currenô 
                    porô  address® 

        PERIOÄ      Terminatå commanä

Thå  useò caî alsï monitoò aî I/Ï porô witè thå Î commanä bù enclosinç  thå 
porô  numbeò  oî thå commanä linå iî parentheses®  Monitoò modå viá  thå  Î 
commanä ió identicaì tï thaô oæ thå Ñ commanä (seå Ñ command).

Iî Z18° modå thå Î anä Ñ commandó supporô accesó tï thå HD64180'ó internaì I/Ï 
registers.

Wheî  writinç tï á Z180/HD6418° port¬ Z8Å firsô clearó thå Â register¬  placeó Šthå porô numbeò iî thå Ã register¬ pointó HÌ tï á buffeò internaì tï Z8Å  coî
taininç thå datá tï bå writteî anä theî executeó aî OUTÉ instruction® 

Thió methoä ensureó thaô A¸ tï A1µ arå aô ° wheî thå Î commanä ió accessinç aî 
I/Ï port® 
.paŠ.tc     O    Output Current Breakpoints to Console ............. #
O    Outpuô Currenô Breakpointó tï Console
________________________________________________________________ß


Thå Ï commanä allowó thå useò tï vie÷ alì breakpointó currentlù iî  effect® 
Thå formaô oæ thå commanä is:

          *Ï  

               nï argumentó arå requireä

Iæ  Z8Å findó á symboì namå correspondinç tï thå absolutå heø addresó oæ  á 
breakpoinô addresó iî thå symboì tablå (iæ á symboì tablå exists© theî  thå 
symboì  namå  aó  welì aó thå memorù addresó ió  displayed®  Iæ  nï  symboì 
correspondinç tï thå addresó ió founä onlù thå heø addresó ió displayed® 

Iæ  anù pasó countó arå currentlù iî effecô theù arå displayeä nexô tï  thå 
breakpoinô addresó witè whicè theù arå associated®
.paŠ.tc     P    Examine/Modify PSW (Flags Register) ............... #
Ğ    Examine/Modifù PS× (Flaç Register)
________________________________________________________________ß


Thå  Ğ  commanä provideó á convenienô methoä oæ  examininç  anä  optionallù 
modifyinç  thå F(lag© registeò iî thå useò registeò set® Thå formaô oæ  thå 
commanä is:

     *Ğ

          nï argumentó arå requireä oî thå commanä linå

Upoî  receipô oæ thå Ğ commanä Z8Å displayó thå mnemonicó correspondinç  tï 
thå  currenô  statå oæ thå fouò useò-modifiablå bitó  (sign¬  carry¬  zero¬ 
parity© iî Flaç register:


     MNEMONIÃ            MEANINÇ             BIÔ STATUÓ

        Ğ                positivå               reseô
        Í                 minuó                  seô

        NÃ               nï carrù               reseô
        Ã                 carrù                  seô

        PÏ              paritù odä              reseô
        PÅ              paritù eveî              seô

        NÚ               noô zerï               reseô
        Ú                  zerï                  seô 


Z8Å  printó thå mnemoniã correspondinç tï thå currenô statå oæ eacè oæ  thå 
fouò flaç bits® Z8Å theî issueó á carriagå return/linå feeä anä pauseó  foò 
useò  input®  Thå useò maù modifù anù oæ thå fouò flaç bitó bù  typinç  thå 
appropriatå  mnemoniã  followeä bù á carriagå return® Thå  useò  maù  enteò 
multiplå mnemonicó bù separatinç eacè onå witè á space® 

Iæ  nï  mnemonicó arå entered¬ nï flagó bitó arå altereä  anä  thå  commanä 
terminates.

Iæ aî invaliä flaç biô mnemoniã ió entereä Z8Å printó á questioî mark®
.paŠ.tc     Q    Query I/O Ports ................................... #
Ñ    Querù I/Ï Ports
________________________________________________________________ß

Thå Ñ commanä allowó thå useò flexiblå accesó tï I/Ï portó bù providinç thå 
abilitù tï perforí singlå bytå input¬ continuouó inpuô (monitoò mode)¬  anä 
singlå oò multé-bytå outpuô followinç á prå-reaä oæ thå port® Thå formaô oæ 
thå commanä isº

     *Ñ [(İ [ARG1İ [)]

          wherå ARG± ió aî ¸ biô porô addresó iî thå rangå ° ­ 255

          ARG±  maù  bå  anù  symboì type¬  howeveò iæ á 1¶  biô  valuå  ió 
          specifieä onlù thå lo÷ ordeò bytå ió  significanô 

          Iæ nï argumenô ió giveî Z8Å wilì uså thå mosô  recenô porô numbeò 
          aó entereä bù thå useò viá aî Î  oò Ñ command.

          Iæ ARG± ió encloseä iî parentheseó Z8Å wilì enteò  MONITOÒ MODE.

Upoî  receipô oæ ARG± Z8Å wilì reaä thå specifieä I/Ï porô anä displaù  thå 
bytå  reaä  aó  botè ¸ biô hexadecimaì valuå  anä  it'ó  ASCIÉ  equivalent® 
Commanä optionó oncå á bytå haó beeî reaä froí thå I/Ï porô arå aó follows:

     SINGLÅ BYTÅ INPUÔ

          Bù  enterinç  á SPACÅ immediatelù followinç  thå   displayeä 
          contentó  oæ  thå  I/Ï porô thå useò caî   instrucô  Z8Å  tï 
          continuå readinç froí thå samå  I/Ï port:

.cp 3
                       *Ñ EÅ
                       EÅ   2´  ¤  <space¾
                       EÅ   2´  ¤

.cp 3   
          Bù  enterinç  á  CARRIAGÅ  RETURÎ  followinç  thå  displayeä 
          contentó oæ thå I/Ï porô thå useò caî  instrucô Z8Å tï  reaä 
          thå nexô porô numbeò  (ascendinç order):

.cp 3
                       *Ñ EÅ
                       EÅ   2´  ¤  <cr¾
                       EÆ   C±  Á

.cp 3
          Bù enterinç á careô "^¢ followinç thå displayeä  contentó oæ 
          thå I/Ï porô thå useò caî instrucô  Z8Å tï reaä thå previouó 
          porô numbeò (descendinç  order):

.cp 3
                       *Ñ EÅ
                       EÅ   2´  ¤  Ş  (uğ arro÷ entereä bù user©
                       EÄ   0¶  ş                       

.cp 2
          Bù enterinç aî equaì sigî "=¢ followeä bù á valiä  argument¬ 
          thå useò caî switcè tï readinç á ne÷  porô address:

.cp 3
                       *Ñ EÅ
                       EÅ   2´  '$§  =9° 
                       9°   BÆ  '?§             


     CONTINUOUÓ INPUÔ (MONITOÒ MODE©

          Z8Å provideó thå useò witè thå abilitù tï monitoò  aî  inpuô Š          port® Z8Å wilì continouslù reaä thå  selecteä inpuô porô anä 
          displaù  thå contentó oî  thå screen® Z8Å displayó thå  bytå 
          iî  botè  heø   anä binary® Thió featurå ió  usefuì  iî  thå  
          testinç  oæ I/Ï ports® Depressinç anù keù oî  thå   keyboarä 
          exitó monitoò mode® 

     MULTI-BYTÅ OUTPUÔ 

          Followinç  thå  reaä oæ aî I/Ï porô thå useò maù   elecô  tï 
          outpuô  data® Thå useò maù enteò aî   ARGUMENÔ-STRINÇ  whicè 
          wilì  bå senô tï thå porô oî  á bytå bù bytå basió  witè  nï 
          interveninç readó  betweeî outputó aó showî below:

                *Ñ 5°
                5°  4´  'D§ 23,9,'B2E',00,F723,8±
                            Ş                   Ş
                            strinç  aó entereä bù 
                            useò appearó  betweeî 
                            arrowó

          Thå datá aó entereä bù thå useò iî thió examplå  woulä firsô 
          bå converteä tï thå ¹ byteó showî  belowº

               2³ 0¹ 4² 3² 4µ 0° F· 2³ 81

          Theså ¹ byteó woulä theî bå senô tï porô 5° onå  bytå  afteò 
          anotheò withouô anù interveninç readó  oò statuó checks®

Wheî  iî Z180/HD6418° modå Z8E'ó Î anä Ñ commandó perforí jusô aó theù  dï  iî 
Z8° modå excepô thaô theù supporô accesó tï thå HD64180'ó internaì I/Ï  regis
ters.

Wheî  readinç  aî I/Ï port¬ Z8Å firsô clearó thå Â register¬ placeó  thå  porô 
numbeò iî thå Ã registeò anä theî executeó aî IÎ A,(C© instruction® 

Wheî writinç tï aî I/Ï port¬ Z8Å firsô clearó thå Â register¬ placeó thå  porô 
numbeò iî thå Ã register¬ pointó HÌ tï á buffeò internaì tï 18Å coîtaininç thå 
datá tï bå writteî anä theî executeó aî OUTÉ instruction® 

Thió methoä ensureó thaô A¸ tï A1µ arå helä aô ° wheî thå Î oò Ñ commandó  arå 
accessinç aî I/Ï port® 
.paŠ.tc     R    Examine/Modify Register Contents .................. #
Ò    Examine/Modifù Registeò Contents
________________________________________________________________ß


Thå  Ò commanä allowó thå useò tï examinå anä optionallù  modifù  registeró 
anä registeò pairó iî thå useò registeò set® Thå formaô oæ thå commanä is:

          *Ò   ARG±  <cr¾ oò spacå

               wherå  ARG±  ió anù oæ thå 2²  registeò  mnemonicó 
               listeä belowº

                     Á    Â    Ã    Ä    Å    È    Ì
                     AÆ   BÃ   DÅ   HÌ   IØ   IÙ   SĞ
                     AF§  BC§  DE§  HL§  É    Ò    Ğ    PÃ

                    (thå  prograí  counteò maù  bå  specifieä  aó 
                    eitheò Ğ oò PC©

Tï  examinå  á  registeò thå useò enteró á mnemoniã  froí  thå  abovå  lisô 
followeä  bù  á carriagå returî oò á space® Z8Å wilì  displaù  thå  currenô 
contentó  oæ thå registeò oî thå samå line® Aô thió poinô thå useò haó  thå 
optioî oæ enterinç aî argumenô oæ anù typå iæ thå contentó oæ thå  registeò 
oò registeò paiò arå tï bå changed® Thå replacemenô valuå maù bå terminateä 
bù eitheò á carriagå returî oò á space® Iæ nï valuå ió entereä Z8Å issueó á 
carriagå  return/linå feeä anä waitó foò thå nexô registeò mnemïniã tï  bå 
entered.

Iæ  thå  useò  specifieó á 1¶ biô valuå aó thå ne÷ contentó  oæ  aî  ¸  biô 
registeò onlù thå lo÷ ordeò bytå oæ thå valuå ió used.

Thå commanä terminateó wheî á carriagå returî oò spacå ió entereä wheî  Z8Å 
ió waitinç foò á registeò mnemonic.
.paŠ.tc     Ó    Singlå Step ......................................® #
Ó    Singlå Step
________________________________________________________________ß

                              
Thå  Ó  commanä  allowó  thå  useò tï  executå  á  prograí  instructioî  bù 
instruction®  Thå Ó commanä provideó foò fulì tracinç oæ thå useò  program® 
Thå formaô oæ thå commanä is:

     *Ó   [/İ [ARG1İ  <cr¾
               
          wherå  ARG± ió thå numbeò oæ instructionó tï executå iî  thå 
          useò  program¬  iæ nï argumenô ió giveî  Z8Å defaultó  tï  ±  
          ARG± maù bå oæ anù type

Thå slasè "/¢ allowó thå useò controì oveò thå tracinç oæ subroutines® Iæ á 
slasè ió includeä beforå thå counô (iæ á counô ió entered)¬ oò iæ thå slasè 
ió  thå  onlù characteò oî thå commanä linå theî subroutineó  wilì  noô  bå 
traced®  Á slasè affectó onlù CALÌ instructionó whicè liå withiî thå  rangå 
oæ  ARG1® Iî thå mosô typicaì caså nï ARG± ió presenô anä thå  singlå  steğ 
counô  defaultó tï 1® Iæ thå currenô PC¬ 100° iî thió example¬ ió  pointinç 
tï á calì instructioî theî thå command:

                    *Ó ¯

          100°  CÄ 5¶ 3°  RASRTNº CALÌ  ANYSUÂ
          100³  FÅ 0´             CĞ    ´
          100µ  CÁ 1· 1°          JĞ    Z,AHEAÄ


wilì  causå  thå entirå subroutinå ANYSUÂ tï bå executeä anä  controì  wilì 
returî tï thå useò aô addresó 1003® 

Iæ  ARG± ió omitteä Z8Å wilì transfeò controì tï thå useò prograí  anä  onå 
instruction¬  thå  onå  pointeä tï bù thå currenô contentó  oæ  thå  user'ó 
prograí  counter¬  wilì  bå  executed®  Followinç  thå  executioî  oæ   thå 
instructioî  (oò  grouğ  oæ instructionó iæ ARG² waó greateò  thaî  1©  Z8Å 
regainó controì anä automaticallù displayó thå currenô contentó oæ alì  thå 
useò registers.

Thå  useò maù optionallù indicatå thaô morå thaî onå instructioî ió  tï  bå 
executeä  bù  enterinç á valuå greateò thaî ± foò ARG1® Z8Å  wilì  transfeò 
controì  tï  thå useò prograí anä regaiî controì onlù  wheî  thå  specifieä 
numbeò  oæ  instructionó  havå beeî executed® Thió  featurå  ió  usefuì  iî 
debugginç smalì loops» iî thaô thå useò caî seô ARG± equaì tï thå numbeò oæ 
instructionó  iî  thå  rangå oæ thå loop® Z8Å  wilì  displaù  thå  registeò 
contentó afteò eacè instructioî oæ thå looğ ió executeä anä returî  controì 
tï thå useò afteò everù iteratioî oæ thå loop.

Thå  singlå  steğ commanä alwayó causeó thå executioî  oæ  thå  instructioî 
pointeä  tï bù thå currenô contentó oæ thå user'ó prograí counter® Thió  ió 
thå instructioî thaô appearó iî dióassembleä forí aó parô oæ thå outpuô  oæ 
thå "X¢ commanä (displaù machinå state)® Beaò iî minä thaô ARG± ió noô  thå 
addresó  aô whicè singlå steppinç ió tï begin» iô ió á counô tï thå  numbeò 
oæ  instructionó  tï  execute® Iæ thå useò desireó tï  singlå  steğ  aô  aî 
addresó  otheò thaî thå onå containeä iî thå prograí counter¬ theî  thå  PÃ 
registeò musô bå modifieä viá thå Ò commanä beforå thå singlå steğ  commanä 
ió issueä tï Z8E.

Allowinç  thå conveniencå oæ enterinç "S¢ <cr¾ tï executå  onå  instructioî 
haó  thå  sidå  effecô oæ noô allowinç thå useò tï  aborô  thå  commanä  iî 
betweeî  thå  timå  thå  "S¢ ió typeä anä thå <cr¾  ió  entereä  bù  simplù Šomittinç  aî argumenô anä typinç <cr>® Iæ yoõ changå youò minä yoõ caî  uså 
^Ø  tï canceì thå commanä oò yoõ caî typå iî aî invaliä argumenô  aó  ARG1® 
Thió  wilì causå á questioî marë tï bå displayed» however¬  nï  instructioî 
wilì bå executed® 

Durinç blocë tracinç (ARG± greateò thaî 1© thå commanä maù bå terminateä bù 
hittinç anù keù oî thå keyboard.

Afteò  aî  Ó  commanä  haó  beeî issueä pressinç  thå  <cr¾  keù  alonå  ió 
equivalenô tï issuinç Ó <cr>.

Thå Ó commanä doeó noô relocatå instructionó beforå executioî aó doeó thå Ç 
commanä  (seå Ç command)® Hence¬ iô ió noô possiblå tï singlå steğ  througè 
eacè iteratioî oæ á DJNÚ ¤ instruction.
.paŠ.tc     T    Select Debug Terminal ............................. #
T    Select Debug Terminal
__________________________________________________________________

Wheî Z8Å ió firsô invoked¬ thå maiî terminaì (i.å thå console© ió  enabled® 
Subsequentlù thå Ô commanä maù bå useä tï enablå aî auxiliarù terminal® 

Thå enableä terminaì displayó alì Z8Å outpuô anä ió useä bù thå operatoò tï 
enteò  commandó  anä  datá  tï Z8E® Iî thió  way¬  enablinç  thå  auxiliarù 
terminaì  allowó  thå useò tï debuç á prograí whicè makeó uså oæ  thå  maiî 
terminaì  aó  partó  oæ itó functioî withouô feaò thaô  thå  debuggeò  wilì 
"walk¢ oî hió nicå cleaî applicatioî screen® (Oæ course¬ thå useò musô HAVÅ 
á seconä terminaì tï takå advantagå oæ this.©          

The format for the T command is as follows: 

      *T  ARG1 <cr>

          wherå ARG± ió thå letteò Á tï enablå thå auxiliarù  terminaì 
          oò thå letteò C tï rå-enablå thå console terminal® 

          Anù otheò letteò wilì resulô iî aî erroò anä wilì causå  twï 
          questioî markó tï bå displayeä (terminaì selectioî doeó  noô 
          change).

          Notå thaô iæ thå auxiliarù terminaì ió enabled¬ anä thå useò 
          typeó á CTRÌ-Ã oî thå auxiliarù terminal¬ á warí booô  (surå 
          curå foò colä feet© wilì bå performeä anä thå maiî  terminaì 
          wilì  receivå thå CP/Í prompô (nï neeä tï  transfeò  controì 
          bacë tï thå maiî terminaì viá "Ô M¢ first)®         

                           * * *  CAUTION  * * *

    ------------------------------------------------------------------

Thå  useò musô havå alreadù configureä 18E'ó auxiliarù terminaì  parameteró 
anä  musô  havå á terminaì (seô tï thå propeò bauä rate© connecteä  tï  hió 
auxiliarù  porô beforå executinç á "Ô A¢ command® Failurå tï dï  thió  wilì 
resulô iî á "lockeä up¢ computeò whicè musô bå rå-booted.

    ------------------------------------------------------------------
.paŠ.tc     U    Write Symbol Table to Disk ........................ #
Õ    Writå Symboì Tablå tï Disk
________________________________________________________________ß


Thå  Õ commanä allowó thå useò tï writå thå currenô symboì tablå tï á  disë 
file® Thå formaô oæ thå commanä is:

          
     *Õ   ARG±

               ARG±  ió thå namå oæ thå filå tï whicè thå  symboì 
               tablå ió tï bå written®

Thió commanä ió usefuì tï savå anù symboì nameó entereä bù thå useò viá thå 
Á command® Thå entirå symboì tablå ió writteî tï disë usinç thå formaô oæ á 
.SYÍ  filå  (seå appendiø A)® Thå tablå caî bå subsequentlù loadeä  aô  thå 
nexô invokatioî oæ Z8E.

Notå thaô sincå thå filå ió storeä aó á .SYÍ formatteä filå thå useò shoulä 
uså  á filå namå extensioî thaô begiî witè thå letteò "S"® Thió ió  duå  tï 
thå facô thaô thå nexô timå Z8Å loadó thió symboì filå iô wilì examinå  thå 
thå  firsô characteò oæ thå filå namå extension® Iæ thå firsô characteò  ió 
aî  "S¢  thå  formaô ió assumeä tï bå .SYÍ anä thå symboì  tablå  ió  builô 
accordingly» thå appearancå oæ anù otheò letteò ió takeî tï indicatå á .PRÎ 
file® 

Iæ á filå witè thå namå ARG± alreadù existó oî disë iô wilì bå deleted® 
.paŠ.tc     V    Verify two memory blocks .......................... #
Ö    Verifù twï memorù blocks
________________________________________________________________ß


Thå  Ö  commanä allowó thå useò tï comparå twï blockó oæ memory®  Z8Å  wilì 
displaù alì differenceó betweeî thå two® Thå formaô oæ thå commanä is:

     *Ö   ARG±  ARG²  ARG³

          wherå  ARG±  ½  thå startinç addresó oæ memorù blocë ±

                 ARG²  ½  thå endinç addresó oæ memorù blocë ±

                 ARG³  ½  thå startinç addresó oæ memorù blocë ²

Z8Å  compareó memorù blocë ± tï memorù blocë ² bytå bù byte® Iæ á  mismatcè 
occuró Z8Å wilì displaù thå addresó iî eacè blocë aô whicè thå mismatcè waó 
found¬  aó  welì  aó thå bytå containeä aô  eacè  address®  Thå  comparisoî 
continueó untiì thå endinç addresó ió reached.

Thå  useò  maù halô thå commanä aô anù timå bù depressinç anù  keù  oî  thå 
keyboard.
.paŠ.tc     W    Write Memory to a file ............................ #
×    Writå Memorù tï á file
________________________________________________________________ß


Thå  ×  commanä allowó thå useò tï writå thå contentó oæ memorù tï  á  disë 
file® Thå formaô oæ thå commanä is:

          *×  arg± [arg²  arg3İ

               ARG±  ió thå namå oæ á filå tï whicè writinç  wilì 
               takå place® 

               ARG² anä ARG³ arå thå optionaì startinç anä endinç 
               addresseó  oæ thå portioî oæ memorù tï bå  writteî 
               tï  thå disk®   Iæ thå addresseó omitteä theî  thå 
               memorù  blocë  tï  bå writteî ió  defineä  bù  thå 
               startinç  anä  endinç addresseó oæ thå  lasô  filå 
               loaded®   Theså  addresseó caî bå  redisplayeä  bù 
               enterinç thå É commanä witè nï arguments®


Z8Å  alwayó deleteó anù filå oî disë whoså namå ió thå samå aó ARG1® Iæ  nï 
filå bù thió namå existó theî Z8Å wilì automaticallù creatå it.

Z8Å wilì echï thå startinç memorù addresó anä continuallù updatå thå endinç 
memorù addresó aó thå writinç tï disë takeó place.
.paŠ.tc     X    Display Machine State.............................. #
Ø    Displaù Machinå State
________________________________________________________________ß


Thå  Ø  commanä displayó thå currenô contentó oæ alì  useò  registers®  Thå 
formaô oæ thå commanä is:

          *Ø   

               nï argumentó arå requireä

Z8Å  displayó  displayó  alì registers¬ excepô thå É  registeò  anä  thå  Ò 
register¬ oî twï lineó oæ thå console® Iî addition¬ thå instructioî pointeä 
tï  bù  thå  user'ó prograí counteò ió disassembleä anä  displayeä  oî  thå 
seconä  line® Thinë oæ thió aó thå "oî deck¢ instructionº  thå  instructioî 
thaô  wilì  bå executeä upoî thå receipô oæ thå nexô Ç (GO©  oò  Ó  (SINGLÅ 
STEP© command.

Tï inspecô thå É oò Ò registeró uså thå Ò command®
.paŠ.tc     Y    Fill Memory ....................................... #
Ù    Filì Memory
________________________________________________________________ß


Thå Ù commanä filló á useò specifieä blocë oæ memorù witè á useò  specifieä 
patterî  oæ  bytes¬ thå lengtè oæ whicè ió limiteä onlù bù  thå  lengtè  oæ 
Z8E'ó inpuô buffeò whicè ió 8° byteó long:

     *Ù  ARG±  ARG²  ARG³  <cr¾

         where  ARG1  =  thå startinç addresó oæ thå blocë  tï fill

                ARG2  =  thå endinç addresó oæ thå blocë tï  fill

                ARG3  =  ió thå datá patterî tï bå writteî  tï memory® 
                         ARG³  ió evaluateä bù  Z8Å aó typå  ARGUMENÔ-
                         STRINÇ  whicè   maù bå oæ anù lengtè  iî  thå 
                         rangå   oæ  ±  througè thå  numbeò  oæ  byteó  
                         remaininç iî thå inpuô buffeò oncå  ARG±  anä 
                         ARG² havå beeî input.


Thå  Ù  commanä giveó thå useò thå capabilitù tï initializå memorù  tï  anù 
datá  pattern®  Thå capabilitù oæ enterinç multé-bytå stringó aó  thå  datá 
patterî  witè  whicè  tï filì memorù allowó thå  useò  tï  storå  repeatinç 
patternó  oæ datá iî memorù witè á singlå command® Foò examplå iæ thå  useò 
entereä thå command:

          *Ù 100° 127Ã 'abcd',16,7·

Z8Å  woulä begiî writinç thå ¶ bytå patterî (6± 6² 6³ 6´ 1¶ 77© entereä  aó 
ARG³  startinç aô addresó 1000® Thió patterî woulä repeaô aô addresó  1006¬ 
100C¬ 1012¬ etc® 

Thå  commanä endó afteò á bytå ió writteî tï thå ARG² addresó eveî iæ  thió 
bytå  doeó  noô  represenô thå lasô bytå iî thå ARG³ block®  Iî  thå  abovå 
examplå  thå commanä woulä enä wheî á bytå ió writteî tï addresó 127Ã  eveî 
iæ thaô bytå ió noô 77® 
.paŠ.tc     Z    Disassemble ....................................... #
Ú    Disassemblå
________________________________________________________________ß


Thå Ú commanä allowó thå useò tï disassemblå á blocë oæ data® Z8Å  performó 
disassembly¬  whicè  ió thå translatioî oæ binarù memorù datá  intï  sourcå 
codå  format¬ usinç thå fulì Z8° instructioî seô anä Ziloç  mnemonics®  Thå 
resultanô sourcå codå maù bå directeä tï thå consolå oò tï thå consolå  anä 
á  disë  filå  simultaneously®  Z8Å alsï allowó  thå  useò  tï  disassemblå 
interactivelù wheî ARG² ió equaì tï 1® Thå formaô oæ thå commanä is:

     *Ú  [ARG± [ARG² ARG3İ]  <cr¾

         where ARG1 ió thå addresó aô whicè dióassemblù ió tï begin

               ARG2 ió optionaì anä representó thå  uppeò limiô oæ thå 
                    disassemblù  procesó (seå detailó below)

               ARG3 ió aî optionaì filå namå specéficatioî foò  disas
                    semblù tï disk

               ARG1 maù bå oæ anù argumenô type® 

               ARG2 ió  treateä iî onå oæ twï wayó dependinç  oî   itó 
                    valueº 

                 1) Iæ  ARG² evaluateó tï á numbeò betweeî ± anä   25µ 
                    (decimal©  Z8Å wilì disassemblå iî  "blocë   mode¢ 
                    anä  ARG² becomeó á counô oæ thå numbeò   oæ  ins
                    tructionó  peò blocë tï disassemble®  Aó  wilì  bå 
                    explaineä  below¬ Z8Å pauseó afteò  eacè blocë  ió 
                    disassembleä  anä allowó thå  useò tï continuå  oò 
                    tï terminatå thå command.

                    Iæ  ARG²  ió omitteä altogetheò  á  defaulô  blocë  
                    sizå  oæ  ±  ió used®   Wheneveò  ARG²  equaló  1¬ 
                    eitheò  explicitlù  oò   bù  default¬  Z8Å  allowó 
                    inteòactivå  dióaósemblù whicè allowó thå useò  tï 
                    chooså thå outpuô formaô oæ thå data®  Interactivå 
                    dióassemblù ió discusseä below.

                 2) Iæ ARG² evaluateó tï á numbeò greateò thaî  25µ iô 
                    ió assumeä tï bå aî endinç address®  Iî thió  caså 
                    disassemblù  wilì proceeä froí   startinç  addresó 
                    (ARG1©  tï  endinç  addresó  (ARG2©  anä  nï  useò 
                    inteòventioî ió required.

               ARG3 iæ  present¬ ió assumeä tï bå thå namå oæ á   disë 
                    filå  intï whicè thå disassembleä outpuô wilì   bå 
                    written® Z8Å searcheó thå specifieä disë foò   thå 
                    nameä file® Iæ thå filå ió found¬ theî alì  disas
                    sembleä outpuô wilì bå writteî tï it¬  overwritinç 
                    anù datá thaô existeä there® Iæ thå filå  doeó noô 
                    exisô  thå  filå wilì bå createä usinç  thå   namå 
                    specifieä iî ARG3® 

               NOTEº Iæ ARG³ ió presenô ARG² musô bå explicitlù  spec
               ified¬  otherwiså Z8Å wilì mistakenlù treaô  thå   filå 
               namå aó ARG2.

Z8Å outputó tï thå consolå usinç thå followinç formatºŠ
     ADDRESÓ      OBJECÔ CODÅ      LABELº   OPCODÅ    OPERANÄ


Z8Å writeó tï disë usinç thå followinç formartº
                                   
                                   LABELº   OPCODÅ    OPERANÄ

Z8Å  disassembleó memorù blocë bù blocë iî thå useò specifieä  blocë  size® 
Afteò  eacè  blocë ió outpuô Z8Å pauseó foò useò input®  Á carriagå  returî 
inpuô  bù  thå  useò  causeó thå nexô  blocë  tï  bå  disassembleä  (unlesó 
interactivå  modå ió iî effect)¬ whilå anù otheò characteò  terminateó  thå 
command®  Perhapó  thå mosô convenienô waù tï disassemblå ió tï  specifù  á 
counô oæ one¬ eitheò explicitù oò bù omittinç ARG2¬ anä tï uså thå carriagå 
returî  aó aî on/ofæ switch®  Holdinç dowî thå retuî keù  produceó  output¬ 
releasinç thå returî keù endó output.

Z8E'ó disassembleò ió especiallù powerfuì wheî useä iî conjunctioî witè thå 
symboì facility® Bù buildinç á symboì tablå witè botè .PRÎ anä .SYÍ  files¬ 
and/oò  creatinç useò defineä symboì nameó viá thå Á command¬ thå useò  caî 
virtuallù  recreatå aî assembleò outpuô listinç (minuó thå  comments©  witè 
Z8Å insertinç labeló anä symboliã operandó whereveò possible.

Iæ  Z8Å  cannoô  matcè  aî operanä iî thå  disassembleä  instructioî  tï  á 
correspondinç symboì iî thå symboì table¬ oò iæ nï symboì tablå exists¬ Z8Å 
useó thå hexadecimaì value.

Iæ multiplå symboló iî thå symboì tablå arå equaì tï thå samå 1¶ biô  valuå 
oò address¬ Z8Å disassembleó usinç thå firsô symboì namå encountereä iî thå 
searcè oæ thå symboì tablå whicè ió equateä tï thå 1¶ biô operanä specifieä 
iî  thå  instructioî beinç disassembled® Thió wilì unavoidablù  producå  aî 
occasionaì  miónameä operanä wheî morå thaî onå symboì namå ió  equateä  tï 
thå samå 1¶ biô value® 

Z8Å  doeó  noô  substitutå symboì nameó iî  thoså  Z8°  instructionó  whicè 
referencå ¸ biô immediatå datá (ie® LÄ A,24H)® Eighô biô immediatå datá  ió 
disassembleä  aó á quoteä ASCIÉ characteò iæ it'ó absolutå valuå ió iî  thå 
rangå 2° heø tï 7Å hex» otherwise¬ iô ió disassembleä aó á heø byte.

Outpuô bù Z8Å tï á disë filå ió instantlù assemblablå bù mosô anù assembleò 
whicè  acceptó Ziloç mnemonicó withouô anù modificationó otheò thaî  addinç 
aî ENÄ statemenô aô thå enä oæ thå file.

Wheî  disassemblinç á blocë oæ memorù (startinç addresó tï endinç  address© 
thå disassemblù procesó maù bå halteä aô anù timå bù depressinç anù keù  oî 
thå keyboard.

Interactivå disassemblù allowó thå useò tï specifù thå formaô oæ thå sourcå 
codå  produceä  bù disassemblù oî á linå bù linå basis®  Interactivå  mode¬ 
whicè ió alwayó iî effecô wheneveò ARG² ió equaì tï 1¬ causeó Z8Å tï  pauså 
afteò  eacè instructioî ió dióassembled® Thió pauså foò inpuô  permitó  thå 
useò  tï enteò onå oæ thå followinç commandó tï chooså thå  desireä  outpuô 
format:

    CHARACTEÒ            OUTPUÔ FORMAÔ         EXAMPLÅ

     
        Á                 ASCIÉ DEFÂ          DEFÂ   'Q§

        Â                  HEØ DEFÂ           DEFÂ   23È    
Š        Ã                    CODÅ             EØ     DE,HÌ

        Ä                  HEØ DEF×           DEF×   02FCÈ  oò    
                                              DEF×   LABEÌ

        »                 adä COMMENÔ         ;Thió ió á Commenô     

     carriagå           (terminatå command©
      returî
   
     anù otheò           PROCEEÄ TÏ THÅ
     characteò          NEXÔ INSTRUCTIOÎ     


     ASCIÉ DEFBº

          Thå contentó oæ memorù aô thå currenô disassemblù addresó ió 
          converteä  tï  á quoteä ASCIÉ character® Valueó   lesó  thaî 
          hexadecimaì 2° (ASCIÉ space© oò greateò thaî  hexadecimaì 7Å 
          (ASCIÉ tilde© cannoô bå disassembleä  intï thió format® 

     HEØ DEFBº

          Thå  ¸  biô contentó oæ memorù aô  thå  currenô  disassemblù  
          addresó arå converteä tï á heø byte.

     CODEº

          Thió ió thå normaì defaulô foò disassembly® Aó Z8Å  moveó oî 
          tï  á  ne÷ addresó iô wilì alwayó displaù thå   contentó  oæ 
          memorù  aó  á  Z8° instruction® Thå "C¢ ió  onlù  needeä  tï 
          redisplaù  thå contentó oæ memorù aó aî instructioî haä  onå 
          oæ thå otheò characteró (A¬ Â oò D© alreadù beeî entered® 

     HEØ DEFWº

          Thå  contentó  oæ thå twï byteó oæ memorù  startinç  aô  thå  
          loactioî oæ thå currenô disassemblù addresó arå outpuô  aó á 
          definå worä directive® Thå bytå pointeä tï  directlù bù  thå 
          currenô  disassemblù addresó becomeó thå  lo÷ ordeò bytå  oæ 
          thå  operand®  Thå  bytå aô disassemblù   addresó  pluó  onå 
          becomeó thå higè ordeò byte.

     NOTEº
          Iæ  Z8Å haä jusô disassembleä á multé­ bytå Z8°  instructioî 
          anä thå useò entereä anù oæ thå characteró listeä abovå  (A¬ 
          B¬  C¬ oò D© onlù thå firsô byte¬ oò  firsô twï foò "D"¬  oæ 
          thå  instructioî   woulä  bå  converteä  tï  thå   requesteä  
          format®  Thå  remaininç byteó oæ thå  instructioî  woulä  bå 
          treateä  aó á ne÷  Z8° instructioî oncå thå  useò  proceedeä  
          tï thå nexô disassemblù address® 

     ADDINÇ COMMENTÓ

          Z8Å  allowó  thå  useò  tï  adä  onå  commenô  peò  linå  oæ  
          disassembleä code® Iæ MAXLEÎ ió seô tï ¶ theî commentó   maù 
          bå  uğ tï 2¹ characteró iî length® Iæ MAXLEÎ ió seô   tï  1´ 
          theî commentó maù bå uğ tï 1¶ characteró iî  length.


Iæ  durinç  disassembly¬ Z8Å encounteró datá whicè cannoô  bå  disassembleä Šintï á valiä Z8° instructioî iô wilì displaù thå datá aó DEFBs.
.paŠ.tã     ?    Evaluate Expression ..............................® #
?    Evaluate Expression
________________________________________________________________ß


          *?   ARG1 <cr>

               where ARG1 is any legal form.

Thå  "?¢  commanä  evaluateó anä displayó  itó  argument®   Indirectioî  ió 
supporteä sï thaô   *¿   (HL©   displayó thå worä aô thå addresó  containeä 
iî HL.

Thå ruleó foò expressioî formatioî arå describeä beforå thå listinç oæ  Z8Å 
commands.
.paŠ.tc     >    Change user number ................................ #
>    Change user number
________________________________________________________________ß


          *>   ARG1 <cr>

               where evaluates to a legal user number.


Usefuì  iî  preparatioî  foò  á L(oad©  command¬  thå  ">¢  commanä  allowó 
switchinç useò numberó afteò Z8Å ió activå sï thaô yoõ maù theî loaä  fileó 
froí useò areaó otheò thaî thå onå froí whicè Z8Å waó invoked®  
.paŠ.tc APPENDIØ A ­ FILÅ FORMAÔ FOÒ SYMBOÌ TABLEÓ ................. #
                APPENDIØ A ­ FILÅ FORMAÔ FOÒ SYMBOÌ TABLEÓ

Z8Å  ió  currentlù seô uğ tï bå ablå tï reaä anù oæ  thå  listinç 
fileó  whicè appeaò belowº

    1® Microsofô   MACRO-8°    V3.3·  .PRÎ fileó  Maù 8¬ 198°
    2® Microsofô   MACRO-8°    V3.4´  .PRÎ fileó  Deã 9¬ 198±
    3® Microsofô   LINK-8°     V3.4´  .SYÍ fileó  Deã 9¬ 198± 
    4® SLÒ Systemó Z80ASÍ      V1.0·  .LSÔ fileó
    5® SLÒ Systemó SLRNË       V1.0·  .SYÍ fileó       
    6® SLÒ Systemó Z80ASÍ      V1.³   .PRÎ fileó

Thå uniquå characteristicó oæ eacè areº

    MACRO-8° V3.3·

          Z8Å searcheó foò thå ¸ bytå strinç "Symbols:¢ iî thå   file® 
          Oncå  thió strinç ió found¬ Z8Å expectó aî  ASCIÉ   carriagå 
          returî characteò anä aî ASCIÉ linå feeä characteò tï bå  thå 
          nexô twï byteó iî thå file® Thå symboì  tablå listinç shoulä 
          begiî iî thå nexô characteò positioî  iî thå file.

          Eacè linå oæ thå symboì tablå listinç containó fouò   symboì 
          nameó anä aî associateä address® 

          Iæ  thå  characteò followinç thå symbol'ó heø  valuå  ió  aî  
          apostophe¬ thå symboì ió considereä tï bå prograí  relative® 
          Iæ  thå useò specifieä á biaó iî thå commanä linå  thå  biaó 
          wilì bå addeä tï thå symbol'ó value.

          Iæ thå characteò followinç thå symbol'ó heø valuå ió aî  "I¢ 
          (meaninç  thaô  thå symboì ió globallù  defined©  theî   thå 
          characteò followinç thå "I¢ ió examined® Iæ thió   characteò 
          ió  aî apostrophå iô ió considereä tï bå  prograí   relativå 
          anä thå bias¬ iæ specifieä ió addeä tï thå  value.

          Iæ thå characteò followinç thå heø symboì valuå oò thå   "I¢ 
          ió  anù  characteò  besideó aî  apostrophe¬  thå  symboì  ió  
          considereä absolutå anä thå biaó wilì noô bå added.

          Thå  filå  shoulä bå terminateä witè  thå  CP/Í  enä-oæ-filå  
          characteò (controì Ú whicè ió equivalenô tï á heø 1A).

          Iæ  thå  strinç  "Symbols¢ ió neveò found¬  Z8Å  printó  thå  
          messageº Symboì Tablå noô Found
.paŠ    MACRO-8°  V3.4´

          Z8Å searcheó foò thå ¸ bytå strinç "Symbols:¢ iî thå   file® 
          Oncå  thió strinç ió found¬ Z8Å expectó aî  ASCIÉ   carriagå 
          returî characteò anä aî ASCIÉ linå feeä characteò tï bå  thå 
          nexô twï byteó iî thå file® Thå symboì  tablå listinç shoulä 
          begiî iî thå nexô characteò positioî  iî thå file.

          Iî thió releaså oæ MACRÏ-8° thå formaô oæ thå symboì   tablå 
          ió  completelù  oppositå oæ V3.37® Thaô is¬ thå  heø   valuå 
          appearó  beforå  thå symboì name® Iî  addition¬  theså   heø 
          value/symboì namå combinatioî appeaò threå peò line.

          Thå  characteò appearinç afteò thå heø valuå ió  interpreteä 
          aó describeä foò versioî 3.37® 

          Iæ  thå  strinç  "Symbols¢ ió neveò found¬  Z8Å  printó  thå  
          messageº Symboì Tablå noô Found

    
    LINK-8°  V3.4´

          LINË-8° caî optionallù producå á linë mağ (.SYÍ file©  whicè 
          listó  alì globallù defineä symboló iæ thå  useò   specifieó 
          thå  "Y¢  optioî  thå L8° commanä  line®   Z8Å   treató  alì 
          symboló  nameó loadeä froí á LINË-8° .SYÍ filå  aó  absolutå 
          (noî-relocatable©  addresses®  Nevertheless¬   iæ  thå  useò 
          specifieó  á bias¬ iô wilì bå addeä tï everù   symboì  valuå 
          reaä iî froí thå .SYÍ file.

          Z8Å expectó thå firsô symboì valuå iî á .SYÍ filå tï   begiî 
          iî  thå firsô bytå positioî iî thå file® Eacè  symboì  valuå 
          consistó  oæ fouò hexadecimaì byteó iî ASCIÉ  followeä bù  á 
          taâ  character® Immediatelù afteò thå taâ  characteò ió  thå 
          symboì  namå whicè maù bå betweeî onå anä  siø  alphanumeriã 
          characteró iî length® Thå symboì namå  ió followeä bù á  taâ 
          anä thå sequencå repeats® Everù  fourtè symboì  value/symboì 
          namå paiò shoulä bå followeä  bù á carriagå returî anä  linå 
          feed.

          Thå  filå  shoulä bå terminateä witè  thå  CP/Í  enä-oæ-filå  
          characteò (controì Ú whicè ió equivalenô tï á heø 1A).


    Z80ASÍ

          Z80ASÍ maù bå configureä tï producå eitheò 8° oò 13²  columî 
          output.

          Z8Å  searcheó foò thå ¸ bytå strinç "Symboì Table:¢  iî  thå  
          file® Thió strinç neeä noô bå aô thå beginninç oæ thå  file» 
          Z8Å  wilì  scaî thå entirå filå lookinç foò it®  Oncå   thió 
          strinç  ió  found¬  Z8Å expectó aî  ASCIÉ  carriagå   returî 
          characteò  anä aî ASCIÉ linå feeä characteò tï bå  thå  nexô 
          twï byteó iî thå file® Thå symboì tablå listinç shoulä begiî 
          iî thå nexô characteò positioî iî thå  file.

          Iî  á  Z80ASÍ  .LSÔ filå thå heø valuå  appearó  beforå  thå  
          symboì  name®  Heø  value/symboì  namå  combinationó  appeaò  
          threå  peò  line® Z80ASÍ symboì nameó maù contaiî uğ  tï  1¶  
          characters®  Z8Å  wilì accepô thå firsô 1´ characteró  oæ  á  Š          symboì namå iæ MAXLEÎ ió seô tï 1´ oò thå firsô ¶ characteró 
          iæ MAXLEÎ ió seô tï 6® 

          Iæ  thå  strinç "Symboì Table:¢ ió neveò found¬  Z8Å  printó  
          thå messageº 

                     Symboì Tablå Noô Founä

    SLRNË

          SLRNË caî optionallù producå á linë mağ (.SYÍ File©  similaò 
          tï  thå  onå produceä bù Linë-80® Z8Å  treató  alì   symboló 
          loadeä froí á SLRNË .SYÍ filå aó absolutå symbols®  However¬ 
          aó  iî  thå  caså oæ Linë-8° .SYÍ files¬  Z8Å   wilì  adä  á 
          relocatioî biaó tï eacè symboì iæ onå ió  specified.

          Eacè  symboì  valuå iî á SLRNË .SYÍ filå  consistó  oæ  fouò  
          hexadecimaì  byteó  followeä  bù á  spacå  followeä  bù  thå  
          symboì  name® Thå symboì namå ió followeä bù twï ASCIÉ   taâ 
          characters® 

          Uså SLRNK'ó /Í optioî tï producå á linë map.

NOTEº

     Whilå  readinç iî á MACRÏ-8° .PRÎ file¬ oò á Z80ASÍ  .LSÔ   file¬ 
     Z8Å ió capablå oæ readinç aî entirå assemblù listinç filå lookinç 
     foò  thå  "Symbols:¢  strinç oò "Symboì   Table:¢  string®  Theså 
     stringó  neeä  noô  bå locateä aô thå   beginninç  oæ  thå  file® 
     However¬  thå  loadinç oæ thå  symboì tablå wilì  bå  speedeä  uğ 
     considerablù  iæ thå  symboì tablå ió thå onlù datá iî thå  file® 
     Thió  ió  accomplisheä quitå easilù iî botè MACRÏ-8°  bù  turninç 
     ofæ   thå  listinç  durinç aî assemblù througè  thå  uså  oæ  thå  
     .XLISÔ  directive® Thå listinç caî theî bå turneä bacë  oî   jusô 
     prioò tï thå ENÄ directivå viá á .LISÔ directivå tï  ensurå  thaô 
     thå symboì tablå ió writteî tï disk.

     Iæ yoõ arå usinç Z80ASÍ uså thå /Ó optioî tï instrucô  Z80ASÍ  tï 
     producå á symboì file.

     Z8Å ió ablå tï procesó symboì tableó whicè occupù multiplå  pageó 
     iî anù oæ thå filå typeó mentioneä above® Headingó whicè  precedå 
     eacè pagå arå automaticallù ignoreä bù Z8E®
.paŠ.tc APPENDIX B - ZILOG MNEMONICS ............................... #
                       APPENDIØ Â ­ ZILOÇ MNEMONICS

  004¹                  NÎ     EQÕ   49È       ;¸ BIÔ OPERANÄ
  123Æ                  NNNÎ   EQÕ   123FÈ     ;1¶ BIÔ   OPERANÄ
  003¶                  INDEØ  EQÕ   36È       ;INDEØ REGISTEÒ INDEØ


  010Â    8Å                   ADÃ   A,(HL©
  010Ã    DÄ 8Å 3¶             ADÃ   A,(IX+INDEX©
  010Æ    FÄ 8Å 3¶             ADÃ   A,(IY+INDEX©
  011²    8Æ                   ADÃ   A,Á
  011³    8¸                   ADÃ   A,Â
  011´    8¹                   ADÃ   A,Ã
  011µ    8Á                   ADÃ   A,Ä
  011¶    8Â                   ADÃ   A,Å
  011·    8Ã                   ADÃ   A,È
  011¸    8Ä                   ADÃ   A,Ì
  011¹    CÅ 4¹                ADÃ   A,NÎ
  011Â    EÄ 4Á                ADÃ   HL,BÃ
  011Ä    EÄ 5Á                ADÃ   HL,DÅ
  011Æ    EÄ 6Á                ADÃ   HL,HÌ
  012±    EÄ 7Á                ADÃ   HL,SĞ


  012³    8¶                   ADÄ   A,(HL©
  012´    DÄ 8¶ 3¶             ADÄ   A,(IX+INDEX©
  012·    FÄ 8¶ 3¶             ADÄ   A,(IY+INDEX©
  012Á    8·                   ADÄ   A,Á
  012Â    8°                   ADÄ   A,Â
  012Ã    8±                   ADÄ   A,Ã
  012Ä    8²                   ADÄ   A,Ä
  012Å    8³                   ADÄ   A,Å
  012Æ    8´                   ADÄ   A,È
  013°    8µ                   ADÄ   A,Ì
  013±    C¶ 4¹                ADÄ   A,NÎ
  013³    0¹                   ADÄ   HL,BÃ
  013´    1¹                   ADÄ   HL,DÅ
  013µ    2¹                   ADÄ   HL,HÌ
  013¶    3¹                   ADÄ   HL,SĞ
  013·    DÄ 0¹                ADÄ   IX,BÃ
  013¹    DÄ 1¹                ADÄ   IX,DÅ
  013Â    DÄ 2¹                ADÄ   IX,IØ
  013Ä    DÄ 3¹                ADÄ   IX,SĞ
  013Æ    FÄ 0¹                ADÄ   IY,BÃ
  014±    FÄ 1¹                ADÄ   IY,DÅ
  014³    FÄ 2¹                ADÄ   IY,IÙ
  014µ    FÄ 3¹                ADÄ   IY,SĞ


  014·    A¶                   ANÄ   (HL©
  014¸    DÄ A¶ 3¶             ANÄ   (IX+INDEX©
  014Â    FÄ A¶ 3¶             ANÄ   (IY+INDEX©
  014Å    A·                   ANÄ   A
  014Æ    A°                   ANÄ   Â
  015°    A±                   ANÄ   Ã
  015±    A²                   ANÄ   Ä
  015²    A³                   ANÄ   Å
  015³    A´                   ANÄ   È
  015´    Aµ                   ANÄ   Ì
  015µ    E¶ 4¹                ANÄ   NÎ
Š
  015·    CÂ 4¶                BIÔ   0,(HL©
  015¹    DÄ CÂ 3¶ 4¶          BIÔ   0,(IX+INDEX©
  015Ä    FÄ CÂ 3¶ 4¶          BIÔ   0,(IY+INDEX©
  016±    CÂ 4·                BIÔ   0,Á
  016³    CÂ 4°                BIÔ   0,Â
  016µ    CÂ 4±                BIÔ   0,Ã
  016·    CÂ 4²                BIÔ   0,Ä
  016¹    CÂ 4³                BIÔ   0,Å
  016Â    CÂ 4´                BIÔ   0,È
  016Ä    CÂ 4µ                BIÔ   0,Ì


  016Æ    CÂ 4Å                BIÔ   1,(HL©
  017±    DÄ CÂ 3¶ 4Å          BIÔ   1,(IX+INDEX©
  017µ    FÄ CÂ 3¶ 4Å          BIÔ   1,(IY+INDEX©
  017¹    CÂ 4Æ                BIÔ   1,Á
  017Â    CÂ 4¸                BIÔ   1,Â
  017Ä    CÂ 4¹                BIÔ   1,Ã
  017Æ    CÂ 4Á                BIÔ   1,Ä
  018±    CÂ 4Â                BIÔ   1,Å
  018³    CÂ 4Ã                BIÔ   1,È
  018µ    CÂ 4Ä                BIÔ   1,Ì


  018·    CÂ 5¶                BIÔ   2,(HL©
  018¹    DÄ CÂ 3¶ 5¶          BIÔ   2,(IX+INDEX©
  018Ä    FÄ CÂ 3¶ 5¶          BIÔ   2,(IY+INDEX©
  019±    CÂ 5·                BIÔ   2,Á
  019³    CÂ 5°                BIÔ   2,Â
  019µ    CÂ 5±                BIÔ   2,Ã
  019·    CÂ 5²                BIÔ   2,Ä
  019¹    CÂ 5³                BIÔ   2,Å
  019Â    CÂ 5´                BIÔ   2,È
  019Ä    CÂ 5µ                BIÔ   2,Ì


  019Æ    CÂ 5Å                BIÔ   3,(HL©
  01A±    DÄ CÂ 3¶ 5Å          BIÔ   3,(IX+INDEX©
  01Aµ    FÄ CÂ 3¶ 5Å          BIÔ   3,(IY+INDEX©
  01A¹    CÂ 5Æ                BIÔ   3,Á
  01AÂ    CÂ 5¸                BIÔ   3,Â
  01AÄ    CÂ 5¹                BIÔ   3,Ã
  01AÆ    CÂ 5Á                BIÔ   3,Ä
  01B±    CÂ 5Â                BIÔ   3,Å
  01B³    CÂ 5Ã                BIÔ   3,È
  01Bµ    CÂ 5Ä                BIÔ   3,L


  01B·    CÂ 6¶                BIÔ   4,(HL©
  01B¹    DÄ CÂ 3¶ 6¶          BIÔ   4,(IX+INDEX©
  01BÄ    FÄ CÂ 3¶ 6¶          BIÔ   4,(IY+INDEX©
  01C±    CÂ 6·                BIÔ   4,Á
  01C³    CÂ 6°                BIÔ   4,Â
  01Cµ    CÂ 6±                BIÔ   4,Ã
  01C·    CÂ 6²                BIÔ   4,Ä
  01C¹    CÂ 6³                BIÔ   4,Å
  01CÂ    CÂ 6´                BIÔ   4,È
  01CÄ    CÂ 6µ                BIÔ   4,Ì

Š  01CÆ    CÂ 6Å                BIÔ   5,(HL©
  01D±    DÄ CÂ 3¶ 6Å          BIÔ   5,(IX+INDEX©
  01Dµ    FÄ CÂ 3¶ 6Å          BIÔ   5,(IY+INDEX©
  01D¹    CÂ 6Æ                BIÔ   5,Á
  01DÂ    CÂ 6¸                BIÔ   5,Â
  01DÄ    CÂ 6¹                BIÔ   5,Ã
  01DÆ    CÂ 6Á                BIÔ   5,Ä
  01E±    CÂ 6Â                BIÔ   5,Å
  01E³    CÂ 6Ã                BIÔ   5,È
  01Eµ    CÂ 6Ä                BIÔ   5,Ì


  01E·    CÂ 7¶                BIÔ   6,(HL©
  01E¹    DÄ CÂ 3¶ 7¶          BIÔ   6,(IX+INDEX©
  01EÄ    FÄ CÂ 3¶ 7¶          BIÔ   6,(IY+INDEX©
  01F±    CÂ 7·                BIÔ   6,Á
  01F³    CÂ 7°                BIÔ   6,Â
  01Fµ    CÂ 7±                BIÔ   6,Ã
  01F·    CÂ 7²                BIÔ   6,Ä
  01F¹    CÂ 7³                BIÔ   6,Å
  01FÂ    CÂ 7´                BIÔ   6,È
  01FÄ    CÂ 7µ                BIÔ   6,Ì


  01FÆ    CÂ 7Å                BIÔ   7,(HL©
  020±    DÄ CÂ 3¶ 7Å          BIÔ   7,(IX+INDEX©
  020µ    FÄ CÂ 3¶ 7Å          BIÔ   7,(IY+INDEX©
  020¹    CÂ 7Æ                BIÔ   7,Á
  020Â    CÂ 7¸                BIÔ   7,Â
  020Ä    CÂ 7¹                BIÔ   7,Ã
  020Æ    CÂ 7Á                BIÔ   7,Ä
  021±    CÂ 7Â                BIÔ   7,Å
  021³    CÂ 7Ã                BIÔ   7,È
  021µ    CÂ 7Ä                BIÔ   7,Ì


  021·    DÃ 123Æ              CALÌ   C,NNNÎ
  021Á    FÃ 123Æ              CALÌ   M,NNNÎ
  021Ä    D´ 123Æ              CALÌ   NC,NNNÎ
  022°    CÄ 123Æ              CALÌ   NNNÎ
  022³    C´ 123Æ              CALÌ   NZ,NNNN
  022¶    F´ 123Æ              CALÌ   P,NNNÎ
  022¹    EÃ 123Æ              CALÌ   PE,NNNÎ
  022Ã    E´ 123Æ              CALÌ   PO,NNNÎ
  022Æ    CÃ 123Æ              CALÌ   Z,NNNÎ


  023²    3Æ                   CCÆ


  023³    BÅ                   CĞ   (HL©
  023´    DÄ BÅ 3¶             CĞ   (IX+INDEX©
  023·    FÄ BÅ 3¶             CĞ   (IY+INDEX©
  023Á    BÆ                   CĞ   Á
  023Â    B¸                   CĞ   Â
  023Ã    B¹                   CĞ   Ã
  023Ä    BÁ                   CĞ   Ä
  023Å    BÂ                   CĞ   Å
  023Æ    BÃ                   CĞ   È
  024°    BÄ                   CĞ   Ì
  024±    FÅ 4¹                CĞ   NÎŠ

  024³    EÄ A¹                CPÄ
  024µ    EÄ B¹                CPDÒ
  024·    EÄ A±                CPÉ
  024¹    EÄ B±                CPIÒ


  024Â    2Æ                   CPÌ


  024Ã    2·                   DAÁ


  024Ä    3µ                   DEÃ   (HL©
  024Å    DÄ 3µ 3¶             DEÃ   (IX+INDEX©
  025±    FÄ 3µ 3¶             DEÃ   (IY+INDEX©
  025´    3Ä                   DEÃ   Á
  025µ    0µ                   DEÃ   Â
  025¶    0Â                   DEÃ   BÃ
  025·    0Ä                   DEÃ   Ã
  025¸    1µ                   DEÃ   Ä
  025¹    1Â                   DEÃ   DÅ
  025Á    1Ä                   DEÃ   Å
  025Â    2µ                   DEÃ   È
  025Ã    2Â                   DEÃ   HÌ
  025Ä    DÄ 2Â                DEÃ   IØ
  025Æ    FÄ 2Â                DEÃ   IÙ
  026±    2Ä                   DEÃ   Ì
  026²    3Â                   DEÃ   SĞ


  026³    F³                   DÉ


  026´    1° 0´                DJNÚ   $+¶


  026¶    FÂ                   EÉ


  026·    E³                   EØ   (SP),HÌ
  026¸    DÄ E³                EØ   (SP),IØ
  026Á    FÄ E³                EØ   (SP),IÙ
  026Ã    0¸                   EØ   AF,AF§          
  026Ä    EÂ                   EØ   DE,HÌ
  026Å    D¹                   EXØ


  026Æ    7¶                   HALÔ


  027°    EÄ 4¶                IÍ   °
  027²    EÄ 5¶                IÍ   ±
  027´    EÄ 5Å                IÍ   ²


  027¶    EÄ 7¸                IÎ   A,(C©
  027¸    DÂ 4¹                IÎ   A,(NN©
  027Á    EÄ 4°                IÎ   B,(C©
  027Ã    EÄ 4¸                IÎ   C,(C©Š  027Å    EÄ 5°                IÎ   D,(C©
  028°    EÄ 5¸                IÎ   E,(C©
  028´    EÄ 6°                IÎ   H,(C©
  028¶    EÄ 6¸                IÎ   L,(C©


  028¸    3´                   INÃ   (HL©
  028¹    DÄ 3´ 3¶             INÃ   (IX+INDEX©
  028Ã    FÄ 3´ 3¶             INÃ   (IY+INDEX©
  028Æ    3Ã                   INÃ   Á
  029°    0´                   INÃ   Â
  029±    0³                   INÃ   BÃ
  029²    0Ã                   INÃ   Ã
  029³    1´                   INÃ   Ä
  029´    1³                   INÃ   DÅ
  029µ    1Ã                   INÃ   Å
  029¶    2´                   INÃ   È
  029·    2³                   INÃ   HÌ
  029¸    DÄ 2³                INÃ   IØ
  029Á    FÄ 2³                INÃ   IÙ
  029Ã    2Ã                   INÃ   Ì
  029Ä    3³                   INÃ   SĞ


  029Å    EÄ AÁ                INÄ
  02A°    EÄ BÁ                INDÒ
  02A²    EÄ A²                INÉ
  02A´    EÄ B²                INIÒ


  02A¶    E¹                   JĞ   (HL©
  02A·    DÄ E¹                JĞ   (IX©
  02A¹    FÄ E¹                JĞ   (IY©
  02AÂ    DÁ 123Æ              JĞ   C,NNNÎ
  02AÅ    FÁ 123Æ              JĞ   M,NNNÎ
  02B±    D² 123Æ              JĞ   NC,NNNÎ
  02B´    C³ 123Æ              JĞ   NNNÎ
  02B·    C² 123Æ              JĞ   NZ,NNNÎ
  02BÁ    F² 123Æ              JĞ   P,NNNÎ
  02BÄ    EÁ 123Æ              JĞ   PE,NNNÎ
  02C°    E² 123Æ              JĞ   PO,NNNÎ
  02C³    CÁ 123Æ              JĞ   Z,NNNÎ


  02C¶    3¸ 0´                JÒ   C,$+¶
  02C¸    1¸ 0´                JÒ   $+¶
  02CÁ    3° 0´                JÒ   NC,$+¶
  02CÃ    2° 0´                JÒ   NZ,$+¶
  02CÅ    2¸ 0´                JÒ   Z,$+¶


  02D°    0²                   LÄ   (BC),Á
  02D±    1²                   LÄ   (DE),Á
  02D²    7·                   LÄ   (HL),Á
  02D³    7°                   LÄ   (HL),Â
  02D´    7±                   LÄ   (HL),Ã
  02Dµ    7²                   LÄ   (HL),Ä
  02D¶    7³                   LÄ   (HL),Å
  02D·    7´                   LÄ   (HL),È
  02D¸    7µ                   LÄ   (HL),Ì
  02D¹    3¶ 4¹                LÄ   (HL),NÎŠ

  02DÂ    DÄ 7· 3¶             LÄ   (IX+INDEX),Á
  02DÅ    DÄ 7° 3¶             LÄ   (IX+INDEX),Â
  02E±    DÄ 7± 3¶             LÄ   (IX+INDEX),Ã
  02E´    DÄ 7² 3¶             LÄ   (IX+INDEX),Ä
  02E·    DÄ 7³ 3¶             LÄ   (IX+INDEX),Å
  02EÁ    DÄ 7´ 3¶             LÄ   (IX+INDEX),È
  02EÄ    DÄ 7µ 3¶             LÄ   (IX+INDEX),Ì
  02F°    DÄ 3¶ 3¶ 4¹          LÄ   (IX+INDEX),NÎ


  02F´    FÄ 7· 3¶             LÄ   (IY+INDEX),Á
  02F·    FÄ 7° 3¶             LÄ   (IY+INDEX),Â
  02FÁ    FÄ 7± 3¶             LÄ   (IY+INDEX),Ã
  02FÄ    FÄ 7² 3¶             LÄ   (IY+INDEX),Ä
  030°    FÄ 7³ 3¶             LÄ   (IY+INDEX),Å
  030³    FÄ 7´ 3¶             LÄ   (IY+INDEX),È
  030¶    FÄ 7µ 3¶             LÄ   (IY+INDEX),Ì
  030¹    FÄ 3¶ 3¶ 4¹          LÄ   (IY+INDEX),NÎ


  030Ä    3² 123Æ              LÄ   (NNNN),A
  031°    EÄ 4³ 123Æ           LÄ   (NNNN),BÃ
  031´    EÄ 5³ 123Æ           LÄ   (NNNN),DÅ
  031¸    2² 123Æ              LÄ   (NNNN),HÌ
  031Â    DÄ 2² 123Æ           LÄ   (NNNN),IØ
  031Æ    FÄ 2² 123Æ           LÄ   (NNNN),IÙ
  032³    EÄ 7³ 123Æ           LÄ   (NNNN),SĞ


  032·    0Á                   LÄ   A,(BC©
  032¸    1Á                   LÄ   A,(DE©
  032¹    7Å                   LÄ   A,(HL©
  032Á    DÄ 7Å 3¶             LÄ   A,(IX+INDEX©
  032Ä    FÄ 7Å 3¶             LÄ   A,(IY+INDEX©
  033°    3Á 123Æ              LÄ   A,(NNNN©
  033³    7Æ                   LÄ   A,Á
  033´    7¸                   LÄ   A,Â
  033µ    7¹                   LÄ   A,Ã
  033¶    7Á                   LÄ   A,Ä
  033·    7Â                   LÄ   A,Å
  033¸    7Ã                   LÄ   A,È
  033¹    EÄ 5·                LÄ   A,É
  033Â    7Ä                   LÄ   A,Ì
  033Ã    3Å 4¹                LÄ   A,NÎ
  033Å    EÄ 5Æ                LÄ   A,Ò


  034°    4¶                   LÄ   B,(HL©
  034±    DÄ 4¶ 3¶             LÄ   B,(IX+INDEX©
  034´    FÄ 4¶ 3¶             LÄ   B,(IY+INDEX©
  034·    4·                   LÄ   B,Á
  034¸    4°                   LÄ   B,Â
  034¹    4±                   LÄ   B,Ã
  034Á    4²                   LÄ   B,Ä
  034Â    4³                   LÄ   B,Å
  034Ã    4´                   LÄ   B,È
  034Ä    4µ                   LÄ   B,Ì
  034Å    0¶ 4¹                LÄ   B,NÎ
Š
  035°    EÄ 4Â 123Æ           LÄ   BC,(NNNN©
  035´    0± 123Æ              LÄ   BC,NNNÎ


  035·    4Å                   LÄ   C,(HL©
  035¸    DÄ 4Å 3¶             LÄ   C,(IX+INDEX©
  035Â    FÄ 4Å 3¶             LÄ   C,(IY+INDEX©
  035Å    4Æ                   LÄ   C,Á
  035Æ    4¸                   LÄ   C,Â
  036°    4¹                   LÄ   C,Ã
  036±    4Á                   LÄ   C,Ä
  036²    4Â                   LÄ   C,Å
  036³    4Ã                   LÄ   C,È
  036´    4Ä                   LÄ   C,Ì
  036µ    0Å 4¹                LÄ   C,NÎ


  036·    5¶                   LÄ   D,(HL©
  036¸    DÄ 5¶ 3¶             LÄ   D,(IX+INDEX©
  036Â    FÄ 5¶ 3¶             LÄ   D,(IY+INDEX©
  036Å    5·                   LÄ   D,Á
  036Æ    5°                   LÄ   D,Â
  037°    5±                   LÄ   D,Ã
  037±    5²                   LÄ   D,Ä
  037²    5³                   LÄ   D,Å
  037³    5´                   LÄ   D,È
  037´    5µ                   LÄ   D,Ì
  037µ    1¶ 4¹                LÄ   D,NÎ


  037·    EÄ 5Â 123Æ           LÄ   DE,(NNNN©
  037Â    1± 123Æ              LÄ   DE,NNNÎ


  037Å    5Å                   LÄ   E,(HL©
  037Æ    DÄ 5Å 3¶             LÄ   E,(IX+INDEX©
  038²    FÄ 5Å 3¶             LÄ   E,(IY+INDEX©
  038µ    5Æ                   LÄ   E,Á
  038¶    5¸                   LÄ   E,Â
  038·    5¹                   LÄ   E,Ã
  038¸    5Á                   LÄ   E,Ä
  038¹    5Â                   LÄ   E,Å
  038Á    5Ã                   LÄ   E,È
  038Â    5Ä                   LÄ   E,Ì
  038Ã    1Å 4¹                LÄ   E,NÎ


  038Å    6¶                   LÄ   H,(HL©
  038Æ    DÄ 6¶ 3¶             LÄ   H,(IX+INDEX©
  039²    FÄ 6¶ 3¶             LÄ   H,(IY+INDEX©
  039µ    6·                   LÄ   H,Á
  039¶    6°                   LÄ   H,Â
  039·    6±                   LÄ   H,Ã
  039¸    6²                   LÄ   H,Ä
  039¹    6³                   LÄ   H,Å
  039Á    6´                   LÄ   H,È
  039Â    6µ                   LÄ   H,Ì
  039Ã    2¶ 4¹                LÄ   H,NÎ

Š  039Å    2Á 123Æ              LÄ   HL,(NNNN©
  03A±    2± 123Æ              LÄ   HL,NNNÎ


  03A´    EÄ 4·                LÄ   I,Á


  03A¶    DÄ 2Á 123Æ           LÄ   IX,(NNNN©
  03AÁ    DÄ 2± 123Æ           LÄ   IX,NNNÎ


  03AÅ    FÄ 2Á 123Æ           LÄ   IY,(NNNN©
  03B²    FÄ 2± 123Æ           LÄ   IY,NNNÎ


  03B¶    6Å                   LÄ   L,(HL©
  03B·    DÄ 6Å 3¶             LÄ   L,(IX+INDEX©
  03BÁ    FÄ 6Å 3¶             LÄ   L,(IY+INDEX©
  03BÄ    6Æ                   LÄ   L,Á
  03BÅ    6¸                   LÄ   L,Â
  03BÆ    6¹                   LÄ   L,Ã
  03C°    6Á                   LÄ   L,Ä
  03C±    6Â                   LÄ   L,Å
  03C²    6Ã                   LÄ   L,È
  03C³    6Ä                   LÄ   L,Ì
  03C´    2Å 4¹                LÄ   L,NÎ


  03C¶    EÄ 4Æ                LÄ   R,Á


  03C¸    EÄ 7Â 123Æ           LÄ   SP,(NNNN©
  03CÃ    F¹                   LÄ   SP,HÌ
  03CÄ    DÄ F¹                LÄ   SP,IØ
  03CÆ    FÄ F¹                LÄ   SP,IÙ
  03D±    3± 123Æ              LÄ   SP,NNNÎ


  03D´    EÄ A¸                LDÄ
  03D¶    EÄ B¸                LDDÒ
  03D¸    EÄ A°                LDÉ
  03DÁ    EÄ B°                LDIÒ
                        
                        
  03DÃ    EÄ 4´                NEÇ


  03DÅ    0°                   NOĞ


  03DÆ    B¶                   OÒ   (HL©
  03E°    DÄ B¶ 3¶             OÒ   (IX+INDEX©
  03E³    FÄ B¶ 3¶             OÒ   (IY+INDEX©
  03E¶    B·                   OÒ   Á
  03E·    B°                   OÒ   Â
  03E¸    B±                   OÒ   Ã
  03E¹    B²                   OÒ   Ä
  03EÁ    B³                   OÒ   Å
  03EÂ    B´                   OÒ   È
  03EÃ    Bµ                   OÒ   Ì
  03EÄ    F¶ 4¹                OÒ   NÎŠ

  03EÆ    EÄ BÂ                OTDÒ
  03F±    EÄ B³                OTIR


  03F³    EÄ 7¹                OUÔ   (C),Á
  03Fµ    EÄ 4±                OUÔ   (C),Â
  03F·    EÄ 4¹                OUÔ   (C),Ã
  03F¹    EÄ 5±                OUÔ   (C),Ä
  03FÂ    EÄ 5¹                OUÔ   (C),Å
  03FÄ    EÄ 6±                OUÔ   (C),È
  03FÆ    EÄ 6¹                OUÔ   (C),Ì
  040±    D³ 4¹                OUÔ   (NN),Á


  040³    EÄ AÂ                OUTÄ
  040µ    EÄ A³                OUTÉ
                        
                        
  040·    F±                   POĞ   AÆ
  040¸    C±                   POĞ   BÃ
  040¹    D±                   POĞ   DÅ
  040Á    E±                   POĞ   HÌ
  040Â    DÄ E±                POĞ   IØ
  040Ä    FÄ E±                POĞ   IÙ


  040Æ    Fµ                   PUSÈ   AÆ
  041°    Cµ                   PUSÈ   BÃ
  041±    Dµ                   PUSÈ   DÅ
  041²    Eµ                   PUSÈ   HÌ
  041³    DÄ Eµ                PUSÈ   IØ
  041µ    FÄ Eµ                PUSÈ   IÙ


  041·    CÂ 8¶                REÓ   0,(HL©
  041¹    DÄ CÂ 3¶ 8¶          REÓ   0,(IX+INDEX©
  041Ä    FÄ CÂ 3¶ 8¶          REÓ   0,(IY+INDEX©
  042±    CÂ 8·                REÓ   0,Á
  042³    CÂ 8°                REÓ   0,Â
  042µ    CÂ 8±                REÓ   0,Ã
  042·    CÂ 8²                REÓ   0,Ä
  042¹    CÂ 8³                REÓ   0,Å
  042Â    CÂ 8´                REÓ   0,È
  042Ä    CÂ 8µ                REÓ   0,Ì


  042Æ    CÂ 8Å                REÓ   1,(HL©
  043±    DÄ CÂ 3¶ 8Å          REÓ   1,(IX+INDEX©
  043µ    FÄ CÂ 3¶ 8Å          REÓ   1,(IY+INDEX©
  043¹    CÂ 8Æ                REÓ   1,Á
  043Â    CÂ 8¸                REÓ   1,Â
  043Ä    CÂ 8¹                REÓ   1,Ã
  043Æ    CÂ 8Á                REÓ   1,Ä
  044±    CÂ 8Â                REÓ   1,E
  044³    CÂ 8Ã                REÓ   1,È
  044µ    CÂ 8Ä                REÓ   1,Ì


  044·    CÂ 9¶                REÓ   2,(HL©Š  044¹    DÄ CÂ 3¶ 9¶          REÓ   2,(IX+INDEX©
  044Ä    FÄ CÂ 3¶ 9¶          REÓ   2,(IY+INDEX©
  045±    CÂ 9·                REÓ   2,Á
  045³    CÂ 9°                REÓ   2,Â
  045µ    CÂ 9±                REÓ   2,Ã
  045·    CÂ 9²                REÓ   2,Ä
  045¹    CÂ 9³                REÓ   2,Å
  045Â    CÂ 9´                REÓ   2,È
  045Ä    CÂ 9µ                REÓ   2,Ì


  045Æ    CÂ 9Å                REÓ   3,(HL©
  046±    DÄ CÂ 3¶ 9Å          REÓ   3,(IX+INDEX©
  046µ    FÄ CÂ 3¶ 9Å          REÓ   3,(IY+INDEX©
  046¹    CÂ 9Æ                REÓ   3,Á
  046Â    CÂ 9¸                REÓ   3,Â
  046Ä    CÂ 9¹                REÓ   3,Ã
  046Æ    CÂ 9Á                REÓ   3,Ä
  047±    CÂ 9Â                REÓ   3,Å
  047³    CÂ 9Ã                REÓ   3,È
  047µ    CÂ 9Ä                REÓ   3,Ì


  047·    CÂ A¶                REÓ   4,(HL©
  047¹    DÄ CÂ 3¶ A¶          REÓ   4,(IX+INDEX©
  047Ä    FÄ CÂ 3¶ A¶          REÓ   4,(IY+INDEX©
  048±    CÂ A·                REÓ   4,Á
  048³    CÂ A°                REÓ   4,Â
  048µ    CÂ A±                REÓ   4,Ã
  048·    CÂ A²                REÓ   4,Ä
  048¹    CÂ A³                REÓ   4,Å
  048Â    CÂ A´                REÓ   4,È
  048Ä    CÂ Aµ                REÓ   4,Ì


  048Æ    CÂ AÅ                REÓ   5,(HL©
  049±    DÄ CÂ 3¶ AÅ          REÓ   5,(IX+INDEX©
  049µ    FÄ CÂ 3¶ AÅ          REÓ   5,(IY+INDEX©
  049¹    CÂ AÆ                REÓ   5,Á
  049Â    CÂ A¸                REÓ   5,Â
  049Ä    CÂ A¹                REÓ   5,Ã
  049Æ    CÂ AÁ                REÓ   5,Ä
  04A±    CÂ AÂ                REÓ   5,Å
  04A³    CÂ AÃ                REÓ   5,È
  04Aµ    CÂ AÄ                REÓ   5,Ì


  04A·    CÂ B¶                REÓ   6,(HL©
  04A¹    DÄ CÂ 3¶ B¶          REÓ   6,(IX+INDEX©
  04AÄ    FÄ CÂ 3¶ B¶          REÓ   6,(IY+INDEX)
  04B±    CÂ B·                REÓ   6,Á
  04B³    CÂ B°                REÓ   6,Â
  04Bµ    CÂ B±                REÓ   6,Ã
  04B·    CÂ B²                REÓ   6,Ä
  04B¹    CÂ B³                REÓ   6,Å
  04BÂ    CÂ B´                REÓ   6,È
  04BÄ    CÂ Bµ                REÓ   6,Ì


  04BÆ    CÂ BÅ                REÓ   7,(HL©
  04C±    DÄ CÂ 3¶ BÅ          REÓ   7,(IX+INDEX©Š  04Cµ    FÄ CÂ 3¶ BÅ          REÓ   7,(IY+INDEX©
  04C¹    CÂ BÆ                REÓ   7,Á
  04CÂ    CÂ B¸                REÓ   7,Â
  04CÄ    CÂ B¹                REÓ   7,Ã
  04CÆ    CÂ BÁ                REÓ   7,Ä
  04D±    CÂ BÂ                REÓ   7,Å
  04D³    CÂ BÃ                REÓ   7,È
  04Dµ    CÂ BÄ                REÓ   7,Ì 


  04D·    C¹                   REÔ
  04D¸    D¸                   REÔ   Ã
  04D¹    F¸                   REÔ   Í
  04DÁ    D°                   REÔ   NÃ
  04DÂ    C°                   REÔ   NÚ
  04DÃ    F°                   REÔ   Ğ
  04DÄ    E¸                   REÔ   PÅ
  04DÅ    E°                   REÔ   PÏ
  04DÆ    C¸                   REÔ   Ú


  04E°    EÄ 4Ä                RETÉ
  04E²    EÄ 4µ                RETÎ


  04E´    CÂ 1¶                RÌ   (HL©
  04E¶    DÄ CÂ 3¶ 1¶          RÌ   (IX+INDEX©
  04EÁ    FÄ CÂ 3¶ 1¶          RÌ   (IY+INDEX©
  04EÅ    CÂ 1·                RÌ   Á
  04F°    CÂ 1°                RÌ   Â
  04F²    CÂ 1±                RÌ   Ã
  04F´    CÂ 1²                RÌ   Ä
  04F¶    CÂ 1³                RÌ   Å
  04F¸    CÂ 1´                RÌ   È
  04FÁ    CÂ 1µ                RÌ   Ì


  04FÃ    1·                   RLÁ


  04FÄ    CÂ 0¶                RLÃ   (HL©
  04FÆ    DÄ CÂ 3¶ 0¶          RLÃ   (IX+INDEX©
  050³    FÄ CÂ 3¶ 0¶          RLÃ   (IY+INDEX)
  050·    CÂ 0·                RLÃ   Á
  050¹    CÂ 0°                RLÃ   Â
  050Â    CÂ 0±                RLÃ   Ã
  050Ä    CÂ 0²                RLÃ   Ä
  050Æ    CÂ 0³                RLÃ   Å
  051±    CÂ 0´                RLÃ   È
  051³    CÂ 0µ                RLÃ   Ì


  051µ    0·                   RLCÁ


  051¶    EÄ 6Æ                RLÄ


  051¸    CÂ 1Å                RÒ   (HL©
  051Á    DÄ CÂ 3¶ 1Å          RÒ   (IX+INDEX©
  051Å    FÄ CÂ 3¶ 1Å          RÒ   (IY+INDEX©Š  052²    CÂ 1Æ                RÒ   Á
  052´    CÂ 1¸                RÒ   Â
  052¶    CÂ 1¹                RÒ   Ã
  052¸    CÂ 1Á                RÒ   Ä
  052Á    CÂ 1Â                RÒ   Å
  052Ã    CÂ 1Ã                RÒ   È
  052Å    CÂ 1Ä                RÒ   Ì


  053°    1Æ                   RRÁ


  053±    CÂ 0Å                RRÃ   (HL©
  053³    DÄ CÂ 3¶ 0Å          RRÃ   (IX+INDEX©
  053·    FÄ CÂ 3¶ 0Å          RRÃ   (IY+INDEX©
  053Â    CÂ 0Æ                RRÃ   Á
  053Ä    CÂ 0¸                RRÃ   Â
  053Æ    CÂ 0¹                RRÃ   Ã
  054±    CÂ 0Á                RRÃ   Ä
  054³    CÂ 0Â                RRÃ   Å
  054µ    CÂ 0Ã                RRÃ   È
  054·    CÂ 0Ä                RRÃ   Ì


  054¹    0Æ                   RRCÁ


  054Á    EÄ 6·                RRÄ


  054Ã    C·                   RSÔ   °
  054Ä    CÆ                   RSÔ   08È
  054Å    D·                   RSÔ   10È
  054Æ    DÆ                   RSÔ   18È
  055°    E·                   RSÔ   20È
  055±    EÆ                   RSÔ   28È
  055²    F·                   RSÔ   30H
  055³    FÆ                   RSÔ   38È


  055´    9Å                   SBÃ   A,(HL©
  055µ    DÄ 9Å 3¶             SBÃ   A,(IX+INDEX©
  055¸    FÄ 9Å 3¶             SBÃ   A,(IY+INDEX©
  055Â    9Æ                   SBÃ   A,Á
  055Ã    9¸                   SBÃ   A,Â
  055Ä    9¹                   SBÃ   A,Ã
  055Å    9Á                   SBÃ   A,Ä
  055Æ    9Â                   SBÃ   A,Å
  056°    9Ã                   SBÃ   A,È
  056±    9Ä                   SBÃ   A,Ì
  056²    DÅ 4¹                SBÃ   A,NÎ
  056´    EÄ 4²                SBÃ   HL,BÃ
  056¶    EÄ 5²                SBÃ   HL,DÅ
  056¸    EÄ 6²                SBÃ   HL,HÌ
  056Á    EÄ 7²                SBÃ   HL,SĞ


  056Ã    3·                   SCÆ


  056Ä    CÂ C¶                SEÔ   0,(HL©Š  056Æ    DÄ CÂ 3¶ C¶          SEÔ   0,(IX+INDEX©
  057³    FÄ CÂ 3¶ C¶          SEÔ   0,(IY+INDEX©
  057·    CÂ C·                SEÔ   0,Á
  057¹    CÂ C°                SEÔ   0,Â
  057Â    CÂ C±                SEÔ   0,Ã
  057Ä    CÂ C²                SEÔ   0,Ä
  057Æ    CÂ C³                SEÔ   0,Å
  058±    CÂ C´                SEÔ   0,È
  058³    CÂ Cµ                SEÔ   0,Ì


  058µ    CÂ CÅ                SEÔ   1,(HL©
  058·    DÄ CÂ 3¶ CÅ          SEÔ   1,(IX+INDEX©
  058Â    FÄ CÂ 3¶ CÅ          SEÔ   1,(IY+INDEX©
  058Æ    CÂ CÆ                SEÔ   1,Á
  059±    CÂ C¸                SEÔ   1,Â
  059³    CÂ C¹                SEÔ   1,Ã
  059µ    CÂ CÁ                SEÔ   1,Ä
  059·    CÂ CÂ                SEÔ   1,Å
  059¹    CÂ CÃ                SEÔ   1,È
  059Â    CÂ CÄ                SEÔ   1,Ì


  059Ä    CÂ D¶                SEÔ   2,(HL©
  059Æ    DÄ CÂ 3¶ D¶          SEÔ   2,(IX+INDEX©
  05A³    FÄ CÂ 3¶ D¶          SEÔ   2,(IY+INDEX©
  05A·    CÂ D·                SEÔ   2,Á
  05A¹    CÂ D°                SEÔ   2,Â
  05AÂ    CÂ D±                SEÔ   2,Ã
  05AÄ    CÂ D²                SEÔ   2,Ä
  05AÆ    CÂ D³                SEÔ   2,E
  05B±    CÂ D´                SEÔ   2,È
  05B³    CÂ Dµ                SEÔ   2,Ì


  05Bµ    CÂ DÅ                SEÔ   3,(HL©
  05B·    DÄ CÂ 3¶ DÅ          SEÔ   3,(IX+INDEX©
  05BÂ    FÄ CÂ 3¶ DÅ          SEÔ   3,(IY+INDEX©
  05BÆ    CÂ DÆ                SEÔ   3,Á
  05C±    CÂ D¸                SEÔ   3,Â
  05C³    CÂ D¹                SEÔ   3,Ã
  05Cµ    CÂ DÁ                SEÔ   3,Ä
  05C·    CÂ DÂ                SEÔ   3,Å
  05C¹    CÂ DÃ                SEÔ   3,È
  05CÂ    CÂ DÄ                SEÔ   3,Ì


  05CÄ    CÂ E¶                SEÔ   4,(HL©
  05CÆ    DÄ CÂ 3¶ E¶          SEÔ   4,(IX+INDEX©
  05D³    FÄ CÂ 3¶ E¶          SEÔ   4,(IY+INDEX©
  05D·    CÂ E·                SEÔ   4,Á
  05D¹    CÂ E°                SEÔ   4,Â
  05DÂ    CÂ E±                SEÔ   4,Ã
  05DÄ    CÂ E²                SEÔ   4,Ä
  05DÆ    CÂ E³                SEÔ   4,Å
  05E±    CÂ E´                SEÔ   4,È
  05E³    CÂ Eµ                SEÔ   4,Ì


  05Eµ    CÂ EÅ                SEÔ   5,(HL©
  05E·    DÄ CÂ 3¶ EÅ          SEÔ   5,(IX+INDEX©Š  05EÂ    FÄ CÂ 3¶ EÅ          SEÔ   5,(IY+INDEX©
  05EÆ    CÂ EÆ                SEÔ   5,Á
  05F±    CÂ E¸                SEÔ   5,Â
  05F³    CÂ E¹                SEÔ   5,Ã
  05Fµ    CÂ EÁ                SEÔ   5,Ä
  05F·    CÂ EÂ                SEÔ   5,Å
  05F¹    CÂ EÃ                SEÔ   5,È
  05FÂ    CÂ EÄ                SEÔ   5,Ì


  05FÄ    CÂ F¶                SEÔ   6,(HL©
  05FÆ    DÄ CÂ 3¶ F¶          SEÔ   6,(IX+INDEX©
  060³    FÄ CÂ 3¶ F¶          SEÔ   6,(IY+INDEX©
  060·    CÂ F·                SEÔ   6,Á
  060¹    CÂ F°                SEÔ   6,Â
  060Â    CÂ F±                SEÔ   6,Ã
  060Ä    CÂ F²                SEÔ   6,Ä
  060Æ    CÂ F³                SEÔ   6,Å
  061±    CÂ F´                SEÔ   6,È
  061³    CÂ Fµ                SEÔ   6,Ì


  061µ    CÂ FÅ                SEÔ   7,(HL©
  061·    DÄ CÂ 3¶ FÅ          SEÔ   7,(IX+INDEX©
  061Â    FÄ CÂ 3¶ FÅ          SEÔ   7,(IY+INDEX)
  061Æ    CÂ FÆ                SEÔ   7,Á
  062±    CÂ F¸                SEÔ   7,Â
  062³    CÂ F¹                SEÔ   7,Ã
  062µ    CÂ FÁ                SEÔ   7,Ä
  062·    CÂ FÂ                SEÔ   7,Å
  062¹    CÂ FÃ                SEÔ   7,È
  062Â    CÂ FÄ                SEÔ   7,Ì


  062Ä    CÂ 2¶                SLÁ   (HL©
  062Æ    DÄ CÂ 3¶ 2¶          SLÁ   (IX+INDEX©
  063³    FÄ CÂ 3¶ 2¶          SLÁ   (IY+INDEX©
  063·    CÂ 2·                SLÁ   Á
  063¹    CÂ 2°                SLÁ   Â
  063Â    CÂ 2±                SLÁ   Ã
  063Ä    CÂ 2²                SLÁ   Ä
  063Æ    CÂ 2³                SLÁ   Å
  064±    CÂ 2´                SLÁ   È
  064³    CÂ 2µ                SLÁ   Ì


  064µ    CÂ 2Å                SRÁ   (HL©
  064·    DÄ CÂ 3¶ 2Å          SRÁ   (IX+INDEX©
  064Â    FÄ CÂ 3¶ 2Å          SRÁ   (IY+INDEX©
  064Æ    CÂ 2Æ                SRÁ   Á
  065±    CÂ 2¸                SRÁ   Â
  065³    CÂ 2¹                SRÁ   Ã
  065µ    CÂ 2Á                SRÁ   Ä
  065·    CÂ 2Â                SRÁ   Å
  065¹    CÂ 2Ã                SRÁ   È
  065Â    CÂ 2Ä                SRÁ   Ì


  065Ä    CÂ 3Å                SRÌ   (HL©
  065Æ    DÄ CÂ 3¶ 3Å          SRÌ   (IX+INDEX©
  066³    FÄ CÂ 3¶ 3Å          SRÌ   (IY+INDEX©Š  066·    CÂ 3Æ                SRÌ   Á
  066¹    CÂ 3¸                SRÌ   Â
  066Â    CÂ 3¹                SRÌ   Ã
  066Ä    CÂ 3Á                SRÌ   Ä
  066Æ    CÂ 3Â                SRÌ   Å
  067±    CÂ 3Ã                SRÌ   È
  067³    CÂ 3Ä                SRÌ   Ì


  067µ    9¶                   SUÂ   (HL©
  067¶    DÄ 9¶ 3¶             SUÂ   (IX+INDEX©
  067¹    FÄ 9¶ 3¶             SUÂ   (IY+INDEX©
  067Ã    9·                   SUÂ   Á
  067Ä    9°                   SUÂ   Â
  067Å    9±                   SUÂ   Ã
  067Æ    9²                   SUÂ   Ä
  068°    9³                   SUÂ   Å
  068±    9´                   SUÂ   È
  068²    9µ                   SUÂ   Ì
  068³    D¶ 4¹                SUÂ   NÎ


  068µ    AÅ                   XOÒ   (HL©
  068¶    DÄ AÅ 3¶             XOÒ   (IX+INDEX©
  068¹    FÄ AÅ 3¶             XOÒ   (IY+INDEX©
  068Ã    AÆ                   XOÒ   Á
  068Ä    A¸                   XOÒ   Â
  068Å    A¹                   XOÒ   Ã
  068Æ    AÁ                   XOÒ   Ä
  069°    AÂ                   XOÒ   Å
  069±    AÃ                   XOÒ   È
  069²    AÄ                   XOÒ   Ì
  069³    EÅ 4¹                XOÒ   NÎ
.paŠ.tc APPENDIX C - Hitachi HD64180 Instructions .................. #
                 APPENDIX C - Hitachi HD64180 Instructions

Iî  additioî  tï  thå 6· Z8° mnemonicó anä thå µ  pseudï-opó  supporteä  bù 
vanillá Z8E¬ thå Z18° versioî alsï supportó thå 1° Hitaché HD6418°-specifiã 
mnemonics® 

Thå  Z8°  mnemonicó arå alì eitheò 2¬ ³ oò ´ characteró lonç  whilå  thå  µ 
pseudï-opó arå eitheò ³ oò ´ characteró long® Thå Hitaché  HD6418°-specifiã 
mnemonicó dï noô quitå fiô thió molä iî thaô threå oæ theí (onlù thå Japan
eså kno÷ why© arå µ characteró long.

Afteò  carefuì  deliberation¬  iô  waó decideä  tï  implemenô  theså  threå 
"rogues¢ aó ´ characteò mnemonicó foò 18E® Thå followinç tablå showó alì 1° 
Hitaché HD6418°-specifiã mnemonicó anä theiò 18Å equivalents® 

                 HITACHI |  Z8E  |  USAGE
              -----------+-------+------------------------------
                  IN0    |  IN0  |  IN0 g,(m)
                  OUT0   |  OUT0 |  OUT0 (m),g
                  TST    |  TST  |  TST g or TST (HL) or TST m
                  SLP    |  SLP  |  SLP
                  MLT    |  MLT  |  MLT ww
                  OTIM   |  OTIM |  OTIM
                  OTDM   |  OTDM |  OTDM
                  OTIMR  |  OIMR |  OIMR      \   5 character
                  OTDMR  |  ODMR |  ODMR       >  mnemonics
                  TSTIO  |  TSIO |  TSIO m    /   changed to 4

              Key to parameter symbols in USAGE column:

                  g     register A, B, C, D, E, È or Ì
                 ww     register pair BC, DE, HL or SP
                (m)     8 bit I/O port address
                  m     8 bit immediate data

Threå  tinù macroó caî accounô foò thå truncateä  mnemonicó iæ yoõ  wanô tï 
assemblå Z18° sourcå fileó generateä froí á Z8Å disassembly.

        oimò    macrï        odmò    macrï        tsiï    macrï   m
                otimò                otdmò                tstiï   m
                emdí                 endí                 endm
.paŠ.tc APPENDIØ Ä ­ SYSTEÍ MEMORÙ MAP ............................. #
                      APPENDIØ D ­ SYSTEÍ MEMORÙ MAP

               |||||||||||||||||||||||||||||||||||ü     FFFÆ
               |ü                                |ü
               |ü                                |ü
               |ü      CP/Í (BDOÓ anä BIOS©      |ü
               |ü                                |ü
               |ü                                |ü
               |||||||||||||||||||||||||||||||||||ü
               |ü                                |ü
               |ü    Z8Å (Approø 9.µ kBytes)     |ü
               |ü                                |ü
               |||||||||||||||||||||||||||||||||||ü
               |ü                                |ü
               |ü     OPTIONAÌ SYMBOÌ TABLÅ      |ü
               |ü                                |ü
               |||||||||||||||||||||||||||||||||||ü
               |ü                                |ü
               |ü                                |ü
               |ü                                |ü               
               |ü                                |ü
               |ü                                |ü
               |ü                                |ü
               |ü              TPÁ               |ü
               |ü                                |ü
               |ü                                |ü
               |ü                                |ü
               |ü                                |ü
               |ü                                |ü
               |ü                                |ü
               |ü                                |ü
               |||||||||||||||||||||||||||||||||||ü  --­ 010°
               |ü                                |ü
               |ü      PAGÅ ZERÏ  RESERVEÄ       |ü 
               |ü                                |ü 
               |||||||||||||||||||||||||||||||||||ü  __ß 000°

Sizå oæ residenô Z8Å debuç code:

Thå  residenô  sizå oæ Z8Å dependó oî thå configuration®  Thå  sizeó  showî 
herå  werå  obtaineä  foò aî ANSÉ console®  Wherå  á  seconä  terminaì  waó 
configureä theî iô waó eitheò á seconä ANSÉ screeî oò aî ADM3A.

     Z8° instructions¬ singlå screeî         9.2µ kBytes
     Z8° instructions¬ dual screenó          9.²µ kBytes
     HD64180/Z180¬ singlå screeî             9.µ  kBytes
     HD64180/Z180¬ dual screeîs              9.µ  kBytes

Foò comparison¬ ZSIÄ occupieó 8.µ kByteó anä SIÄ occupieó 6.7µ kBytes®  Foò 
thå extrá .7µ kBytå oveò ZSIÄ yoõ geô thå fulì screeî animateä display¬ thå 
use of a second display to separate debug output from program output, and á 
pretty good disassembler which can create a usable source file.  And for aî 
extrá .2µ kByteó yoõ caî dï alì oæ thå abovå oî á Z180/HD64180.

Nonå oæ thå numberó quoteä abovå includeó anù spacå occupieä bù symboló  sï 
thå comparisoî ió fair.
.paŠ.tc APPENDIX E - COMMAND SUMMARY ............................... #
                       APPENDIØ Ä ­ COMMANÄ SUMMARÙ

CMÄ         Descriptioî                      Argumentó

 Á        Inlinå  Assemblù             StartAddò 
 Â        Seô Breakpoinô               Addr1[,Pasó Countİ  [Addr2..AddrNİ
 Ã        Cleaò Breakpoinô             Addr±               [Addr2..AddrNİ
 Ä        Dumğ Memorù                  [StartAddrİ         [End/Countİ 
 Å        Examinå Memorù               StartAddò
 Æ        Finä                         StartAddò           MatchDatá
 Ç        Gï                           ExecutionAddò
 È        Displaù Symboì Tablå         [FirstSymbolİ
 É        Initializå commanä linå      [string1..stringNİ
 Ê        Fulì Screen/Animateä Debuç   [/İ [*İ [Addrİ      [Timeoutİ
 Ë        Seô Memorù Windo÷            StartAddò           [Sizeİ
 Ì        Loaä Filå                    FileNamå            [,Loaä Addressİ
 Í        Movå Memorù                  SourceStarô    SourceEnä    DestStarô
 Î        Outpuô tï Porô NÏ Pre-Reaä   [(İ PortAddò [)İ
 Ï        Outpuô Currenô Breakpointó
 Ğ        Exam/Modifù PS× (Flaç Reg©
 Ñ        Querù I/Ï Porô               [(İ PortAddò [)İ
 Ò        Examine/Modifù Registeró     RegSpecifieò
 Ó        Single-Steğ                  [/İ  [Countİ
 Ô        Selecô debuç terminaì        A oò C
 Õ        Writå Symboì Tablå Tï Disë   FileNamå    
 Ö        Verifù Memorù                SourceStarô     SourceEnä   DestStarô
 ×        Writå tï Disë                FileNamå        [StartAddrİ [EndAddrİ 
 Ø        Examinå Machinå Statå
 Ù        Filì Memorù                  FromAddò        ToAddò       Datá
 Ú        Disassemblå                  StartAddò       End/Counô    FileNamå
 ¿        Evaluatå expressioî          Expression
 ¾        Changå useò numbeò           UserNumber

[İ        Denoteó Optionaì Argumenô
[/İ       Dï Noô Tracå Subroutinå
[*İ       Dï Noô Tracå BDOÓ Calì  
[(İ [)İ   I/Ï Porô Monitoò Modå



               Z8Å   Copyrighô (c© 198´  AERO-SOFÔ
