OPTFLAGS=-g

include src/config.o

OBJ=src/sakura.o

CFLAGS=$(OPTFLAGS) $(VTEINC) -DICON_DIR=\"$(ICON_DIR)\" -DEVILVTE_VERSION=\"$(EVILVTE_VERSION)\"

ifeq ($(SUSE_DETECTED),TRUE)
CFLAGS += -DSUSE_DETECTED=1
endif

all: evilvte showvte

parsecfg:
	sh src/evilvte.c

evilvte: parsecfg $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS)

strip: all
	strip --remove-section=.comment --remove-section=.note $(PROG)

showvte:
	head -n 19 src/showvte.c | grep -v 'This is not an C program text' > src/showvte
	tail -n 7 src/showvte.c | head -n 6 >> src/showvte
	sh src/showvte.c >> src/showvte
	tail -n 1 src/showvte.c >> src/showvte
	sed 's/\t/ /g' src/config.h | tr -s ' ' ' ' | sed 's/^ //' | grep ^\#define | sed 's~/\*~\n~g' | grep -v \*\/ >> src/showvte
	chmod 755 src/showvte

install:
	install -d $(bindir)
	install -m 755 $(PROG) src/showvte $(bindir)
	install -d $(ICON_DIR_INSTALL)
	install -m 644 misc/evilvte.png misc/evilvte.xpm $(ICON_DIR_INSTALL)
	install -d $(mandir)
	install -m 644 misc/evilvte.1 misc/showvte.1 $(mandir)
	install -d $(deskdir)
	install -m 644 misc/evilvte.desktop $(deskdir)

installstrip: strip install

clean: src/config.o
	rm -f $(PROG) src/showvte src/*.o src/evilvte.h

src/config.o:
	./configure
