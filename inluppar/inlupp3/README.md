# Anton Eckervald & Oskar Blank

# Compiling

Go to inlupp3 directory
To compile, type "make" or "make all"
To run the program, type "make run"
To run test for the ast, type "make test"
To clear the classes directory, type "make clean"

# Error Handling

If user where to assign a number to a value, ex (2 = 3), Output is "Error: Numbers cannot be used as a variable name" and the program continues.
Same goes for different Errors the user might input, and the prompt types out the error and the program continues.
This is handled by a try-catch in the main event-loop in Calculator.java, where we try the code and catch all the possible errors.
