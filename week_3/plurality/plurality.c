#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i].name, name))
        {
            // When a candidate has been matched, iterate his vote count and return
            // from the vote function properly (return true).
            candidates[i].votes += 1;
            return true;
        }
    }

    // If no candidate has been found with the prescribed name, return true.
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int index_max = 0;

    // Find the candidate with the highest number of votes. If there are multiple, the
    // first index will be stored. This is not a problem, however, as we loop through
    // the list once more to print all that match the highest number of votes.
    for (int i = 1; i < candidate_count; i++)
    {
        if (candidates[i].votes > candidates[index_max].votes)
        {
            index_max = i;
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == candidates[index_max].votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }

    return;
}