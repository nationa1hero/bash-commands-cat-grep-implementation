all: test
test:
	cp ../../s21_grep{,.c} .
	touch  s21_grep.h
	./t_grep.sh

clean:
	ls | grep -v 't_grep.sh\|s21_grep_patterns.txt\|Makefile' | xargs $(RM)