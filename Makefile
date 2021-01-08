.PHONY: clean
CC=clang
CFLAGS=-I. -g -Wall --std=gnu11 -fpic
SRCS=wheel.c
OBJS=$(patsubst %.c,%.o,$(SRCS))
PROGRAM=wheel

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(PROGRAM)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lib%.so: $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)
