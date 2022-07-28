#include <cs50.h>
#include <stdio.h>
#include <math.h>

int num_of_digits(long number);
int get_digit(long number, int pos);

int main(void)
{
    long cc_number;
    int digits;
    // Ask the user for a cc number, until a number is provided.
    cc_number = get_long("Number: ");
    digits = num_of_digits(cc_number);

    // Calculate the sum for the Luhn's algorithm. Starting with the last digits, take
    // every other and add it to the sum. Starting with the second to last digit, take
    // every other and add its multiple of 2 to the sum.
    int sum = 0;
    for (int i = 0; i < digits; i++)
    {
        // Handling all the other digits, starting with the second to last.
        if (i % 2)
        {
            int multiple = get_digit(cc_number, i) * 2;
            // Because the multiple is max 9 * 2 = 18, this is a max 2-digit number.
            // According to the algorithm, we need to sum both of the digits separately.
            sum += get_digit(multiple, 0);
            sum += get_digit(multiple, 1);
        }
        // Handling all the other digits, starting with the last.
        else
        {
            sum += get_digit(cc_number, i);
        }
    }

    // Check if the last digit of the sum is 0 (valid card number).
    if (sum % 10 == 0)
    {
        // If the first two digits are 51, 52, 53, 54, 55, this is a Mastercard.
        if (get_digit(cc_number, digits - 1) == 5 &&
            (get_digit(cc_number, digits - 2) == 1 ||
             get_digit(cc_number, digits - 2) == 2 ||
             get_digit(cc_number, digits - 2) == 3 ||
             get_digit(cc_number, digits - 2) == 4 ||
             get_digit(cc_number, digits - 2) == 5) &&
            digits == 16)
        {
            printf("MASTERCARD\n");
        }
        // If the first two digits are 34 or 37, this is an American Express.
        else if (get_digit(cc_number, digits - 1) == 3 &&
                 (get_digit(cc_number, digits - 2) == 4 ||
                  get_digit(cc_number, digits - 2) == 7) &&
                 digits == 15)
        {
            printf("AMEX\n");
        }
        // if the first digit is 4, this is a Visa.
        else if (get_digit(cc_number, digits - 1) == 4 &&
                 (digits == 13 ||
                  digits == 16))
        {
            printf("VISA\n");
        }
        // If no match, the card is invalid.
        else
        {
            printf("INVALID\n");
        }
    }
    // If the last digit of the sum is not 0, the card is invalid.
    else
    {
        printf("INVALID\n");
    }
}

int num_of_digits(long number)
{
    int length = 1;
    while (number /= 10)
    {
        length++;
    }
    return length;
}

int get_digit(long number, int pos)
{
    return (number / (long)pow(10, pos)) % 10;
}