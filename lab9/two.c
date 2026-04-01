// Assuming a page size of 32 bytes and there are total of 8 such pages totaling 256 bytes. Write a C program to simulate this memory mapping. The program should read the logical memory address and display the page number and page offset in decimal. How many bytes do you need to represent the address in this scenario? Display the page number and offset to reference the following logical addresses: 204 byte, 56 byte.

#include <stdio.h>

int main()
{
    int logicalAddr;
    int pageSize = 32;

    printf("Enter logical address (0-255): ");
    scanf("%d", &logicalAddr);

    if (logicalAddr < 0 || logicalAddr > 255)
    {
        printf("Invalid logical address.\n");
        return -1;
    }

    int pageNo = logicalAddr / pageSize;
    int offset = logicalAddr % pageSize;

    printf("Page number: %d\nOffset: %d\n", pageNo, offset);

    return 0;
}