## inlupp 2 for IOOPM course 

inlupp 2 is a webstore backend built upon the datastructures from inlupp 1 as part of the course Imperativ och objektorienterad programmeringsmetodik (1DL221), HT2026.

# The project contains:

A logic file with the underlying logic for the webstore. 

A user interface file who builds the interface and takes care of input from the user.

A utils file with general helper functions used throughtout the code. 

A test file for the logic of the backend. 

# Requirements 

GCC
Make
CUnit
Valgrind (for memory checking)

# running 

To run and use the webstore: 

make run 


Run test and mem check logic: 

make test 
make valgrind_logic


Mem test for ui:

make valgrind_ui


Compile all: 

make all

# design choices 

The program is split up in two main parts logic and ui which serves two diffent purposes where logic handles the backend logic with the function list merchandise being the only exception whom prints out the list. The backend uses hash tables to store merchandise, shopping carts and shelf locations. Linked lists are used to manage the shelves associated with each item.

UI only prints out the interface and handles all the user input and pass it to logic.

# cleaning files

to remove compiled files use make clean