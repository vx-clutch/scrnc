srcdir = .
bindir = $(exec_prefix)/bin

CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC
LDFLAGS =

SRCS = $(wildcard $(srcdir)/src/**/*.c)
HEADERS = $(wildcard $(srcdir)/src/**/*.h)
OBJS = $(addprefix obj/,$(patsubst $(srcdir)/src/%,obj/%,$(patsubst %.c,%.o,$(notdir $(SRCS)))))

STATIC_LIB = lib/libscrnc.a
SHARED_LIB = lib/libscrnc.so
ALL_LIBS = $(STATIC_LIB) $(SHARED_LIB)

-include config.mak

ifeq ($(wildcard config.mak),)
all:
	@echo "File config.mak not found, run configure"
	@exit 1
else

all: clean lib obj $(ALL_LIBS)

obj:
	mkdir -p $@
lib:
	mkdir -p $@

obj/%.o: $(srcdir)/src/**/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^

$(SHARED_LIB): $(OBJS)
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS)

endif

install:
	@echo "NOT IMPL"
	exit 1

uninstall:
	@echo "NOT IMPL"
	exit 1

clean:
	rm -rf obj lib

dist-clean: clean
	rm config.mak

.PHONY: all clean dist-clean install uninstall

