import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database file into a variable
    sequences_db = []
    try:
        with open(sys.argv[1]) as f:
            reader = csv.DictReader(f)
            for sequence in reader:

                # Convert the occurences of a subsequence into an integer value
                for key in sequence:
                    if key != "name":
                        sequence[key] = int(sequence[key])

                sequences_db.append(sequence)
    except FileNotFoundError:
        sys.exit(f"File {sys.argv[1]} not found!")

    # Read DNA sequence file into a variable
    try:
        with open(sys.argv[2]) as f:
            test_sequence = f.read()
    except FileNotFoundError:
        sys.exit(f"File {sys.argv[2]} not found!")

    # Create a dictionary that will store the subsequences and how many times they are
    # repeated in the text_sequence
    subsequence_repeats = {}
    for subsequence in sequences_db[0]:
        if subsequence != "name":
            subsequence_repeats[subsequence] = 0

    # Find longest match of each STR in DNA sequence
    for subsequence in subsequence_repeats:
        subsequence_repeats[subsequence] = longest_match(test_sequence, subsequence)

    # Check database for matching profiles
    match = None
    for person in sequences_db:
        name = person.pop("name")
        if person == subsequence_repeats:
            match = name
            break

    # If a match is found, print the matched name.
    if match:
        print(match)
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
