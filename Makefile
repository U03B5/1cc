CC=gcc
MAIN=1cc
SRC_DIR=src
HEADER_DIR=include
HEADERS=$(wildcard include/$(MAIN)/*.h)
CFLAGS=-Wall -D_DEFAULT_SOURCE $(DEBUG) -fvisibility=hidden -std=c11 -Wno-format -Wno-missing-braces -O2 -I include
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:%.c=%.o)

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN)

debug:
	$(MAKE) all DEBUG=-DDEBUG

clean:
	rm -rf $(MAIN) $(OBJS) $(BIN_DIR)
	find . -name "*~" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;

format:
	clang-format -i $(SRCS) $(HEADERS)

.PHONY: debug clean format
