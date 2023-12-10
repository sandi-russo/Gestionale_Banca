#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RWCFile.c"
#include "IBAN.c"

// Dichiarazione variabili per il form di Login e di Registrazione
char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100];


void Login(FILE *Utenti, char percorso[100])
{
    // Inserimento dati utente già registrato
    // Il system("cls") permette il clear della console su WINDOWS ma non in altri sistemi operativi. Se si vuole fare per altri sistemi si utilizza system("clear");
    system("cls");
    printf("Benvenuto nella schermata di login!\n");
    printf("Inserisci i tuoi dati per accedere.\n");
    printf("Nome Utente: ");
    scanf("%s", &NomeUtente);
    printf("Password: ");
    scanf("%s", &Password);

    printf("\nNu:%s, Pass:%s", NomeUtente, Password); // Debug per vedere se stampa i dati corretti
}

void Register(FILE *Utenti, char percorso[100])
{
    controlloFileX(Utenti, percorso); // Funzione che controlla se il file esiste. Guardare RWCFile.C!

    bool fineRegistrazione = false;
    bool controlloFineFile;
    char IBAN[28];
    GeneraIBAN(IBAN);
    
    // Inserimento dati utente per registrazione
    system("cls");
    printf("Compila il seguente form per aprire il conto!\n");
    printf("Inserisci il tuo nome: ");
    scanf("%s", &Nome);
    printf("Inserisci il tuo cognome: ");
    scanf("%s", &Cognome);

    do
    {
        printf("Inserisci il tuo nome utente: ");
        scanf("%s", &NomeUtente);

        // Ciclo per portare in corsivo il nome utente
        for (int i = 0; NomeUtente[i]; i++)
        {
            NomeUtente[i] = tolower(NomeUtente[i]);
        }
        controlloFineFile = rFile(Utenti, percorso, NomeUtente, "Nome utente gia' esistente!\n", 2);
    } while (controlloFineFile != false);

    /*
    Questo do while permette di controllare se la password e la conferma_password combaciano,
    quindi se entrambe combaciano, manda alla finestra di login.
    */
    do
    {
        printf("Inserisci una password: ");
        scanf("%s", &Password);
        printf("Conferma la password: ");
        scanf("%s", &ConfermaPassword);
        int provaPassword = strcmp(Password, ConfermaPassword);
        if (provaPassword)
        {
            printf("Le password non combaciano, reinseriscile!\n");
            Sleep(2000);
            system("cls");
        }
        else
        {
            fineRegistrazione = true;
            printf("Le password combaciano, stai per entrare nella sezione per il login!");
            cFileS5(Utenti, Nome, Cognome, NomeUtente, Password, IBAN, percorso);
            Sleep(2000);
            Login(Utenti, percorso);
        }
    } while (fineRegistrazione != true);

    // int provaPassword = strcmp(Password, ConfermaPassword);
    // printf("\nNome: %s; Cognome: %s; Nome utente: %s; Password: %s; Conferma: %s", Nome, Cognome, NomeUtente, Password, ConfermaPassword);
}


void Banca(){
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

}