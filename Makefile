ALL=rdd

CFLAGS=-g -O3 -Wall -Wno-switch -Wextra -Wwrite-strings
CPPFLAGS=-I./chacha-opt/app/include

DESTDIR=
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/share/man

all: $(ALL) chacha-opt/bin/chacha.lib

clean: FRC
	rm -f $(ALL) *.o
	make -C chacha-opt clean

rdd: rdd.o chacha-opt/bin/chacha.lib

chacha-opt/bin/chacha.lib:
	cd chacha-opt && ./configure && make

install: FRC all
	mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)/man1 $(DESTDIR)$(ZSHCOMPDIR)
	install -m0755 $(ALL) $(DESTDIR)$(BINDIR)
	install -m0644 $(ALL:=.1) $(DESTDIR)$(MANDIR)/man1

README: rdd.1
	mandoc -Tutf8 $< | col -bx >$@

FRC:
