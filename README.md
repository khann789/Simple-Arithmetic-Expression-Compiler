Simple Arithmetic Expression Compiler
This project implements a simple compiler for evaluating arithmetic expressions. It supports the four basic arithmetic operations: addition, subtraction, multiplication, and division. The compiler processes an expression in three distinct phases: preprocessing, lexical analysis, and syntax analysis.

Features:
Preprocessing: Removes spaces and replaces the semicolon (;) with a $ sentinel to mark the end of the expression.

Lexical Analysis: Identifies tokens such as identifiers (variables), operators, and the sentinel ($). It flags any invalid tokens and terminates the program with an error message if any are found.

Syntax Analysis: Uses a stack-based approach to parse the expression according to operator precedence:

Division and multiplication are evaluated first.

Addition and subtraction are evaluated afterward.

User Input: The compiler prompts the user to provide values for any variables (identifiers) in the expression.

Step-by-step Parsing: Displays a stack-based table showing each operation and the intermediate results.

Error Handling: Provides error messages for invalid tokens, division by zero, or incorrect syntax.

How to Use:
Clone the repository or download the files.

Compile the COMP451_project.c file using any standard C compiler:

bash
Copy
gcc COMP451_project.c -o compiler
Run the executable:

bash
Copy
./compiler
Input an arithmetic expression in the format id = expression; (e.g., out = a + b * c;).

The program will prompt you to input numeric values for any identifiers in the expression and then display the result.

Example:
Input:

c
Copy
out = a + b * c;
Output:

bash
Copy
Enter integer values of the following identifiers:
Value of a: 5
Value of b: 3
Value of c: 2

Stack                 Action
-----------------------------
$                     None
5                     Push
3                     Push
2                     Push
...
Result: out = 11
Technologies Used:
C Programming Language

Standard Libraries: stdio.h, stdlib.h, string.h, ctype.h
