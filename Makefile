ATARGET = lib/libscrnc.a
STARGET = lib/libscrnc.so
CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC -I.

SRCS = $(wildcard src/*.c)
OBJS = $(addprefix obj/,$(patsubst $(srcdir)/src/%,obj/%,$(patsubst %.c,%.o,$(notdir $(SRCS)))))

.PHONY: all clean lib

lib: $(clean)
	mkdir $@

obj: $(clean)
	mkdir $@

all: $(ATARGET) $(STARGET)

$(ATARGET): lib $(OBJS)
	ar rcs $@ $^

$(STARGET): lib $(OBJS)
	$(CC) -shared -o $@ $^

%.o: obj %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: lib obj
	rm -rf $(OBJS) $(STARGET) $(ATARGET) lib obj
