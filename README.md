# C Programs
A Survey of C Programs
## Hello World
The most basic C program. Prints Hello World! to console. In order to build and run:
```bash
$ make
$ ./hello
```

## Calculator
![CalculatorDemo](Media/CalculatorDemo.gif "Demo of Calculator program")  
A calculator program that works much like many of the simple calculators you can find on your phone. Run the program and type h to get a list of available operations. Pass sample_input.txt to test the code as seen in the demo. Use the following command to verify that the outputs match.
```bash
$ make
$ ./calc < sample_input.txt > temp_output.txt
$ cmp -s temp_output.txt sample_output.txt && echo 'Files are identical' || echo 'Files have differences, use diff to view them'
```

## GPS
![GPSDemo](Media/GPSDemo.gif "Demo of GPS program")  
A program that takes the inputs of a series of points (simulated GPS data) and computes the way points, distance traveled, and speed information for the specific delta time with regards to an internally defined set of roads.
```bash
$ make
$ ./tracker < sample_input.txt > temp_output.txt
$ cmp -s temp_output.txt sample_output.txt && echo 'Files are identical' || echo 'Files have differences, use diff to view them'
```
### Example explanation
![GPSDesmos](Media/GPSDesmos.png "Annotated Desmos plot of GPS point fixes")  
The above plot shows the example two roads, y = -x (road one) and y = x (road two). The input gps fix points are attributed to their respective road through a normalization process by finding the closest road to that point. The path of the car is then tracked and statistics are computed.

## Sudoku
![SudokuDemo](Media/SudokuDemo.gif "Demo of Sudoku program")  
A program that allows the user to play a game of sudoku.
```bash
$ make
$ ./tester # check test cases
$ ./terminal # play sudoku (use the provided files easy.sko, med.sko, hard.sko)
```

# Addendum: Makefile Explanation
Source: https://www.gnu.org/software/make/manual/make.html  
To simply use the attached makefile for your programs, replace the target with the main file in your c program. 
Makefiles allow you to build c programs. It is important to understand the makefile used to override the default setup line by line. Note that make decides which files to build and update based on last-edit time each successive time you run the command. Make has a list of default rules that it follows if you do not specify a makefile. You can view these rules by running:
```bash
$ make -p
```  
The first few lines define variables that will be used later in the makefile. The **TARGET** variable specifies the main executable file to be created and it's source. **LIBS** is set to '-lm' to link the math library for historical reasons. **CC** sets the compiler to gcc. **CFLAGS** are compiler flags. '-g' tells gcc to add debugging information to the executable file. '-Wall' stands for all warnings and enables a specified list of warnings. '-Wextra' stands for extra warnings and enables even more warnings.
```make
TARGET = hello
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Wextra
```
Make by default assumes that all of your targets are files. That is all fine and dandy until you want make to run commands like _clean_ and _all_ which do not represent files on your system. If you so happen to have a file called 'clean,' make will confuse it for a target by default. **.PHONY** lets make know that these commands are 'phony' targets. It also forces the command to run regardless of the state of the filesystem (i.e. even if the file has not been updated)
```make
.PHONY: default all clean
```
The next lines (**default**, **all**) override the rules set in the default makefile. "By default make starts with the first target. This is called the _default goal_." This is why you are simply able to run _make_ without explicitly specifying anything. Other rules only get processed if they are prerequisites of the default goal.
```make
default: $(TARGET)
all: default
```
The next few lines define more variables which are used to make the compiling process easier. The **OBJECTS** variable is used to specify the list objects to built to supplement the main program. Glob **wildcard** are automatically filled by make except in command arguments. There, they need to be explicitly called. **patsubst** takes arguments in the form ```$(patsubst pattern,replacement,text)``` where the '%' act as wildcards. The **HEADERS** variable is set to all the specified headers in the folder.
```make
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
```
The next wildcard rule, **%.o**, specifies how to build object files for the supplementary c files. The '-c' flag prevents gcc from invoking the linker after compiling the file. Note that '$@' is an implicit variable that defines the name of target of the rule and '$<' represents the first prerequisite. '-o' tells gcc to create an object file with the specified name.
```make
%.o: %.c $(HEADERS)
    $(CC) $(CFLAGS) -c $< -o $@
```
**.PRECIOUS** prevents make from deleting the targets listed in the argument if make is quit during execution.
```make
.PRECIOUS: $(TARGET) $(OBJECTS)
```
The main rule, **$(TARGET)**, is run to generate the executable. First, it defines all the object files that it needs which are build using the object rule that was defined prior. Next, it uses the specified compiler and object files to build the executable using the defined options.
```make
$(TARGET): $(OBJECTS)
    $(CC) $(OBJECTS) -Wall -Wextra $(LIBS) -o $@
```
The last rule, **clean**, is not run by default but when run, it cleans up the generated files that are generated by the make process.
```make
clean:
    -rm -f *.o
    -rm -f $(TARGET)
```