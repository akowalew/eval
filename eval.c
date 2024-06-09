#include <limits.h>

// NOTE: Depth of computation stack is compile time constant.
// We can make it variable size, but better not on console.
#ifndef EVDEPTH
#define EVDEPTH 8
#endif

// NOTE: When doing asserts I want to see them directly in the debugger
#define Assert(x) if(!(x)) { *(int*)(0) = 0; }

typedef unsigned char bool;

typedef struct
{
    const char* At;
} cstr;

static char EatAllWhitespace(cstr* Str)
{
    char C;
    while((C = *Str->At) != 0)
    {
        // TODO: SIMD hammer here please!
        if(C == ' ' ||
           C == '\t' ||
           C == '\r' ||
           C == '\v' ||
           C == '\r' ||
           C == '\n' ||
           C == '\b')
        {
            // NOTE: Of course I may use `isspace`, but I don't want
            Str->At++;
        }
        else
        {
            break;
        }
    }

    return C;
}

static bool ParseNumber(cstr* Str, int* Result, bool* Overflow)
{
    const char* At = Str->At;

    bool Negative = 0;
    if(*At == '-')
    {
        Negative = 1;
        At++;
    }

    char C = *At;
    if(C < '0' || C > '9')
    {
        // NOTE: There MUST BE no space between minus and number
        // Languages like C/C++/Python allow to put space, they also allow
        // to negate/positive multiple times, but this is a SIMPLE evaluator
        return 0;
    }

    int Sum = 0;

    do
    {
        // TODO: SIMD hammer please here
        int Acc = Sum * 10 + (C - '0');
        if(Acc < Sum)
        {
            *Overflow = 1;
            return 1;
        }

        Sum = Acc;
        C = *(++At);
    } while(C >= '0' && C <= '9');

    if(Negative)
    {
        if(Sum == -Sum)
        {
            *Overflow = 1;
            return 1;
        }

        Sum = -Sum;
    }

    *Result = Sum;
    Str->At = At;
    return 1;
}

// NOTE: For ease of testing I've decided to have some global error string
static const char* EvaluateError;
// It can be improved though by putting more detailed info like char number etc
#define EVERROR(Str) EvaluateError = (Str)

static bool Calculate(int* Sum, int Operand, char Op)
{
    int Val = *Sum;

    switch(Op)
    {
        case '-':
        {
            if(((Operand > 0) && (Val < INT_MIN + Operand)) ||
               ((Operand < 0) && (Val > INT_MAX + Operand)))
            {
                EVERROR("Subtraction overflow");
                return 0;
            }

            *Sum = Val - Operand;
        } break;

        case '+':
        {
            if(((Operand < 0) && (Val < INT_MIN - Operand)) ||
               ((Operand > 0) && (Val > INT_MAX - Operand)))
            {
                EVERROR("Addition overflow");
                return 0;
            }

            *Sum = Val + Operand;
        } break;

        case '*':
        {
            if(Val == 0 || Operand == 0)
            {
                *Sum = 0;
            }
            else
            {
                // TODO: This probably can cause UB - fix this!
                int Acc = Val * Operand;
                if(Val != Acc / Operand)
                {
                    EVERROR("Multiply overflow");
                    return 0;
                }

                *Sum = Acc;
            }
        } break;

        case '/':
        {
            if(Operand == 0)
            {
                EVERROR("Divide by zero");
                return 0;
            }

            *Sum = Val / Operand;
        } break;

        default: Assert(0); break;
    }

    return 1;
}

static bool Evaluate(const char *Expression, int *Result)
{
    cstr Str = { .At = Expression };

    int Level = 0;
    char OpStack[EVDEPTH]; // NOTE: 4 operators, 6 bits waste, but easy to use.
    int SumStack[EVDEPTH];

    int Sum = 0;
    char Op = '+'; // NOTE: We can always do addition to zero
    while(*Str.At)
    {
        char C;

        while((C = EatAllWhitespace(&Str)) != 0)
        {
            int Operand;
            bool Overflow = 0;
            // NOTE: Number parsing can be replaced with strtol family, but they
            // rely on ERRNO, which has to be reentrant, and its syntax causes headache.
            if(ParseNumber(&Str, &Operand, &Overflow))
            {
                if(Overflow)
                {
                    EVERROR("Operand overflow");
                    return 0;
                }

                if(!Calculate(&Sum, Operand, Op))
                {
                    return 0;
                }

                Op = 0;

                break; // Expect Op or close paren as next
            }
            else if(C == '(')
            {
                if(Level >= EVDEPTH)
                {
                    EVERROR("Too much nesting");
                    return 0;
                }

                OpStack[Level] = Op;
                SumStack[Level] = Sum;
                Level += 1;

                Sum = 0;
                Op = '+';
                Str.At++;
                continue; // Expect operand or new open paren as next
            }
            else
            {
                EVERROR("Invalid operand");
                return 0;
            }
        }

        while((C = EatAllWhitespace(&Str)) != 0)
        {
            if(C == '-' || C == '+' || C == '*' || C == '/')
            {
                Op = C;
                Str.At++;
                break; // Expect operand or open paren as next
            }
            else if(C == ')')
            {
                if(Op)
                {
                    EVERROR("Missing operand");
                    return 0;
                }

                if(!Level)
                {
                    EVERROR("Unexpected closing");
                    return 0;
                }

                Level--;
                int Num = SumStack[Level];
                if(!Calculate(&Num, Sum, OpStack[Level]))
                {
                    return 0;
                }

                Sum = Num;
                Str.At++;
                continue; // Expect Op or another close paren as next
            }
            else
            {
                EVERROR("Invalid operator");
                return 0;
            }
        }
    }

    if(Op)
    {
        EVERROR("Missing operand");
        return 0;
    }
    else if(Level)
    {
        EVERROR("Unclosed paren");
        return 0;
    }

    *Result = Sum;

    return 1;
}
