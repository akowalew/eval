#include "eval.c"

#include <stdio.h>

int main(int Argc, char** Argv)
{
    if(Argc == 1)
    {
        fprintf(stderr, "Syntax: %s <expression>\n", Argv[0]);
        return 1;
    }

    int Result;
    if(!Evaluate(Argv[1], &Result))
    {
        fprintf(stderr, "%s\n", EvaluateError);
        return 1;
    }

    printf("%d\n", Result);
}