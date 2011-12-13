OPTFLAGS=-g

include src/config.o

OBJ=src/sakura.o

CFLAGS=$(OPTFLAGS) $(VTEINC) -DICON_DIR=\"$(ICON_DIR)\" -DEVILVTE_VERSION=\"$(EVILVTE_VERSION)\"

all: evilvte showvte

evilvte: $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS)

showvte:
	head -n 1 src/showvte.c > src/showvte
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
	install -m 644 misc/evilvte.1 $(mandir)
	install -d $(deskdir)
	install -m 644 misc/evilvte.desktop $(deskdir)

clean: src/config.o
	rm -f $(PROG) src/showvte src/*.o

src/config.o:
	./configure
