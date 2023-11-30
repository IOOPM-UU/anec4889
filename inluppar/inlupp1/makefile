

C_COMPILER     = gcc --coverage
C_OPTIONS      = -Wall -pedantic -g -pg
C_LINK_OPTIONS = -lm -pg
CUNIT_LINK     = -lcunit

%.o: %.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

test: hash_table.o linked_list.o hash_table_test.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK)

debug: hash_table.o linked_list.o hash_table_test.o
	gcc -Wall -g $^ -o test -lcunit
	@gdb ./test freq-count/small.txt

memtest: test
	valgrind --leak-check=full -s ./test

covtest: test
	@./test
	gcov -abcfu hash_table.o linked_list.o hash_table_test.o

freqtest: hash_table.o linked_list.o freq-count.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK)

freqdebug: hash_table.o linked_list.o freq-count.o
	gcc -Wall -g $^ -o freqtest -lcunit
	@gdb ./freqtest freq-count/small.txt

freqmem: freqtest
	valgrind --leak-check=full -s ./freqtest freq-count/10k-words.txt

freqcov: freqtest
	@./freqtest
	gcov -abcfu hash_table.o linked_list.o freq-count.o

clean:
	rm -f *.o *.gcno *.gcda *.gcov test
