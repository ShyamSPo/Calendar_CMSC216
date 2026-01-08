cc = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra
PUBTESTS = public01 public02 public03 public04 public05 public06 public07


.PHONY: all clean

all: $(PUBTESTS) student_tests

clean:
	rm -f *.o $(PUBTESTS) student_tests a.out


public01: public01.o calendar.o
	gcc -o public01 public01.o calendar.o

public02: public02.o calendar.o
	gcc -o public02 public02.o calendar.o

public03: public03.o calendar.o
	gcc -o public03 public03.o calendar.o

public04: public04.o calendar.o
	gcc -o public04 public04.o calendar.o

public05: public05.o calendar.o
	gcc -o public05 public05.o calendar.o

public06: public06.o calendar.o
	gcc -o public06 public06.o calendar.o

public07: public07.o calendar.o
	gcc -o public07 public07.o calendar.o

student_tests: student_tests.o calendar.o
	gcc -o student_tests student_tests.o calendar.o


public01.o: public01.c
	gcc -c public01.c

public02.o: public02.c
	gcc -c public02.c

public03.o: public03.c
	gcc -c public03.c

public04.o: public04.c
	gcc -c public04.c

public05.o: public05.c
	gcc -c public05.c

public06.o: public06.c
	gcc -c public06.c

public07.o: public07.c
	gcc -c public07.c

student_tests.o: student_tests.c
	gcc -c student_tests.c


calendar.o: calendar.c calendar.h event.h
	gcc -c calendar.c