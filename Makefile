export INSTALLDIR=/usr/local

export CFLAGS:=-g
export LIBS:=
export CC:=gcc

MAJOR:=$(shell tla tree-version | sed -e "s+.*--.*--++g")
MINOR:=$(shell tla logs -r | head -1 | sed -e "s+.*-++g")


all: libRuneStone.so.$(MAJOR).$(MINOR) test

%.o: src/%.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

libRuneStone.so.$(MAJOR).$(MINOR): Struct.o
	$(CC) $(CFLAGS) $(LIBS) -fPIC -shared -Wl,-soname,libRuneStone.so.0.1 -o $@ $^

libRuneStone.so.$(MAJOR): libRuneStone.so.$(MAJOR).$(MINOR)
	ln -s $< $@

libRuneStone.so: libRuneStone.so.$(MAJOR)
	ln -s $< $@

.PHONY: clean
clean: test-clean
	-rm *.so.* *.so *.o *~ core* src/*~ RuneStone/*~

.PHONY: test
test: libRuneStone.so
	make -C Test

.PHONY: test-clean
test-clean:
	make -C Test clean

.PHONY: install-libs
install-libs: libRuneStone.so libRuneStone.so.$(MAJOR) libRuneStone.so.$(MAJOR).$(MINOR)
	cp -d $^ $(INSTALLDIR)/lib

.PHONY: install-headers
install-headers: RuneStone
	cp -dr $^ $(INSTALLDIR)/include

.PHONY: install
install: install-libs install-headers


.PHONY: uninstall
uninstall:
	rm -rf $(INSTALLDIR)/lib/{libRuneStone.so,libRuneStone.so.$(MAJOR),libRuneStone.so.$(MAJOR).$(MINOR)} $(INSTALLDIR)/include/RuneStone
