CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/obj/%.o)

STATIC_LIB = build/lib/scrnc.a
SHARED_LIB = build/lib/scrnc.so

.PHONY: all clean

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB): $(OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

$(SHARED_LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -shared -o $@ $^

build/obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
