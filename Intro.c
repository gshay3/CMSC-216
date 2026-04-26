#include <stdio.h>
#include <string.h>

int main(void)
{
    char str[100] = {0};
    char binary[9] = {0};
    printf("\n");
    fgets(str, sizeof(str), stdin);

    for(int i = 0; i < strlen(str); i++)
    {
        int x = (int)str[i];
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