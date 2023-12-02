#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Windows.h>
#include "RWCFile.c"

// Dichiarazione variabili per il form di Login e di Registrazione
char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100];

void Login(FILE *Utenti, char percorso[100])
{
    // Inserimento dati utente già registrato
    // Il system("cls") permette il clear della console su WINDOWS! E non su linux o su altri sistemi operativi. Se si vuole fare per altri sistemi si utilizza system("clear");
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
    controlloFileX(Utenti, percorso); //Funzione che controlla se il file esiste. Guardare CFileScrittura.C!

    bool fineRegistrazione = false;

    // Iserimento dati utente per registrazione
    system("cls");
    printf("Compila il seguente form per aprire il conto!\n");
    printf("Inserisci il tuo nome: ");
    scanf("%s", &Nome);
    printf("Inserisci il tuo cognome: ");
    scanf("%s", &Cognome);
    printf("Inserisci il tuo nome utente: ");
    scanf("%s", &NomeUtente);
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
            cFileS5(Utenti, Nome, Cognome, NomeUtente, Password, ConfermaPassword, percorso);
            Sleep(2000);
            Login(Utenti, percorso);
        }
    } while (fineRegistrazione != true);

    // int provaPassword = strcmp(Password, ConfermaPassword);
    // printf("\nNome: %s; Cognome: %s; Nome utente: %s; Password: %s; Conferma: %s", Nome, Cognome, NomeUtente, Password, ConfermaPassword);
}