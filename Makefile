all:
	bazel test -c dbg //...
	bazel build -c opt //:diskimages
	for a in $$(dirname bazel-bin/arch/*/diskimage.img bazel-bin/arch/*/*/diskimage.img); do \
		f=$$(basename $$a); cp $$a/diskimage.img $$f.img; chmod +rw $$f.img; done

verbose:
	bazel test -s -c dbg //...
	bazel build -s -c opt //:diskimages

clean:
	bazel clean
