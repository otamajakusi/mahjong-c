SRCS = tile.c hand.c meld.c element.c agari.c score.c yaku.c fu.c mahjong.c util.c
TEST_SRCS = test.c test_tile.c test_meld.c test_hand.c test_element.c test_agari.c test_score.c test_mahjong.c
EXAMPLE_SRCS = example.c
TARGET = libmahjong.so
TEST_TARGET = test.elf
EXAMPLE_TARGET = example.elf

CC = gcc
CFLAGS = -O3 -Wall -Wextra -Wshadow -Werror -ffunction-sections -fdata-sections -fPIC
LDFLAGS = -shared

OBJS = $(patsubst %c,%o,$(filter %.c,$(SRCS)))
DEPS = $(patsubst %c,%d,$(filter %.c,$(SRCS)))
TEST_OBJS = $(patsubst %c,%o,$(filter %.c,$(TEST_SRCS)))
TEST_DEPS = $(patsubst %c,%d,$(filter %.c,$(TEST_SRCS)))
EXAMPLE_OBJS = $(patsubst %c,%o,$(filter %.c,$(EXAMPLE_SRCS)))
EXAMPLE_DEPS = $(patsubst %c,%d,$(filter %.c,$(EXAMPLE_SRCS)))

all: $(TARGET) $(TEST_TARGET) $(EXAMPLE_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TEST_TARGET): $(TEST_OBJS) $(TARGET)
	$(CC) -L. $^ -o $@

$(EXAMPLE_TARGET): $(EXAMPLE_OBJS) $(TARGET)
	$(CC) -L. $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MP

-include $(DEPS)
-include $(TEST_DEPS)
-include $(EXAMPLE_DEPS)

test: $(TEST_TARGET)
	LD_LIBRARY_PATH=. ./$(TEST_TARGET)

example: $(EXAMPLE_TARGET)
	@LD_LIBRARY_PATH=. ./$(EXAMPLE_TARGET) 2> /dev/null

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(EXAMPLE_OBJS) $(DEPS) $(TEST_DEPS) $(EXAMPLE_DEPS) $(TARGET) $(TEST_TARGET) $(EXAMPLE_TARGET)
