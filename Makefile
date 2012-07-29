UNAME := $(shell uname -s)

MAKEFILE=Makefile

all:
	cd src && make -f$(MAKEFILE)
	cp src/installer .

bootstrap:
	cd src && make -f$(MAKEFILE) bootstrap
	cp src/installer .

debug:
	cd src && make -f$(MAKEFILE) debug
	cp src/installer .

clean:
	cd src && make -f$(MAKEFILE) clean

realclean:
	cd src && make -f$(MAKEFILE) realclean

install:
	cd src && make -f$(MAKEFILE) install

uninstall:
	cd src && make -f$(MAKEFILE) uninstall
