#include <stdio.h>
#include <string.h>
#include "Consumer.c"
#include "Administrator.c"
#include "LoginRegister.c"

void main()
{
    // Apertura del file in modalità lettura
    FILE *fpt = fopen("Utenti.csv", "r");

    int scelta;
    printf("\nBenvenuto nella nostra banca!\n");
    printf("Cosa vuole fare?\n");
    printf("1 - Entrare nel conto\n");
    printf("2 - Aprire un conto\n");
    scanf("%d", &scelta);
    switch (scelta)
    {
    case 1:
        Login(fpt);
        break;

    case 2:
        Register(fpt);
        break;
    }
}