CC=gcc
CPPFLAGS=-D_DEFAULT_SOURCE
CFLAGS=-W -Wall -Wextra -Wshadow

LD=ar
LDFLAGS=rcs
LIBS=

WITH_LOCK ?= 0
LOCK_FILENAME ?= $(shell mktemp -u)

ifneq ($(WITH_LOCK), 0)
	CPPFLAGS += -DWITH_LOCK -DLOCK_FILENAME='"$(LOCK_FILENAME)"'
endif

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)
BIN=libdesktop-utils.a

DEBUG ?= 0

ifneq ($(DEBUG), 0)
	CPPFLAGS += -DDEBUG=$(DEBUG)
	CFLAGS += -O0 -g
else
	CFLAGS += -O2
endif

.PHONY : all clean install uninstall


all : $(BIN)

$(BIN) : $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o : %.c $(HDR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean :
	rm -f $(OBJ) $(BIN)
