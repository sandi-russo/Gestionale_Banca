#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define NUM_COLONNE 5
#define LunghezzaStringa 100
#define NumeroUtenti 255

typedef struct
{
    char Nome[LunghezzaStringa];
    char Cognome[LunghezzaStringa];
    char NomeUtente[LunghezzaStringa];
    char Password[LunghezzaStringa];
    char IBAN[LunghezzaStringa];
} utente;

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

static const int maxchar = 100;

void Consumer(char *utenteAutenticato)
{
    printf("\nCiao, %s!\n", utenteAutenticato);
}

int Login(char *utenteAutenticato)
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
                strcpy(utenteAutenticato, u[j].NomeUtente); // Memorizza il nome utente autenticato
                Consumer(utenteAutenticato);
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

#define MAX_BUFFER 100
const char *filename = "Utenti.csv";

void IsExists()
{

    FILE *file = fopen(filename, "r");

    if (file == NULL) // Controllo se non esiste il file
    {
        file = fopen(filename, "w"); // Creo il file e lo apro in scrittura

        if (file != NULL)
        {
            fprintf(file, "Nome,Cognome,NomeUtente,Password,IBAN\n"); // Scrivo l'header
            fclose(file);                                             // Chiudo il file dopo la scrittura
            printf("Il file '%s' e' stato creato con successo.\n", filename);
        }
        else
        {
            printf("Errore nella creazione del file.\n");
        }
    }
    else
    {
        fclose(file);
        printf("Esiste il file '%s' e non lo creo.\n", filename);
    }
}

int UserExists(const char *inputUtente)
{
    FILE *file = fopen(filename, "r");
    char buffer[MAX_BUFFER];

    if (file == NULL)
    {
        printf("Errore nell'apertura del file '%s'.\n", filename);
        return 0; // Indica un errore nell'apertura del file
    }

    // Salta la prima riga contenente l'header
    fgets(buffer, MAX_BUFFER, file);

    // Scansione del file per controllare se il nome utente esiste già
    while (fgets(buffer, MAX_BUFFER, file) != NULL)
    {
        char storedNomeUtente[100];
        sscanf(buffer, "%*[^,],%*[^,],%[^,]", storedNomeUtente);

        if (strcmp(inputUtente, storedNomeUtente) == 0)
        {
            fclose(file);
            printf("Nome utente gia' esistente!\n");
            return 1; // Indica che il nome utente esiste già nel file
        }
    }

    fclose(file);
    return 0; // Indica che il nome utente non esiste nel file
}

void Writing(char *nome, char *cognome, char *nomeUtente, char *password, char *iban)
{
    FILE *file = fopen(filename, "a"); // Apre il file in modalità append

    if (file == NULL)
    {
        printf("Errore nell'apertura del file '%s' per la scrittura.\n", filename);
        return;
    }

    fprintf(file, "%s,%s,%s,%s,%s\n", nome, cognome, nomeUtente, password, iban);

    fclose(file);

    printf("Registrazione completata con successo!\n");
}

// Dichiarazione variabili per il form di Login e di Registrazione
char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100], IBAN[28];

void Register()
{
    IsExists();       // Richiamo la funzione per controllare se esiste il file CSV
    GeneraIBAN(IBAN); // Richiamo la funzione per generare un IBAN

    int FineRegistrazione = 0, controlloUtente = 0, Virgola = 0;
    // Inserimento dati utente per registrazione
    system("cls");
    printf("Compila il seguente form per aprire il conto!\n");

    do
    {
        Virgola = 0; // Inizializza la variabile a false
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
            Login(utenteAutenticato); // Richiamo la funzione Login
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

// void FileUpdate(char token[100], int l)
// { // Il parametro char d serve per ottenere le informazioni che devono essere controllate nel file
//     char buffer[1000], *delimeter, *gAppoggio;
//     int colonne = 0, comparazioneStringhe;
//     FILE *temp;

//     f = fopen("Utenti.csv", "r"); // Apre il file in modalità lettura

//     temp = fopen("Temp.csv", "w"); // Apro il file temporaneo

//     while (fgets(buffer, sizeof(buffer), f) != NULL)
//     {                // Cicla per tutto il file
//         colonne = 0; // Rinizializza la colonna a zero dopo che finisce di comparare la terza colonna

//         delimeter = strtok(buffer, ",");
//         while (delimeter)
//         {
//             if (colonne == l)
//             {
//                 gAppoggio = delimeter;                           // Il token della terza colonna viene assegnato ad una variabile di appoggio
//                 comparazioneStringhe = strcmp(token, gAppoggio); // Compara il token con la stringa in entrata
//                 if (comparazioneStringhe)                        // Se vero
//                 {
//                     // Diversi
//                     fputs(buffer, temp);
//                 }
//             }
//             delimeter = strtok(NULL, ",");
//             colonne++;
//         }
//     }
//     fclose(f);     // Chiude il file
//     fclose(temp); // Chiude il file temporaneo
// }
