#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser.
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int check_cycled(int root_winner, int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i], name))
        {

            // of the voter with the candidate.
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Go through all combinations of candidates and compare the preference votes
    // that are cast for them. Put the winner/loser of each combination in the array
    // of structures. If there is a tie, do not generate an array entry.
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int strengths[pair_count];
    int max_index;
    int temp_strength;
    pair temp_pairs;

    // Generate an array of relative strengths of victories, based on the preference of winner-loser to loser-winner
    for (int i = 0; i < pair_count; i++)
    {
        strengths[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].loser];
    }

    // Sort based on strength of victory. The sorting algorithm is selection sort.
    for (int i = 0; i < pair_count - 1; i++)
    {
        // Find the index of the maximum value in the unsorted part of the list.
        max_index = i;
        for (int j = i + 1; j < pair_count; j++)
            if (strengths[j] > strengths[max_index])
            {
                max_index = j;
            }

        // Switch the maximum strength value with the left-most unsorted one.
        temp_strength = strengths[i];
        strengths[i] = strengths[max_index];
        strengths[max_index] = temp_strength;

        // Do the same switch on the array of pairs.
        temp_pairs = pairs[i];
        pairs[i] = pairs[max_index];
        pairs[max_index] = temp_pairs;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int pair_number = 0; pair_number < pair_count; pair_number++)
    {
        if (!check_cycled(pairs[pair_number].winner, pairs[pair_number].winner, pairs[pair_number].loser))
        {
            locked[pairs[pair_number].winner][pairs[pair_number].loser] = true;
        }
    }

    return;
}

// This recursive function follows all the possible paths: It check if the loser in
// this iteration has won over somebody else and continues to do so until either there
// Is no path forward, in which case it returns one iteration up, or if the loser is the
// original winner, in which case a cycle has been found and a non-zero value is returned.
int check_cycled(int root_winner, int winner, int loser)
{
    if (loser == root_winner)
    {
        return 1;
    }

    int cycled = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            cycled += check_cycled(root_winner, loser, i);
        }
    }

    return cycled;
}

// Print the winner of the election
void print_winner(void)
{
    // Find the source of the graph. That is, in the locked matrix, find the column with
    // no true elements (nobody prefers another candidate over this one).
    bool column_or;
    // Loop through all the columns.
    for (int i = 0; i < candidate_count; i++)
    {
        column_or = false;

        // Make an OR of all the elements in the column.
        for (int j = 0; j < candidate_count; j++)
        {
            column_or = column_or || locked[j][i];
        }

        // If the column has no true elements (OR is false), we have found a winner!
        if (!column_or)
        {
            printf("%s\n", candidates[i]);
        }
    }

    return;
}