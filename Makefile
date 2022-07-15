OS=$(shell uname -s)
ARCH=$(shell uname -m)
CFLAGS=-O -Wall -Wextra -fstack-protector-strong -pipe -MMD
ifeq ($(OS), Darwin)
	CFLAGS+=-arch $(ARCH) -mmacosx-version-min=12.0
endif
RM=rm -fvr
SRCS=main.c convert.c usage.c
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
TARGET=dos2unix

.PHONY: all clean dist-clean deps

all: $(TARGET)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)

dist-clean: clean
	$(RM) $(DEPS)

deps: $(DEPS)

test: all
	@test/run-test.sh

-include $(DEPS)
