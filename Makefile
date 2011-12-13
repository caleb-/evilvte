include src/config.o

ifeq ($(CFLAGS),)
	CFLAGS = -Os
endif

ifeq ($(CC),)
	CC = gcc
endif

CFLAGS += $(OPTFLAGS) $(VTEINC) -DICON_DIR=\"$(ICON_DIR)\" -DEVILVTE_VERSION=\"$(EVILVTE_VERSION)\"

ifeq ($(SUSE_DETECTED),TRUE)
	CFLAGS += -DSUSE_DETECTED=1
endif

all: evilvte showvte manpage

prepare:
	rm -f src/custom.h

src/custom.h: prepare
	cp $(CONF_FILE) src/custom.h

src/evilvte.h: src/custom.h
	sh src/evilvte.sh src/$(PROG)

evilvte: src/evilvte.h src/sakura.o
	$(CC) $(LDFLAGS) src/sakura.o $(LDLIBS) -o src/$(PROG)

strip: all
	strip --remove-section=.comment --remove-section=.note src/$(PROG)

showvte: src/custom.h
	sh src/showvte.sh

manpage: src/custom.h
	sh src/manpage.sh

install:
	install -d $(bindir)
	install -m 755 src/$(PROG) src/showvte $(bindir)
	install -d $(ICON_DIR_INSTALL)
	install -m 644 misc/evilvte.png $(ICON_DIR_INSTALL)
	install -d $(mandir)
	install -m 644 misc/evilvte.1 misc/showvte.1 $(mandir)
	install -d $(deskdir)
	install -m 644 misc/evilvte.desktop $(deskdir)
	install -d $(GNOME_DEF_APP)
	install -m 644 misc/evilvte.xml $(GNOME_DEF_APP)

installstrip: strip
	install -d $(bindir)
	install -m 755 src/$(PROG) src/showvte $(bindir)
	install -d $(ICON_DIR_INSTALL)
	install -m 644 misc/evilvte.png $(ICON_DIR_INSTALL)
	install -d $(mandir)
	install -m 644 misc/evilvte.1 misc/showvte.1 $(mandir)
	install -d $(deskdir)
	install -m 644 misc/evilvte.desktop $(deskdir)
	install -d $(GNOME_DEF_APP)
	install -m 644 misc/evilvte.xml $(GNOME_DEF_APP)

uninstall:
	sh src/uninstall.sh

clean: src/config.o
	rm -f src/$(PROG) src/showvte src/sakura.o src/evilvte.h misc/evilvte.? src/custom.h

distclean: clean
	rm -f src/*.o src/uninstall.sh

src/config.o:
	./configure --quiet

.PHONY: all prepare evilvte strip showvte manpage install installstrip uninstall clean distclean
