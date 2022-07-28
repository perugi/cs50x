#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage ./recover IMAGE\n");
        return 1;
    }

    // Define a byte type for convenience.
    typedef uint8_t BYTE;

    // Define a block size in bytes which will be read out from the card file.
    const int BLOCK_SIZE = 512;

    // Allocate a space for a buffer of BLOCK_SIZE bytes.
    BYTE *buffer = malloc(BLOCK_SIZE * sizeof(BYTE));

    // Open the raw file, based on the argument, passed by the user.
    FILE *raw_file = fopen(argv[1], "r");

    // The filename will be 3 characters long + the NUL character.
    char *filename = malloc(4 * sizeof(char));
    // This pointer is used for opening the .jpg files.
    FILE *jpg_file = NULL;

    // The file counter is used in naming the generated .jpgs (000.jpg, 001.jpg, etc.)
    int file_counter = 0;
    bool file_found = false;

    // Read from the card as long as the card can be read.
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // In the first four bytes of the block, look for the .jpg signature.
        if (
            buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0
        )
        {
            // When a jpg is found, if this is not the first .jpg, close the previous one.
            if (file_found)
            {
                fclose(jpg_file);
            }
            sprintf(filename, "%03i.jpg", file_counter++);
            jpg_file = fopen(filename, "a");
            file_found = true;
        }

        // Write the contents of the memory into the .jpg file.
        if (file_found)
        {
            fwrite(buffer, 1, BLOCK_SIZE, jpg_file);
        }
    }

    fclose(jpg_file);

    free(buffer);
    free(filename);
    return 0;
}