OPTFLAGS=-g

include src/config.o

OBJ=src/sakura.o

CFLAGS=$(OPTFLAGS) $(VTEINC) -DICON_DIR=\"$(ICON_DIR)\"

all: evilvte

evilvte: $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS)

install:
	install -d $(bindir)
	install -m 755 $(PROG) $(bindir)
	install -d $(ICON_DIR_INSTALL)
	install -m 644 src/evilvte.png $(ICON_DIR_INSTALL)

clean: src/config.o
	rm -f $(PROG) src/*.o

src/config.o:
	./configure
