
CFLAGS = -Wall -Wextra -g
LDLIBS = -lcunit

PROFLAGS = -pg
TIME = time --6verbose
VALGRIND = valgrind --leak-check=full
COVERAGE = gcovr -r .





main: main.o logic.o datastruct.o ui.o utils.o 
	$(CC) $(CFLAGS) $^ -o $@

run: main 
	./main
	
valgrind_ui: main
	$(VALGRIND) ./main <webstoretest.txt 


logic: datastruct.c logic.c 
	$(CC) $(CFLAGS) -c logic.c

ui: datastruct.c ui.c
	$(CC) $(CFLAGS) -c ui.c 

#skapa .o filer
%.o: %.c %.h 
	$(CC) $(CFLAGS) -c $< -o $@

compile_test: logic.o ui.o testfile.o utils.o datastruct.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

test: compile_test
	./compile_test

valgrind: compile_test
	$(VALGRIND) ./compile_test 

all: compile_test

clean:
	rm -f *.o 
	rm -f compile_test
	rm -f *.gcda *.gcno *.gcov
	rm -f gmon.out profile.txt

.PHONY: clean valgrind test all 

