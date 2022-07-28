from cs50 import get_string


def count_words(text):
    """Calculates the number of words in a text by counting the number of spaces and
    adding 1."""
    count = 0

    for character in text:
        if character == " ":
            count += 1

    return count + 1


def count_letters(text):
    """Counts the number of letters in a text by counting alpha characters [a-zA-Z]."""
    count = 0

    for character in text:
        if character.isalpha():
            count += 1

    return count


def count_sentences(text):
    """Counts the number of sentences by counting the final characters [.!?]."""
    count = 0

    for character in text:
        if character in ".?!":
            count += 1

    return count


text = get_string("Text: ")

# Count the letters, words and sentences in the input text and calculate the averages
# per 100 words.
word_count = count_words(text)
letter_avg = 100.0 * count_letters(text) / word_count
sentence_avg = 100.0 * count_sentences(text) / word_count

# Calclulate the Coleman-Liau index.
index = 0.0588 * letter_avg - 0.296 * sentence_avg - 15.8

if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {int(round(index))}")
