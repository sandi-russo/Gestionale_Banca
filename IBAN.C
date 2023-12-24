#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

void GeneraIBAN(char stringa[])
{
    int min = 10, max = 99;

    int cineu = rand() % (max - min + 1) + min;
    char it[] = "IT";
    char abi[] = "07648";
    char cab[] = "05433";
    char zeri[] = "00000";
    char conto[8];
    char cin = 'A' + rand() % 26;

    for (int i = 0; i < 7; i++)
    {
        conto[i] = '0' + rand() % (10);
    }
    conto[7] = '\0';

    snprintf(stringa, 28, "%s%02d%c%s%s%s%s", it, cineu, cin, abi, cab, zeri, conto);
}

// Ciao Danil