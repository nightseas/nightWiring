######################################################
#             Makefile for nightWiring lib           #
#        By Xiaohai Li  (haixiaolee@gmail.com)       #
######################################################

VERSION=$(shell cat ../VERSION)
DESTDIR?=/usr
PREFIX?=/local

LDCONFIG?=ldconfig

ifneq ($V,1)
Q ?= @
endif

STATIC=libnightWiring.a
DYNAMIC=libnightWiring.so.$(VERSION)

#DEBUG	= -g -O0
DEBUG	= -O2
CC	= gcc
#CC	= arm-linux-gnueabihf-gcc-4.8
INCLUDE	= -I.
DEFS	= -D_GNU_SOURCE
CFLAGS	= $(DEBUG) $(DEFS) -Wformat=2 -Wall -Winline $(INCLUDE) -pipe -fPIC

LIBS    =

###############################################################################

SRC	=		nightWiring.c								\
			nightWiringSerial.c							\
			nightWiringPriority.c nightWiringThread.c	\
			nightWiringSPI.c nightWiringI2C.c			\
			nightWiringGPIO.c nightWiringShift.c        \
			nightWiringSoftPwm.c nightWiringSoftTone.c 

HEADERS =	nightWiring.h								\
			nightWiringSerial.h							\
			nightWiringSPI.h nightWiringI2C.h			\
			nightWiringGPIO.h nightWiringShift.h        \
			nightWiringSoftPwm.h nightWiringSoftTone.h


OBJ	=	$(SRC:.c=.o)

all:		$(DYNAMIC)

static:		$(STATIC)

$(STATIC):	$(OBJ)
	$Q echo "[Link (Static)]"
	$Q ar rcs $(STATIC) $(OBJ)
	$Q ranlib $(STATIC)

$(DYNAMIC):	$(OBJ)
	$Q echo "[Link (Dynamic)]"
	$Q $(CC) -shared -Wl,-soname,libnightWiring.so$(nightWiring_SONAME_SUFFIX) -o libnightWiring.so.$(VERSION) -lpthread $(OBJ)

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@


.PHONY:	clean
clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) $(OBJ_I2C) *~ core tags Makefile.bak libnightWiring.*

.PHONY:	tags
tags:	$(SRC)
	$Q echo [ctags]
	$Q ctags $(SRC)


.PHONY:	install
install:	$(DYNAMIC)
	$Q echo "[Install Headers]"
	$Q install -m 0755 -d						$(DESTDIR)$(PREFIX)/include
	$Q install -m 0644 $(HEADERS)					$(DESTDIR)$(PREFIX)/include
	$Q echo "[Install Dynamic Lib]"
	$Q install -m 0755 -d						$(DESTDIR)$(PREFIX)/lib
	$Q install -m 0755 libnightWiring.so.$(VERSION)			$(DESTDIR)$(PREFIX)/lib/libnightWiring.so.$(VERSION)
	$Q ln -sf $(DESTDIR)$(PREFIX)/lib/libnightWiring.so.$(VERSION)	$(DESTDIR)/lib/libnightWiring.so
	$Q $(LDCONFIG)

.PHONY:	install-static
install-static:	$(STATIC)
	$Q echo "[Install Headers]"
	$Q install -m 0755 -d						$(DESTDIR)$(PREFIX)/include
	$Q install -m 0644 $(HEADERS)					$(DESTDIR)$(PREFIX)/include
	$Q echo "[Install Static Lib]"
	$Q install -m 0755 -d						$(DESTDIR)$(PREFIX)/lib
	$Q install -m 0755 libnightWiring.a					$(DESTDIR)$(PREFIX)/lib

.PHONY:	install-deb
install-deb:	$(DYNAMIC)
	$Q echo "[Install Headers: deb]"
	$Q install -m 0755 -d							~/nightWiring/debian-template/nightWiring/usr/include
	$Q install -m 0644 $(HEADERS)						~/nightWiring/debian-template/nightWiring/usr/include
	$Q echo "[Install Dynamic Lib: deb]"
	install -m 0755 -d							~/nightWiring/debian-template/nightWiring/usr/lib
	install -m 0755 libnightWiring.so.$(VERSION)				~/nightWiring/debian-template/nightWiring/usr/lib/libnightWiring.so.$(VERSION)
	ln -sf ~/nightWiring/debian-template/nightWiring/usr/lib/libnightWiring.so.$(VERSION)	~/nightWiring/debian-template/nightWiring/usr/lib/libnightWiring.so

.PHONY:	uninstall
uninstall:
	$Q echo "[UnInstall]"
	$Q cd $(DESTDIR)$(PREFIX)/include/ && rm -f $(HEADERS)
	$Q cd $(DESTDIR)$(PREFIX)/lib/     && rm -f libnightWiring.*
	$Q $(LDCONFIG)


.PHONY:	depend
depend:
	makedepend -Y $(SRC) $(SRC_I2C)


# Basic Modules
nightWiring.o: nightWiring.h

# GPIO Independent Modules
wiringSerial.o: nightWiringSrial.h
nightWiringSPI.o: nightWiring.h nightWiringSPI.h
nightWiringI2C.o: nightWiring.h nightWiringI2C.h

# System Dependent Modules
nightWiringPriority.o: nightWiring.h
nightWiringThread.o: nightWiring.h

# GPIO Dependent Modules
nightWiringGPIO.o: nightWiringGPIO.h nightWiring.h
wiringShift.o: nightWiring.h nightWiringGPIO.h nightWiringShift.h
softPwm.o: nightWiring.h nightWiringGPIO.h softPwm.h
softTone.o: nightWiring.h nightWiringGPIO.h softTone.h
