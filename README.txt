Welcome to the binary calculator!

This little program allows you to manipulate binary numbers, create variables and calculate complex expressions as well.

Please execute it in a linux terminal or in a console that supports ANSI escape sequences!

Compile and execute Mode:

1) LEX: $ lex lexer.l
2) YACC: $ yacc -vd program.y
3) C: $ gcc y.tab.c lex.yy.c binary.c -lm -lfl [-std=c99]
4) execute: $ ./a.out

Operations:

+: Addition
-: Subtraction
*: Multiplication
/: Division
%: Module
&: logical AND
|: logical OR

Constraints:

Unlike the most of calculators, if you have two variables a and b, when you put this expression: ab, it'll a call for the ab variable and doesn't makes a*b. Similar to a(b), that's taken as error.