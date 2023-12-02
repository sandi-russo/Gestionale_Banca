#include <stdio.h>
#include <string.h>
#include "Consumer.c"
#include "Administrator.c"
#include "LoginRegister.c"

void main()
{
    // Apertura del file in modalità lettura
    char p1[100] = "Utenti.csv"; //p1 sta per primo percorso file quindi in questo caso stiamo prendendo Utenti.csv
    FILE *fpt;
    controlloFileX(fpt, p1); //Funzione che controlla se il file esiste. Guardare CFileScrittura.C!
    
    int scelta;
    printf("\nBenvenuto nella nostra banca!\n");
    printf("Cosa vuole fare?\n");
    printf("1 - Entrare nel conto\n");
    printf("2 - Aprire un conto\n");
    scanf("%d", &scelta);
    switch (scelta)
    {
    case 1:
        Login(fpt, p1);
        break;

    case 2:
        Register(fpt, p1);
        break;
    }
}