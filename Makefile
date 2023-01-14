SRCS = tile.c hand.c meld.c agari.c score.c mahjong.c
TEST_SRCS = test.c
TARGET = libmahjong.so
TEST_TARGET = test.elf

CC = gcc
CFLAGS = -O3 -Wall -Wextra -Wshadow -Werror -ffunction-sections -fdata-sections -fPIC
LDFLAGS = -shared

OBJS = $(patsubst %c,%o,$(filter %.c,$(SRCS)))
DEPS = $(patsubst %c,%d,$(filter %.c,$(SRCS)))
TEST_OBJS = $(patsubst %c,%o,$(filter %.c,$(TEST_SRCS)))
TEST_DEPS = $(patsubst %c,%d,$(filter %.c,$(TEST_SRCS)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TEST_TARGET): $(TEST_OBJS) $(TARGET)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)
-include $(TEST_DEPS)

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)
