SRCS = tile.c hand.c
SRCS_TEST = test.c
TARGET = libmahjong.so
TARGET_TEST = test.elf

CC ?= gcc
CFLAGS ?= -O3 -Wall -Wextra -Wshadow -Werror -ffunction-sections -fdata-sections
LDFLAGS ?= -shared -fPIC

OBJS = $(patsubst %c,%o,$(filter %.c,$(SRCS)))
DEPS = $(patsubst %c,%d,$(filter %.c,$(SRCS)))
TEST_OBJS = $(patsubst %c,%o,$(filter %.c,$(SRCS_TEST)))
TEST_DEPS = $(patsubst %c,%d,$(filter %.c,$(SRCS_TEST)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TEST_TARGET): $(TEST_OBJS) $(TARGET)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET_TEST): $(TARGET)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)
-include $(TEST_DEPS)
