OPTFLAGS=-g

include ../config.mak

OBJS=evilvte.o

CFLAGS=$(OPTFLAGS) $(VTEINC)

all: $(PROGS)

evilvte: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

install:
	install -d $(bindir)
	install -m 755 $(PROGS) $(bindir); \

clean:
	rm -f *.o $(PROGS)
