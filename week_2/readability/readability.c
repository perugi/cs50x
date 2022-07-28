#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_words(string text);
int count_letters(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    // Count the letters, words and sentences in the input text.
    int letter_count = count_letters(text);
    int word_count = count_words(text);
    int sentence_count = count_sentences(text);

    // Calclulate the Coleman-Liau index.
    float index = 0.0588 * ((float)letter_count / word_count * 100) -
                  0.296 * ((float)sentence_count / word_count * 100) - 15.8;

    if (index < 1)
    {
        // In case the index is before grade 1.
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        // In case the index is beyond grade 16.
        printf("Grade 16+\n");
    }
    else
    {
        // If not, print the rounded grade.
        printf("Grade %i\n", (int)round(index));
    }
}

int count_letters(string text)
{
    // Count the number of letters, by counting [a-z] and [A-Z] characters.
    int count = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (isupper(text[i]) || islower(text[i]))
        {
            count++;
        }
    }

    return count;
}

int count_words(string text)
{
    // Count the number of words, by counting the spaces between them and adding one.
    int count = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }

    return ++count;
}

int count_sentences(string text)
{
    // Count the number of sentences, by counting the .?!
    int count = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }

    return count;
}