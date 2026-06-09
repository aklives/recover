#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char *argv[])
{
    // Checks how many command-line arguments were provided. 
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;   
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }
    
    // Initializes a buffer to hold 512 bytes, which is the size of a block on a memory card.
    const int BLOCK_SIZE = 512;
    uint8_t buffer[BLOCK_SIZE];
    int file_count = 0;

    FILE *output = NULL;

    // Reads the memory card in blocks of 512 bytes until the end of the file is reached.
    while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (file_count > 0)
            {
                fclose(output);
            }
            char filename[8];
            sprintf(filename, "%03d.jpg", file_count);
            output = fopen(filename, "w");
            file_count++;
        }
        if (file_count > 0)
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, output);
        }
    }

    // Closes any open files and exits the program.
    fclose(card);
    fclose(output);
    return 0;
}