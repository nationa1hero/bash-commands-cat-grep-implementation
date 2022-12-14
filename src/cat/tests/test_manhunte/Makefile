all: test
test:
	cp ../../s21_cat .
	./t_cat.sh

clean:
	ls | grep -v 't_cat.sh\|bytes.txt\|Makefile' | xargs $(RM)