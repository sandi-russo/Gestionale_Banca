#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_COLONNE 5
#define LunghezzaStringa 100
#define NumeroUtenti 255

static const int maxchar = 100;
typedef struct
{
    char Nome[LunghezzaStringa];
    char Cognome[LunghezzaStringa];
    char NomeUtente[LunghezzaStringa];
    char Password[LunghezzaStringa];
    char IBAN[LunghezzaStringa];
} utente;

int Login()
{
    FILE *fp;
    char row[maxchar];
    int i = 0;

    fp = fopen("Utenti.csv", "r"); // Apro il file csv in lettura

    utente u[NumeroUtenti]; // Dichiarazione di un array di strutture utente

    while (fgets(row, maxchar, fp) != NULL)
    {

        if (i == 0)
        { // Salto la prima riga "header"
            i++;
            continue;
        }

        printf("%d riga: %s", i, row);

        // Utilizza sscanf per assegnare i valori ai campi della struttura
        sscanf(row, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
               u[i].Nome,
               u[i].Cognome,
               u[i].NomeUtente,
               u[i].Password,
               u[i].IBAN);

        i++;
    }

    fclose(fp);

    // if (i > 0) return 0;
    //  {
    //      // Stampa le informazioni degli utenti
    //      for (int j = 1; j < i; j++)
    //      {
    //          printf("\n--------------------\n");
    //          printf("Nome: %s\n", u[j].Nome);
    //          printf("Cognome: %s\n", u[j].Cognome);
    //          printf("Nome Utente: %s\n", u[j].NomeUtente);
    //          printf("Password: %s\n", u[j].Password);
    //          printf("IBAN: %s\n", u[j].IBAN);
    //      }
    //  }
    // else return 1;
    //  {
    //      printf("Nessun utente trovato nel file.\n");
    //  }

    char nomeutente[100], password[100];
    int combacia = 0, tentativi = 0;

    // Inserimento dati utente già registrato
    // Il system("cls") permette il clear della console su WINDOWS ma non in altri sistemi operativi. Se si vuole fare per altri sistemi si utilizza system("clear");
    system("cls");
    printf("Benvenuto nella schermata di login!\n");

    do
    {
        printf("Inserisci i tuoi dati per accedere.\n");
        printf("Nome Utente: ");
        scanf("%s", &nomeutente);
        printf("Password: ");
        scanf("%s", &password);

        combacia = 0;

        for (int j = 1; j < i; j++)
        {
            if ((strcmp(nomeutente, u[j].NomeUtente) == 0) && (strcmp(password, u[j].Password) == 0))
            {
                printf("I dati combaciano.\n");
                combacia = 1;
                break;
            }
        }

        if (combacia == 0)
        {
            tentativi++;

            if (tentativi < 5)
            {
                printf("\nI dati non combaciano. Riprova.\n");
                Sleep(2000);
                system("cls");
            }
            else
            {
                printf("Mi dispiace ma hai sbagliato %d volte!", tentativi);
                Sleep(2000);
                system("cls");
                break; // Esci dal ciclo se il numero di tentativi raggiunge 5
            }
        }

    } while (combacia != 1 && tentativi != 5);
}
