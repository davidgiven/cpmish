def _removeextension(s):
    i = s.rindex(".")
    return s[:i]

def _compile_ack_files(ctx, srcfiles, headers):
    ofiles = []
    for src in srcfiles:
        n = src.basename[:src.basename.rindex(".")]
        o = ctx.actions.declare_file(ctx.label.name + "/" + n + ".o")
        h = [ header.dirname for header in headers ]

        args = ctx.actions.args()
        args.add("-m" + ctx.attr.arch)
        args.add_all(ctx.attr.cflags)
        args.add_all(h, format_each="-I%s")
        args.add("-c")
        args.add("-O3")
        args.add("-o", o)
        args.add(src)

        ctx.actions.run_shell(
            mnemonic = "AckCompile",
            command = "ack $@",
            arguments = [args],
            inputs = [src] + headers,
            outputs = [o],
            use_default_shell_env = True,
        )

        ofiles += [o]

    return ofiles

AckLibrary = provider(
    "Ack Library fields",
    fields = {
        "public_headers": "depset of header Files from transitive dependencies.",
        "files_to_link": "depset of Files from compilation.",
    }
)

def _ack_library_impl(ctx):
    sources = [f for f in ctx.files.srcs if f.extension == "c"]
    private_headers = [f for f in ctx.files.srcs if f.extension == "h"]
    public_headers = ctx.files.hdrs + [dep[AckLibrary].public_headers for dep in ctx.attr.deps]

    ofiles = _compile_ack_files(ctx, sources, private_headers + public_headers)

    output_file = ctx.actions.declare_file(ctx.label.name + ".a")

    args = ctx.actions.args()
    args.add("qc")
    args.add(output_file)
    args.add_all(ofiles)

    ctx.actions.run_shell(
        mnemonic = "AckLibrary",
        command = "aal $@",
        arguments = [args],
        inputs = ofiles,
        outputs = [output_file],
        use_default_shell_env = True,
    )

    return [
        AckLibrary(
            public_headers = depset(public_headers),
            files_to_link = depset(
                [output_file],
                transitive = [
                    dep[AckLibrary].files_to_link for dep in ctx.attr.deps
                ],
            ),
        ),
        DefaultInfo(files = depset([output_file])),
    ]

ack_library = rule(
    implementation = _ack_library_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = True),
        "cflags": attr.string_list(),
        "arch": attr.string(default = "cpm"),
        "deps": attr.label_list(providers = [AckLibrary]),
        "hdrs": attr.label_list(allow_files = True),
    }
)

def _ack_binary_impl(ctx):
    sources = [f for f in ctx.files.srcs if f.extension == ".c"]
    private_headers = [f for f in ctx.files.srcs if f.extension == ".h"]
    public_headers = [f for dep in ctx.attr.deps for f in dep[AckLibrary].public_headers.to_list()]
    files_to_link = [f for dep in ctx.attr.deps for f in dep[AckLibrary].files_to_link.to_list()]

    ofiles = _compile_ack_files(ctx, ctx.files.srcs, private_headers + public_headers)

    output_file = ctx.actions.declare_file(ctx.label.name + ".exe")

    args = ctx.actions.args()
    args.add("-m" + ctx.attr.arch)
    args.add("-." + ctx.attr.lang)
    args.add("-o", output_file)
    args.add_all(ofiles)
    args.add_all(files_to_link)

    ctx.actions.run_shell(
        mnemonic = "AckBinary",
        command = "ack $@",
        arguments = [args],
        inputs = ofiles + files_to_link,
        outputs = [output_file],
        use_default_shell_env = True,
    )

    return [
        DefaultInfo(files = depset([output_file])),
    ]

ack_binary = rule(
    implementation = _ack_binary_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = True),
        "cflags": attr.string_list(),
        "arch": attr.string(default = "cpm"),
        "deps": attr.label_list(providers = [AckLibrary]),
        "lang": attr.string(default = "c"),
    }
)
        
# vim: ts=4 sw=4 et

