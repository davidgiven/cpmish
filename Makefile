OBJDIR = .obj
LUA_5_1 = lua5.1

all: $(OBJDIR)/build.ninja
	@ninja -v -f $(OBJDIR)/build.ninja

clean:
	rm -rf $(OBJDIR)

lua-files = $(shell find . -name 'build*.lua')
$(OBJDIR)/build.ninja: build/ackbuilder.lua build/cpm.lua Makefile $(lua-files)
	@mkdir -p $(OBJDIR)
	@$(LUA_5_1) \
		build/ackbuilder.lua \
		build/build.lua \
		build/cpm.lua \
		build.lua \
		--ninja \
		OBJDIR=$(OBJDIR) \
		CC=gcc \
		AR=ar \
		> $@
