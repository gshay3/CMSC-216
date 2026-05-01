/*
 * Description:
 * This program reads a line of text from standard input and converts each
 * character into its 8-bit binary representation. The binary values are printed
 * with spaces between them and a newline inserted after every 8 characters.
 *
 * Author: Griffin Shay
 * Date: 9/9/2024
 */

#include <stdio.h>
#include <string.h>

int main(void)
{
    // Buffer to store the input string.
    char str[100] = {0};

    // Buffer to store the 8-bit binary representation of a character.
    char binary[9] = {0};
    
    printf("\n");
    fgets(str, sizeof(str), stdin);

    // Loop through each character in the input string.
    for(int i = 0; i < strlen(str); i++)
    {
        // Convert character to its ASCII integer value.
        int x = (int)str[i];

        // Convert the integer to binary (8 bits).
        for(int j = 7; j >= 0; j--)
        {
            if(x % 2 == 0)
            {
                binary[j] = '0';
                x = x / 2;
            }
            else
            {
                binary[j] = '1';
                x = (x - 1) / 2;
            }
        }

        // Null-terminate the binary string.
        binary[8] = '\0';
        
        printf("%s ", binary);
        if((i + 1) % 8 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("\n");
    return 0;
}
