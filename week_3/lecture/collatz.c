#include <cs50.h>
#include <stdio.h>

int collatz(int number, int step);

int main(void)
{
    int number = get_int("Number: ");

    printf("%i\n", collatz(number, 0));
}

int collatz(int number, int step)
{
    if (number == 1)
        return 0;

    if (number % 2)
        step = collatz(3 * number + 1, step);
    else
        step = collatz(number / 2, step);

    return ++step;
}