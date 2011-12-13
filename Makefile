include src/config.o

ifeq ($(CFLAGS),)
	CFLAGS += -Os
endif

CFLAGS += $(OPTFLAGS) $(VTEINC) -DICON_DIR=\"$(ICON_DIR)\" -DEVILVTE_VERSION=\"$(EVILVTE_VERSION)\"
CFLAGS += -Wall -Wextra -DG_DISABLE_DEPRECATED -DG_DISABLE_SINGLE_INCLUDES -DGDK_DISABLE_DEPRECATED -DGDK_DISABLE_SINGLE_INCLUDES -DGTK_DISABLE_DEPRECATED -DGTK_DISABLE_SINGLE_INCLUDES

ifeq ($(SUSE_DETECTED),TRUE)
	CFLAGS += -DSUSE_DETECTED=1
endif

ifeq ($(EVILVTE_CUSTOM),TRUE)
	CFLAGS += -DEVILVTE_CUSTOM=1
endif

all: evilvte showvte misc/evilvte.1

src/custom.h:
	cp $(CONF_FILE) src/custom.h

src/evilvte.h: src/custom.h
	sh src/evilvte.sh $(CONF_FILE) $(PROG)

evilvte: src/evilvte.h src/sakura.o
	$(CC) $(LDFLAGS) src/sakura.o $(LDLIBS) -o $(PROG)

strip: all
	strip --remove-section=.comment --remove-section=.note $(PROG)

showvte:
	grep ^\# src/showvte.sh > src/showvte
	tail -n 8 src/showvte.sh | grep -v ^grep >> src/showvte
	sh src/showvte.sh $(CONF_FILE) >> src/showvte
	grep ^grep src/showvte.sh >> src/showvte
	chmod 755 src/showvte

misc/evilvte.1:
	sh src/manpage.sh $(CONF_FILE)

install:
	install -d $(bindir)
	install -m 755 $(PROG) src/showvte $(bindir)
	install -d $(ICON_DIR_INSTALL)
	install -m 644 misc/evilvte.png $(ICON_DIR_INSTALL)
	install -d $(mandir)
	install -m 644 misc/evilvte.1 misc/showvte.1 $(mandir)
	install -d $(deskdir)
	install -m 644 misc/evilvte.desktop $(deskdir)

installstrip: strip
	install -d $(bindir)
	install -m 755 $(PROG) src/showvte $(bindir)
	install -d $(ICON_DIR_INSTALL)
	install -m 644 misc/evilvte.png $(ICON_DIR_INSTALL)
	install -d $(mandir)
	install -m 644 misc/evilvte.1 misc/showvte.1 $(mandir)
	install -d $(deskdir)
	install -m 644 misc/evilvte.desktop $(deskdir)

clean: src/config.o
	rm -f $(PROG) src/showvte src/sakura.o src/evilvte.h misc/evilvte.? src/custom.h

distclean: clean
	rm -f src/*.o

src/config.o:
	./configure

.PHONY: all evilvte strip showvte install installstrip clean distclean
