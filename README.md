# Free time task - expression evaluator

_This is not a full RPN or AST based expression parser and evaluator._

This program tries to imitate oldschool pocket calculators, where operations were executed sequentially, as user was typing numbers and selecting operators (so simply speaking from LEFT to RIGHT). It means that there are NO operator precedense rules. All four operators (+, -, *, /) are equal, and are executed one after another.

The only special handling is related to parenthesis. They have higher priority then operators, so program first calculates contents of what is inside parenthesis, and then sums up rest.

These assumptions are making program RADICALLY simpler. There is simply just one stack for operators and operands, which is used ONLY when there is any opening parenthesis. If not then program runs fully on single accumulator and does everything in single pass.

If only there is an opening paren, it puts the current accumulator along with previous operator on stack and starts new calculation for this parenthesis. When closing paren is found, it pops back from stack accumulator, its operator, adds to the sum calculated inside the parenthesis and continues further.

_In contrast for example to full RPN parsers program has to run at least in two passes - first by generating tokens from infix to postfix form, and then calculating RPN postfix equation. This program uses only single pass. Much easier, less lines, much faster._

Program handles prefix negation for numbers. Only requirement is that there CANNOT be any space between minus sign and number. Because of that the main parsing loop has aka two-state-machine, where in first state it searches for operand (and eventually open paren), and in second one searches for operator (and eventually close paren).

As it is with calculators there is also range and overflow checking, both for operands like also for additions, subtractions and multiplications. For division there is check if operand is not zero.

Provided a two sets of unit tests. One set verifies expected result, other verifies that particular test fails. Provided also a command line application which does exactly what is should - evaluate-s.

The core file `eval.c` could have be written fully without using ANY include file, but since the task very generic C, then I had to use at least `limits.h` when checking for overflows/underflows.

This repo is 100% my own work code.

(C) Adam Kowalewski : ram.techen@gmail.com

## Building

On Windows, simply launch MSVC developer console, type `build.bat` and you are in.

On Linux, simply launch terminal, type `sh build.sh`, and you are in.

It should build everything and run tests.

## Running

When building tests will run automatically. You can do it by hand as well by launching `eval_test.exe`

CLI application simply gets input via parameters. Try it out `eval.exe 1+2*3/4`

![image](https://github.com/akowalew/eval/assets/11333571/15605d99-61e6-463e-85c2-f3f805b40ff3)
