#include "eval.c"

#include <stdio.h>

// NOTE: Helper macros for stringifying things (e.g. numeric constants)
#define STR_(x) #x
#define STR(x) STR_(x)

static int TestsOk;
static int TestsFailed;

static void CheckSuccess(const char* Expression, int Expected)
{
    int Result;
    if(Evaluate(Expression, &Result))
    {
        if(Result == Expected)
        {
            printf("[  OK  ] %8d == Evaluate(\"%s\")\n", Expected, Expression);
            TestsOk++;
        }
        else
        {
            printf("[FAILED] %8d != Evaluate(\"%s\") (actual result is %d)\n", Expected, Expression, Result);
            TestsFailed++;
        }
    }
    else
    {
        printf("[FAILED] Evaluate(\"%s\") returned false (%s)\n", Expression, EvaluateError);
        TestsFailed++;
    }
}

static void CheckFailure(const char* Expression)
{
    int Result;
    if(!Evaluate(Expression, &Result))
    {
        printf("[  OK  ] %20s for Evaluate(\"%s\")\n", EvaluateError, Expression);
        TestsOk++;
    }
    else
    {
        printf("[FAILED] Evaluate(\"%s\") returned true with result: %d\n", Expression, Result);
        TestsFailed++;
    }
}

int main()
{
    printf("------------ Positive test cases ------------\n");
    CheckSuccess("1", 1);
    CheckSuccess("1 - 2", -1);
    CheckSuccess("1 - 2 + 3", 2);
    CheckSuccess("1 - 2 + 3 * 4", 8);
    CheckSuccess("1 - 2 + 3 * 4 / 5", 1);
    CheckSuccess("-1", -1);
    CheckSuccess("-1 + -2", -3);
    CheckSuccess("-1 + -2 * -4", 12);
    CheckSuccess("-1 + -2 * -4 / -5", -2);
    CheckSuccess("(1)", 1);
    CheckSuccess("(1 + 2)", 3);
    CheckSuccess("(1 + 2 - ( 3 ) )", 0);
    CheckSuccess("(1 + 2 - ( 3 * 4 ) + 1 )", -8);
    CheckSuccess("(1 + 2 * ( 3 - 4 ) / 2 )", -1);
    CheckSuccess("((((1))))", 1);
    CheckSuccess("(4 / 2) + 6", 8);
    CheckSuccess("4 + (12 / (1 * 2))", 10);
    CheckSuccess("1 + 3", 4);
    CheckSuccess("(1 + 3) * 2", 8);
    CheckSuccess("(4 / 2) + 6", 8);
    CheckSuccess("4 + (12 / (1 * 2))", 10);

    printf("------------ Negative test cases ------------\n");
    CheckFailure("");
    CheckFailure("1 - ");
    CheckFailure("1 - 2 + ");
    CheckFailure("1 - 2 + 3 * ");
    CheckFailure("1 - 2 + 3 * 4 / ");
    CheckFailure("-1 + -");
    CheckFailure("-1 + -2 * -");
    CheckFailure("-1 + -2 * -4 / -");
    CheckFailure("(2 + (12 * 2)");
    CheckFailure("-1 + (1 + )");
    CheckFailure("-");
    CheckFailure("+-*/");
    CheckFailure("(+ 2)");
    CheckFailure("1 + )");
    CheckFailure("()");
    CheckFailure("((((");
    CheckFailure("((((((((1+(2)))))))))");
    CheckFailure("1+2*3/0");
    CheckFailure("1+2*4/(3-2-1)");
    CheckFailure("100000000*100000000");
    CheckFailure(STR(INT_MAX)"0");
    CheckFailure(STR(INT_MAX)"+1");
    CheckFailure(STR(INT_MIN)"-1");
    CheckFailure(STR(SHRT_MAX)"*"STR(SHRT_MAX)"*"STR(SHRT_MAX));
    CheckFailure(STR(SHRT_MAX)"*"STR(SHRT_MAX)"*-"STR(SHRT_MAX));

    printf("------------------ Summary ------------------\n");
    printf("[%s] Tests failed: %d\n", TestsFailed ? "FAILED" : "  OK  ", TestsFailed);
    printf("[%s] Tests ok: %d\n", "  OK  ", TestsOk);
    printf("\n");

    return TestsFailed ? 1 : 0;
}
