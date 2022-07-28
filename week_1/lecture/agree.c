#include <cs50.h>
#include <stdio.h>

int main(void)
{
    char response = get_char("Do you agree? ");

    if (response == 'y' || response == 'Y')
    {
        printf("We agree.\n");
    }
    else if (response == 'n' || response == 'N')
    {
        printf("We disagree.\n");
    }
}