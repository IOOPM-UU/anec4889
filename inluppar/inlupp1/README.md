# Compiling

To compile go to the inlupp1 directory and type in "make test".
To run the tests, simply type "./test".
To check valgrind, type "make memtest".
To check coverage, type "make covtest".
To debug with gdb, type "make debug".
To clean, type "make clean".

To use freq-count:
type, "make freqtest"
To run the program, type "./freqtest [filename.txt]". E.g. "./freqtest freq-count/small.txt".
To check valgrind, type "make freqmem". (If you want to change the file, go under freqmem in the MAKEFILE and change the file)
To check coverage, type "make freqcov".
To debug with gdb, type "make freqdebug".
To clean, type "make clean".

To profile the program, run the program then type "gprof ./freqtest"

NOTE: If you struggle with any of the above, try "make clean" and go again.

# Design decisions

- See the function documentations for function pre-requisites
- Linked list have dummy node
- See code for more design decisions

# Initial Profiling Results

Using gprof

small.txt:

- Top 3 Ioopm-functions:

1. ioopm_iterator_has_next
2. ioopm_hash_table_has_key
3. ioopm_hash_table_insert

- Top 3 functions (in code):

1. key_eq
2. find_previous_entry_for_key
3. string_sum_hash

16k-words.txt:

- Top 3 Ioopm-functions:

1. ioopm_hash_table_has_key
2. ioopm_hash_table_insert
3. ioopm_hash_table_lookup

- Top 3 functions (in code):

1. key_eq
2. find_previous_entry_for_key
3. string_sum_hash

10k-words.txt:

- Top 3 Ioopm-functions:

1. ioopm_hash_table_has_key
2. ioopm_hash_table_insert
3. ioopm_hash_table_lookup

- Top 3 functions (in code):

1. key_eq
2. find_previous_entry_for_key
3. string_sum_hash

16k-words.txt:

- Top 3 Ioopm-functions:

1. ioopm_hash_table_has_key
2. ioopm_hash_table_insert
3. ioopm_hash_table_lookup

- Top 3 functions (in code):

1. key_eq
2. string_sum_hash
3. find_previous_entry_for_key
