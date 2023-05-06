# JOAOPLEX
Maybe not the best C++ SIMPLEX implementation, but the one with the coolest name.

# Simplex algorithm
The Simplex is a well known algorithm to solve linear optimization problems. Faced with a problem, they are inequalities that represent restrictions for the variables. From there, possibilities are tested in order to optimize the result as quickly as possible. See more in the [Cornell optimization webpage](https://optimization.cbe.cornell.edu/index.php?title=Simplex_algorithm). The solving method used was the [Two phase Simplex method](https://uomustansiriyah.edu.iq/media/lectures/6/6_2022_01_08!08_05_56_PM.pdf).

# Implementation
There are three .cpp files apart from mais. Are they:

- **inputParser.cpp**: 
Process a standardized input file from an LP generating a inital matrix (almost standard form).
- **simplex.cpp**:
All simplex numerical methods.
- **commom.cpp**:
Commom methods and data structures.

The main file is responsible for starting the simplex phases, verifying that the matrix is in standard form, in addition to filling in the output data structure (certificate, solution and objective).

# GMP library
In order to avoid numerical errors that accumulate during Simplex execution, the GMP library was adopted to represent rational numbers. GMP is the GNU Multiple Precision Arithmetic Library and its documentation can be seen [here](https://gmplib.org/).

# Input
The input to the program is a standardized file from an LP. In the repository you can access a sample input.txt file to verify the accepted input pattern.

# Run and compile instructions
The GMP library was pre compiled and static included in the project. I hope this is enough to, in a Linux computer, just run ``make`` in the root directory to compile all files (the code was tested using g++ version 9.5.0). In worst case scenarion if the code does not compile, just install the library throught ``sudo apt install libgmp-dev`` command.

After compilation, the JOAOPLEX.out requires two input parameters: the path to input and output. To execute the code with the example input and save output in a file "output.txt" run ``./JOAOPLEX.out input.txt output.txt`` or just. ``make run``.

# Considerations
I used the vector structure to allocate arrays and perform operations. This leaves the execution time slower than when compared to the standard structure of arrays in C. In addition, a series of structures and routines in the code could be optimized, but as it is a study repository, I chose to ignore these details ( at least it works). In the end, to decide if you are going to use JOAOPLEX to help your mother optimize her Sunday lunch, take a moment and think about the problem:

- Will it cost you **money**? : Use **boring** [GUROBI](https://www.gurobi.com/).
- Will it make you **cooler**? : Use **badass** JOAOPLEX.

## Author

- [<img src="https://avatars.githubusercontent.com/u/73205375?v=4" width=115><br><sub>João Pedro Fernandes Silva</sub>](https://github.com/JoaoP-Silva)
