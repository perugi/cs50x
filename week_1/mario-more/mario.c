#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get the required size of the Pyramid from the user.
    int size;
    while (true)
    {
        size = get_int("Height: ");
        // Keep asking until the user gives a number between 1 and 8.
        if (size >= 1 && size <= 8)
        {
            break;
        }
    }

    // Print the required number of rows.
    for (int i = 1; i <= size; i++)
    {
        // Print the required number of columns.
        for (int j = 1; j <= 2 * size + 2 + 1; j++)
        {
            // Print the padding
            if (j <= size - i)
            {
                printf(" ");
            }
            // Next, print the left side of the pyramid.
            else if (j <= size)
            {
                printf("#");
            }
            // Next, print the middle double whitespace.
            else if (j <= size + 2)
            {
                printf(" ");
            }
            // Next, print the right side of the pyramid.
            else if (j <= size + 2 + i)
            {
                printf("#");
            }
            // Once everything has been printed, make a new line and break the
            // printing of the row.
            else
            {
                printf("\n");
                break;
            }
        }
    }
}