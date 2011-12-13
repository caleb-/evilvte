OPTFLAGS=-g

include ../config.mak

OBJS=evilvte.o

CFLAGS=$(OPTFLAGS) $(VTEINC) -DEVILVTE_VERSION=\"$(EVILVTE_VERSION)\"

all: $(PROGS)

evilvte: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

evilvtab: evilvtab.o
	$(CC) -o $@ evilvtab.o $(LDFLAGS)

install:
	install -d $(bindir)
	install -m 755 $(PROGS) $(bindir); \

clean:
	rm -f *.o $(PROGS)
