CC = gcc

TOP := $(dir $(lastword $(MAKEFILE_LIST)))
OUTPUT_DIRECTORY = $(TOP)/output
INCLUDE_DIRECTORY = $(TOP)/include
TEST_TARGET = $(OUTPUT_DIRECTORY)/all_tests

override CFLAGS += -std=c99 -Wall -Wshadow -Wstrict-prototypes \
		-Wunreachable-code -Wuninitialized -I \
		$(INCLUDE_DIRECTORY) -lpthread

SRC = $(wildcard $(TOP)/src/*.c)
TEST_SRC = $(wildcard $(TOP)/test/*.c)
SRCS = $(SRC) $(TEST_SRC)
SRC_OBJECTS = $(SRC:.c=.o)
TEST_OBJECTS = $(TEST_SRC:.c=.o)

.PHONY: clean print-%

all: $(TEST_TARGET)

$(OUTPUT_DIRECTORY):
	mkdir $(OUTPUT_DIRECTORY)

TAGS: $(SRCS)
	etags $(SRCS)

tests: CFLAGS += -g
tests: $(TEST_TARGET)

$(TEST_TARGET): $(SRC_OBJECTS) $(TEST_OBJECTS) $(OUTPUT_DIRECTORY)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(SRC_OBJECTS) $(TEST_OBJECTS) 

clean:
	@find -type f -iname '*~' -delete
	@rm -f $(SRC_OBJECTS) $(TEST_OBJECTS) $(UTIL_OBJECTS) $(PDU_OBJECTS)
	@rm -f $(TEST_TARGET)

# Allows you to quickly get the value of any makefile variable. 
# For example, suppose you want to know the value of a variable 
# called SOURCE_FILES. You'd just type:
#
#      make print-SOURCE_FILES
#
# More on: http://blog.jgc.org/2015/04/the-one-line-you-should-add-to-every.html
print-%: ; @echo $*=$($*)
