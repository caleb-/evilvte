include src/config.o

ifeq ($(CFLAGS),)
	CFLAGS = -Os
endif

CFLAGS += $(OPTFLAGS) $(VTEINC)

evilvte: src/evilvte.h src/evilvte.o
	$(CC) $(LDFLAGS) src/evilvte.o $(LDLIBS) -o src/$(PROG)

prepare:
	rm -f src/custom.h

src/custom.h: prepare
	cp $(CONF_FILE) src/custom.h

src/evilvte.h: src/custom.h
	sh src/process.sh src/$(PROG)

strip: evilvte
	strip --remove-section=.comment --remove-section=.note src/$(PROG)

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
	sh src/install.sh

uninstall:
	sh src/uninstall.sh

clean: src/config.o
	rm -f src/$(PROG) src/showvte src/evilvte.o src/evilvte.h misc/evilvte.? src/custom.h

distclean: clean
	rm -f src/*.o src/install.sh src/uninstall.sh

src/config.o:
	./configure --quiet

.PHONY: evilvte prepare strip install installstrip uninstall clean distclean
