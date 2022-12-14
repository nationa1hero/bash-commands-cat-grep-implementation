all: test
test:
	cp ../../s21_grep .
	gcc -lcheck s21_*_test.c -o $@
	./$@

clean:
	ls | grep -v 'common\|s21_grep_test.[c|h]\|Makefile' | xargs $(RM)