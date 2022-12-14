.PHONY: all test check clean
override CFLAGS += -std=c11 -Wall -Wextra -Werror -Ofast
CC=gcc
RM=rm -rf
EXE=$(patsubst %.c,%,$(wildcard s21_*.c))
RUN_DBG=$(EXE) $(wildcard s21_*.c) $(wildcard s21_*.c)
# vpath %.c liba

ECHO=echo
ifeq ($(shell uname -s), Darwin)
ECHO=echo -e
endif

all: $(EXE)

%.o: %.c
	$(CC) -c $(CFLAGS) $^

# $(EXE).a: $(EXE).a($(patsubst liba/%.c,%.o,$(wildcard liba/*.c)))
# 	ranlib $@

# s21_%: s21_%.a
# 	$(CC) $@.a -o $@

$(EXE): $(EXE).o
	$(CC) $^ -o $@

test: $(EXE)
	cd tests/test_manhunte && $(MAKE)

check:
	@$(ECHO) "\033[0;32m"
	cp ../../materials/linters/CPPLINT.cfg . && python3 ../../materials/linters/cpplint.py --extensions=c *.c
	@$(ECHO) "\033[0;34m"
	cppcheck -q --enable=all --inconclusive --std=c11 --suppress=missingIncludeSystem .
	@$(ECHO) "\033[0;35m"
ifeq ($(shell uname -s), Darwin)
	bash -c "grep -e 'leaks for' <(CK_FORK=no leaks -q --atExit -- ./$(RUN_DBG))"
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./$(RUN_DBG)
	tail -n 1 RESULT_VALGRIND.txt
endif

clean:
	ls | grep -v 'liba\|tests\|s21_[a-z_]*[.][c|h]\|Makefile' | xargs $(RM)
	cd tests/test_manhunte && $(MAKE) clean
