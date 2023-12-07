

C_COMPILER     = gcc --coverage
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit

%.o: %.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

test: datastrukturer/hash_table.c datastrukturer/linked_list.c webstore.o unit_test.o utils.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK)

debug: datastrukturer/hash_table.c datastrukturer/linked_list.c webstore.c unit_test.c utils.c
	gcc -Wall -g $^ -o test -lcunit
	@gdb ./a.out

memtest: test
	valgrind --leak-check=full -s ./test 

covtest: test
	@./test
	gcov -abcfu webstore.c 

clean:
	rm -f *.o *.gcno *.gcda *.gcov test

comp:
	gcc -Wall -pedantic -g datastrukturer/hash_table.c datastrukturer/linked_list.c webstore.c utils.c interface.c
