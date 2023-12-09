#include <stdio.h>
#include "Consumer.c"
#include "Administrator.c"
#include "LoginRegister.c"

int main()
{
    // Generazione seed randomico per la generazione dell'Iban
    srand((unsigned int)time(NULL));
    int scelta;
    // Apertura del file in modalità lettura
    char p1[100] = "Utenti.csv"; // p1 sta per primo percorso file. In questo caso, stiamo prendendo "Utenti.csv"
    FILE *fpt;
    controlloFileX(fpt, p1); // Funzione che controlla se il file esiste. Guardare RWCFile.C!

    do
    {
        printf("\nBenvenuto nella nostra banca!\n");
        printf("Cosa vuole fare?\n");
        printf("0 - Esci dal programma\n");
        printf("1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n");
        scanf("%d", &scelta);

        // Menu del programma
        if (scelta == 1)
        {
            Login(fpt, p1);
        }
        else if (scelta == 2)
        {
            Register(fpt, p1);
        }
        else
        {
            if (scelta == 0)
            {
                printf("Uscita in corso dal programma!");
            }
            else
            {
                printf("Valore non valido!");
                Sleep(2000);
                system("cls");
            }
        }
    } while (scelta != 0);

    return 0;
}