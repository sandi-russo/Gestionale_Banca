#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RWCFile.c"
#include "IBAN.c"
#include "Login.c"

// Dichiarazione variabili per il form di Login e di Registrazione
char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100], IBAN[28];

void Register()
{
    IsExists();       // Richiamo la funzione dal file RWCFile per controllare se esiste il file CSV
    GeneraIBAN(IBAN); // Richiamo la funzione dal file IBAN per generare un IBAN

    int FineRegistrazione = 0, controlloUtente = 0, Virgola = 0;
    // Inserimento dati utente per registrazione
    system("cls");
    printf("Compila il seguente form per aprire il conto!\n");

    do
    {
        Virgola = 0; // Inizializza la variabile a false
        printf("Inserisci il tuo nome: ");
        scanf("%s", &Nome);
        printf("Inserisci il tuo cognome: ");
        scanf("%s", &Cognome);
        printf("Inserisci il tuo nome utente: ");
        scanf("%s", NomeUtente);
        printf("Inserisci una password: ");
        scanf("%s", &Password);
        printf("Conferma la password: ");
        scanf("%s", &ConfermaPassword);
        int provaPassword = 0;
        provaPassword = strcmp(Password, ConfermaPassword);

        // Verifica se il nome utente contiene una virgola
        for (int i = 0; NomeUtente[i]; i++)
        {
            if (NomeUtente[i] == ',' || Nome[i] == ',' || Cognome[i] == ',' || Password[i] == ',' || ConfermaPassword[i] == ',')
            {
                Virgola = 1;
                break;
            }
            NomeUtente[i] = tolower(NomeUtente[i]);
        }
        if (provaPassword < 0)
        {
            printf("\nLe password non sono uguali.");
        }
        // Se il nome utente contiene una virgola, richiede di reinserire il valore
        if (Virgola)
        {
            printf("\nNon puoi inserire le virgole nel form. Reinserisci il valore.\n");
        }
        if (Virgola == 0 && provaPassword == 0)
        {
            FineRegistrazione = 1;
        }
    } while (Virgola || UserExists(NomeUtente) != 0 || FineRegistrazione != 1);
    Writing(Nome, Cognome, NomeUtente, Password, IBAN);
}

void Banca()
{
    int scelta;
    char utenteAutenticato[MAX_BUFFER];

    do
    {
        printf("\nBenvenuto nella nostra banca!\n");
        printf("Quale operazione desidera eseguire:\n");
        printf("0 - Esci dal programma\n");
        printf("1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n");
        scanf("%d", &scelta);

        // Menu del programma
        if (scelta == 1)
        {
            Login(utenteAutenticato);    // Richiamo la funzione Login dal file Login
            Consumer(utenteAutenticato); // Richiamo la funzione Consumer dal file Consumer
        }
        else if (scelta == 2)
        {
            Register();
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
}