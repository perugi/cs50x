#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask the user for a name:
    string name = get_string("What's your name? ");

    // Print the hello, together with the input name:
    printf("hello, %s\n", name);
}