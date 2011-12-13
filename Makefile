all: evilvte

evilvte:
	$(MAKE) -C src $@

install:
	$(MAKE) -C src install

clean: config.mak
	$(MAKE) -C src clean
	rm -f config.mak

config.mak:
	./configure
