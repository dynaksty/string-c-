CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -std=c11
OFLAGS=-Wall -Wextra -Werror -std=c11
GCOVFLAGS=-fprofile-arcs -ftest-coverage

SOURCE_DIR = .
TEST_DIR = ./tests
BUILD_DIR = ../build

TARGET_NAME = string.a
TEST_NAME = unittest

COVERAGE_NAME = coverage.info

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SOURCES))

all: $(TARGET_NAME)

$(TARGET_NAME): $(BUILD_DIR) $(OBJECTS)
	ar -rs $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

###
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) $(shell pkg-config --cflags check) $< -o $@

test: unittest
	./$(TEST_NAME)

leaks_test: unittest
	@bash ./scripts/leak_test.sh
	
unittest: install_dependence $(BUILD_DIR) $(TARGET_NAME) $(TEST_OBJECTS)
	$(CC) $(OFLAGS) $(TEST_OBJECTS) $(shell pkg-config --libs check) -L . $(TARGET_NAME) -o $(TEST_NAME)

gcov_report: clean install_dependence install_coverage_dependence $(BUILD_DIR) $(TEST_OBJECTS)
	$(CC) $(OFLAGS) $(GCOVFLAGS) $(TEST_OBJECTS) $(SOURCES) $(shell pkg-config --libs check) -L . -o $(TEST_NAME)
	./$(TEST_NAME)
	lcov -t "gcov_report" -o $(COVERAGE_NAME) -c -d $(SOURCE_DIR) 
	genhtml -q -s -o ./report $(COVERAGE_NAME)
	open ./report/index.html

install_dependence:
	@bash ./scripts/install_pkg_config.sh
	@bash ./scripts/install_check.sh

install_coverage_dependence:
	bash ./scripts/install_gcov.sh
##

check:
	clang-format -n *.c
	clang-format -n *.h
	clang-format -n */*.c

clang:
	clang-format -i *.c
	clang-format -i *.h
	clang-format -i */*.c
	@rm -rf tests/*temp*

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(SOURCE_DIR)/$(TARGET_NAME)
	@rm -rf $(TEST_NAME)
	@rm -rf $(SOURCE_DIR)/report
	@rm -rf $(SOURCE_DIR)/$(COVERAGE_NAME)
	@rm -rf tests/*temp*
	@find . -name '*.gcov' -delete
	@find . -name '*.gcda' -delete
	@find . -name '*.gcno' -delete