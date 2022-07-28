#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int check_arguments(int argc);
int check_key_length(string key);
int check_invalid_chars(string key);
int check_unique_chars(string key);
string cipher(string key, string plaintext);

int main(int argc, string argv[])
{

    if (check_arguments(argc))
    {
        return 1;
    }

    // Perform all check on the key, input by the user as the command line argument.
    string key = argv[1];
    if (check_key_length(key))
    {
        return 1;
    }
    if (check_invalid_chars(key))
    {
        return 1;
    }
    if (check_unique_chars(key))
    {
        return 1;
    }

    // Get the input text as a string from the user.
    string plaintext = get_string("plaintext:  ");

    // Based on the input key, perform the cipher of the plaintext.
    string ciphertext = cipher(key, plaintext);

    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

// Check that the user passed only a single command line argument.
int check_arguments(int argc)
{
    if (argc != 2)
    {
        printf("Usage: /.substitution key\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

// Check that the key, passed by the user, contains exactly 26 characters.
int check_key_length(string key)
{
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

int check_invalid_chars(string key)
{
    // Check for invalid characters in the string (characters that are not either
    // [a-z] or [A-Z]).
    for (int i = 0; i < strlen(key); i++)
    {
        if (!(isalpha(key[i])))
        {
            printf("Invalid key! Please input only alphabetical characters.\n");
            return 1;
        }
    }

    return 0;
}

int check_unique_chars(string key)
{
    // Check that all the characters in the key string are unique
    for (int i = 0; i < strlen(key); i++)
    {
        for (int j = i + 1; j < strlen(key); j++)
        {
            if (tolower(key[i]) == tolower(key[j]))
            {
                printf("Invalid key! Please input only unique characters.\n");
                return 1;
            }
        }
    }

    return 0;
}

string cipher(string key, string plaintext)
{
    // The ciphertext needs to be as long as the plaintext, we use the assignment to
    // reserve enough space in the memory.
    string ciphertext = plaintext;

    // Cipher the text character by character. Match the input case and skip the
    // characters, not belonging to [a-zA-Z].
    int key_lookup;
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (islower(plaintext[i]))
        {
            // The 'a' character is encoded as 97 in ASCII. It must be translated into
            // the key_lookup of 0, 'b' is 1 etc.
            key_lookup = (int)plaintext[i] - 97;
            ciphertext[i] = tolower(key[key_lookup]);
        }
        else if (isupper(plaintext[i]))
        {
            // The 'A' character is encoded as 65 in ASCII. It must be translated into
            // the key_lookup of 0, 'B' is 1 etc.
            key_lookup = (int)plaintext[i] - 65;
            ciphertext[i] = toupper(key[key_lookup]);
        }
        // The characters not belonging to [a-zA-Z] do not need an else statement,
        // as they have been transferred to ciphertext in the original assignment.
    }

    return ciphertext;
}