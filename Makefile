# Project specific variables
BINARY=main
TESTCOVERAGEDIR=./test_coverage

TEST=tests
TESTS=$(wildcard $(TEST)/*.c)
TESTBINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%, $(TESTS))

CODEDIRS=src
INCDIRS=include

# Compiler and options
CC=gcc
OPT=-O0

# Dependency flags
DEPFLAGS=-MP -MD

# Compiler flags
CFLAGS=-Wall -Wextra -Werror -Wpedantic -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS) -fPIC -fprofile-arcs -ftest-coverage

# Source and object files
CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.c)) mongoose.c
OBJECTS=$(patsubst %.c,%.o,$(filter-out $(TESTS), $(CFILES)))
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

# Default target
all: $(BINARY)

# Link the main binary
$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ -lgcov -lm

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build files
clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES) $(TESTBINS) $(TESTCOVERAGEDIR) *.gcda *.gcno *.gcov coverage.info
	rm -rf tests/bin/
	rm -rf src/*.gcda src/*.gcno

# Distribution package
distribute: clean
	tar zcvf dist.tgz *

# Git status and diff
diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

# Compile test binaries
$(TEST)/bin/%: $(TEST)/%.c $(OBJECTS)
	mkdir -p $(TEST)/bin
	$(CC) $(CFLAGS) -o $@ $^ -lcriterion --coverage -lm

# Run tests
test: $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

# Test coverage
test_coverage:
	mkdir -p test_coverage
	lcov --capture --directory . --output-file test_coverage/coverage.info
	lcov --remove test_coverage/coverage.info '/usr/*' --output-file test_coverage/coverage.info
	genhtml test_coverage/coverage.info --output-directory test_coverage/report

# Include dependency files
-include $(DEPFILES)

# Phony targets
.PHONY: all clean distribute diff test test_coverage

