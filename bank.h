#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define NUM_COLONNE 5
#define MAX_STR_LEN 100
#define MAX_UTENTI 255
#define FILE_NAME "Utenti.csv"
#define TEMP_FILE_NAME "Temp.csv"

// Dichiarazione variabili per il form di Login e di Registrazione
char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100], IBAN[28];

typedef struct // Sruttura contenente i dati del singolo utente
{
    char NomeUtente[MAX_STR_LEN];
    char Password[MAX_STR_LEN];
    char IBAN[MAX_STR_LEN];
    float Saldo;
    char Nome[MAX_STR_LEN];
    char Cognome[MAX_STR_LEN];
} utente;

void GeneraIBAN(char stringa[])
{
    int min = 10, max = 99;

    // Dichiarazione dei parametri che compongono un IBAN generico
    char it[] = "IT";                           // Sigla nazionale
    int cineu = rand() % (max - min + 1) + min; // Codice di controllo europeo
    char cin = 'A' + rand() % 26;               // Codice di controllo interno
    char abi[] = "07648";                       // Codice identificativo banca
    char cab[] = "05433";                       // Codice identificativo filiale
    char zeri[] = "00000";                      // Zeri identificaativi numero conto
    char conto[8];                              // Numero conto

    // Generazione casuale del numero conto
    for (int i = 0; i < 7; i++)
    {
        conto[i] = '0' + rand() % (10);
    }
    conto[7] = '\0';

    // Unione dei singoli elementi In un'unica stringa IBAN
    snprintf(stringa, 28, "%s%02d%c%s%s%s%s", it, cineu, cin, abi, cab, zeri, conto);
}

void Consumer(char *utenteAutenticato)
{
    printf("\nCiao, %s!\n", utenteAutenticato);
}

void Login(char *utenteAutenticato)
{
    FILE *file;
    char InputNomeUtente[MAX_STR_LEN], InputPassword[MAX_STR_LEN], riga[MAX_STR_LEN];
    int Trovato, tentativi = 5, daticorretti = 0;

    system("cls");

    while (!daticorretti && tentativi > 0)
    {
        // Stampa menu login
        printf("Benvenuti nella schermata di Login!\n");
        printf("\nInserisci il tuo nome utente: ");
        scanf("%s", InputNomeUtente);
        printf("Inserisci la tua password: ");
        scanf("%s", InputPassword);

        file = fopen(FILE_NAME, "r");

        if (file == NULL)
        {
            printf("Errore nella lettura del file %s", FILE_NAME);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        Trovato = 0;

        while (fgets(riga, MAX_STR_LEN, file) != NULL)
        {
            char StoredNomeUtente[MAX_STR_LEN];
            sscanf(riga, "%[^;]", StoredNomeUtente);

            if (strcmp(InputNomeUtente, StoredNomeUtente) == 0)
            {
                Trovato = 1;

                utente user;
                sscanf(riga, "%[^;];%[^;];%[^;];%f;%[^;];%[^;];",
                       user.NomeUtente, user.Password, user.IBAN,
                       &user.Saldo, user.Nome, user.Cognome);

                if (strcmp(InputPassword, user.Password) == 0)
                {
                    daticorretti = 1;
                    printf("Utente [%s] autenticato\n", user.NomeUtente);
                    strcpy(utenteAutenticato, user.NomeUtente);
                    break; // Esci dal ciclo quando l'utente è autenticato
                }
                else
                {
                    tentativi--;
                    printf("Password errata.");
                }
            }
        }

        fclose(file);

        if (!Trovato)
        {
            tentativi--;
            printf("L'utente [%s] non risulta registrato.\n", InputNomeUtente);
        }

        if (!daticorretti)
        {
            // Stampa avviso di numero di tentativi disponibili superato
            if (tentativi == 0)
            {
                printf("Troppi tentativi errati. Uscita in corso...\n");
            }
            else
            {
                printf("Tentativi rimasti: %d", tentativi);
            }
        }
        Sleep(2000);
        system("cls");
    }
}

void IsFileExists()
{

    FILE *file = fopen(FILE_NAME, "r"); // Apro il file in modalita' lettura

    if (file == NULL) // Controllo se non esiste il file
    {
        file = fopen(FILE_NAME, "w"); // Creo il file e lo apro in scrittura

        if (file != NULL)
        {
            fprintf(file, "NomeUtente;Password;IBAN;Saldo;Nome;Cognome;\n"); // Scrivo l'header
            fclose(file);                                                    // Chiudo il file dopo la scrittura
            // printf("Il file '%s' e' stato creato con successo.\n", FILE_NAME);
        }
        else
        {
            printf("Errore nella creazione del file.\n");
        }
    }
    else
    {
        fclose(file);
        // printf("Il file '%s' e' gia' esistente!\n", FILE_NAME);
    }
}

int UserExists(const char *InputNomeUtente)
{
    FILE *file = fopen(FILE_NAME, "r");
    char buffer[MAX_STR_LEN];

    if (file == NULL)
    {
        printf("Errore nell'apertura del file '%s'.\n", FILE_NAME);
        return 0; // Indica un errore nell'apertura del file
    }

    // Salta la prima riga contenente l'header
    fgets(buffer, MAX_STR_LEN, file);

    // Scansione del file riga per riga
    while (fgets(buffer, MAX_STR_LEN, file) != NULL)
    {
        char StoredNomeUtente[100];
        sscanf(buffer, "%[^;]", StoredNomeUtente);

        if (strcmp(InputNomeUtente, StoredNomeUtente) == 0)
        {
            printf("Nome utente gia' esistente!\n");
            return 1; // Indica che il nome utente esiste nel file
        }
    }

    fclose(file);
    return 0; // Indica che il nome utente non esiste nel file
}

void Writing(char *nome, char *cognome, char *nomeUtente, char *password, char *iban, float saldo)
{
    FILE *file = fopen(FILE_NAME, "a"); // Apri il file in modalità append

    if (file == NULL)
    {
        printf("Errore nell'apertura del file '%s' per la scrittura.\n", FILE_NAME);
        return;
    }

    fprintf(file, "%s;%s;%s;%0.2f;%s;%s;\n", nomeUtente, password, iban, saldo, nome, cognome);
    fclose(file);
}

void Register()
{
    IsFileExists();   // Richiamo la funzione per controllare se esiste il file CSV
    GeneraIBAN(IBAN); // Richiamo la funzione per generare un IBAN
    float Saldo = 0.00;
    int FineRegistrazione = 0, controlloUtente = 0, Delimiter, provaPassword;
    // Inserimento dati utente per registrazione

    do
    {
        system("cls");
        printf("Compila il seguente form per aprire il conto!\n");

        printf("Inserisci il tuo nome: ");
        scanf("%s", Nome);
        printf("Inserisci il tuo cognome: ");
        scanf("%s", Cognome);
        printf("Inserisci il tuo nome utente: ");
        scanf("%s", NomeUtente);
        printf("Inserisci una password: ");
        scanf("%s", Password);
        printf("Conferma la password: ");
        scanf("%s", ConfermaPassword);

        provaPassword = strcmp(Password, ConfermaPassword);

        // Inizializzo il flag Delimiter a fa
        Delimiter = 0;

        // Verifica se l'utentente inserisce un punto e virgola
        for (int i = 0; NomeUtente[i]; i++)
        {
            if (NomeUtente[i] == ';' || Nome[i] == ';' || Cognome[i] == ';' || Password[i] == ';' || ConfermaPassword[i] == ';')
            {
                Delimiter = 1;
                break;
            }
            NomeUtente[i] = tolower(NomeUtente[i]);
        }
        if (provaPassword != 0)
        {
            printf("\nLe password non sono uguali.\n");
        }
        // Se i dati inseriti dall'utente contengono il delimiter, richiede di reinserire i dati
        if (Delimiter)
        {
            printf("\nCarattere \";\" non valido. Riprova:\n");
        }
        if (Delimiter == 0 && provaPassword == 0)
        {
            FineRegistrazione = 1;
        }

    } while (Delimiter || UserExists(NomeUtente) != 0 || FineRegistrazione != 1);

    Writing(Nome, Cognome, NomeUtente, Password, IBAN, Saldo);

    printf("Registrazione avvenuta con successo! Reindirizzamento al menu in corso...");

    Sleep(2000);
    system("cls");
}

void Banca()
{
    int scelta;
    char utenteAutenticato[MAX_STR_LEN];

    do
    {
        system("cls");
        printf("Benvenuto nella nostra banca!\n");
        printf("\nMenu:\n");
        printf("0 - Esci dal programma\n");
        printf("1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n");
        printf("\nInserisci la tua scelta: ");

        // Controlla se l'input è un numero intero
        if (scanf("%d", &scelta) != 1)
        {
            // Pulisce l'input buffer
            while (getchar() != '\n')
                ;

            printf("Input non valido. Inserisci un numero.\n");

            Sleep(2000);
            system("cls");

            continue; // Rientra nel ciclo per ottenere un valore corretto
        }

        // Scelta dell'opzione
        if (scelta == 1)
        {
            Login(utenteAutenticato); // Richiamo la funzione Login
        }
        else if (scelta == 2)
        {
            Register(); // Richiamo la funzione Register
        }
        else if (scelta == 0)
        {
            printf("Uscita dal programma in corso...\n");
        }
        else
        {
            printf("Valore non inserito valido!\n"); // Gestisci input non validi

            Sleep(2000);
            system("cls");
        }
    } while (scelta != 0);
}