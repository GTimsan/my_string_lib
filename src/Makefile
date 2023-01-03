CC=gcc
FLAGS=-std=c11 -Wall -Werror -Wextra
LIBS_MACOS= -L /usr/local/lib -lcheck
LIBS_LINUX= -lcheck -lm -lpthread -lrt -lsubunit -D_GNU_SOURCE
 
OS = $(shell uname)
ifeq ($(OS), Darwin)
OPEN=open gcov_report/index.html
LIBS=$(LIBS_MACOS)
# LEAK_CHECK = leaks --atExit -- ./test
endif
 
ifeq ($(OS), Linux)
OPEN=
LIBS=$(LIBS_LINUX)
# LEAK_CHECK = valgrind --leak-check=full --show-leak-kinds=all -s ./test
endif

all: gcov_report s21_string.a 

s21_string.o_with_gcov: s21_string.c s21_sprintf.c
	$(CC) $(FLAGS) -c s21_string.c s21_sprintf.c --coverage

s21_string.a_with_gcov: s21_string.o_with_gcov
	ar -rcs s21_string.a s21_string.o s21_sprintf.o

test: s21_test.check s21_string.a_with_gcov
	checkmk clean_mode=1 s21_test.check > s21_test.c
	$(CC) s21_test.c s21_string.a -o s21_test $(LIBS) --coverage
	./s21_test
	rm -f s21_test.gcda s21_test.gcno s21_test.c

s21_string.a: s21_string.c s21_sprintf.c
	$(CC) -c s21_string.c s21_sprintf.c
	ar -rcs s21_string.a s21_string.o s21_sprintf.o

gcov_report: test
	gcovr --html --html-details -o report.html
	open report.html

lcov: test
	lcov -t “test” -o lcov_test.info -c -d .
	genhtml -o report lcov_test.info
	open ./report/index.html


clean:
	rm -f *.out *.gcda *.gcno *.o *.a report.* s21_test test_with_gcov* *.gcov s21_test.c *.info
	rm -rf ./lcov_report report/
