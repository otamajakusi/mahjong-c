SRCS = tile.c hand.c meld.c element.c agari.c score.c yaku.c mahjong.c util.c
TEST_SRCS = test.c test_tile.c test_meld.c test_hand.c test_element.c
TARGET = libmahjong.so
TEST_TARGET = test.elf

CC = gcc
CFLAGS = -O3 -Wall -Wextra -Wshadow -Werror -ffunction-sections -fdata-sections -fPIC
LDFLAGS = -shared

OBJS = $(patsubst %c,%o,$(filter %.c,$(SRCS)))
DEPS = $(patsubst %c,%d,$(filter %.c,$(SRCS)))
TEST_OBJS = $(patsubst %c,%o,$(filter %.c,$(TEST_SRCS)))
TEST_DEPS = $(patsubst %c,%d,$(filter %.c,$(TEST_SRCS)))

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TEST_TARGET): $(TEST_OBJS) $(TARGET)
	$(CC) -L. $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MP

-include $(DEPS)
-include $(TEST_DEPS)

test:
	LD_LIBRARY_PATH=. ./$(TEST_TARGET)

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)
