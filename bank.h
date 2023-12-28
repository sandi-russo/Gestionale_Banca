#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define NUM_COLONNE 5
#define MAX_STR_LEN 100
#define MAX_BUFFER_LEN 1024
#define MAX_UTENTI 255
#define FILE_NAME "Utenti.csv"
#define TEMP_FILE_NAME "Temp.csv"
#define ABI "07648"
#define CAB "05433"

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

FILE *ApriFile(const char *nomeFile, const char *modalita, const char *messaggioErrore)
{
    FILE *file = fopen(nomeFile, modalita);
    if (!file)
    {
        perror(messaggioErrore);
        exit(EXIT_FAILURE);
    }
    return file;
}

void ChiudiFile(FILE *file, const char *messaggioErrore)
{
    if (fclose(file) != 0)
    {
        perror(messaggioErrore);
        exit(EXIT_FAILURE);
    }
}

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

int SearchInFile(const char *colonna, const char *valore, char *rigaVuota)
{
    FILE *file = ApriFile(FILE_NAME, "r", "SearchFail: errore nell'apertura del file");

    char rigaTemp[MAX_STR_LEN];
    char riga[MAX_STR_LEN];
    char *token;
    int indiceColonna = -1, userExists = 0, columnExists = 0;

    // Leggi il file e cerca la colonna specificata
    if (fgets(riga, MAX_BUFFER_LEN, file) != NULL)
    {
        strcpy(rigaTemp, riga); // Copia la riga per non perdere dati con strtok
        token = strtok(rigaTemp, ";");
        for (int i = 0; token; i++)
        {
            if (strcmp(token, colonna) == 0)
            {
                indiceColonna = i;
                columnExists = 1;
                break;
            }
            token = strtok(NULL, ";");
        }
    }
    if (!columnExists)
    {
        ChiudiFile(file, "SearchFail: la colonna specificata non esiste nel file");
        return -1; // La colonna specificata non esiste nel file
    }

    // Leggi il file e cerca la riga contenente il valore specificato
    while (fgets(riga, MAX_BUFFER_LEN, file) != NULL)
    {
        strcpy(rigaTemp, riga); // Copia la riga per non perdere dati con strtok
        token = strtok(rigaTemp, ";");
        for (int i = 0; token; i++)
        {
            if (i == indiceColonna && strcmp(token, valore) == 0)
            {
                // Copia nella stringa vuota la riga contenente i dati dell'utente
                strcpy(rigaVuota, riga);
                ChiudiFile(file, "SearchFail: errore nella chiusura del file");
                return 0; // L'elemento è stato trovato
            }
            token = strtok(NULL, ";");
        }
    }

    ChiudiFile(file, "SearchFail: errore nella chiusura del file");
    return 1; // L'elemento non esiste nel file
}

utente CreateUserStruct(char *RigaUtente)
{
    utente user;
    sscanf(RigaUtente, "%[^;];%[^;];%[^;];%f;%[^;];%[^;];",
           user.NomeUtente, user.Password, user.IBAN,
           &user.Saldo, user.Nome, user.Cognome);

    return user;
}

void RemoveLine(const char *colonna, const char *valore)
{
    FILE *file = ApriFile(FILE_NAME, "r", "EditFail: errore nell'apertura del file originale");
    FILE *fileTemp = ApriFile(TEMP_FILE_NAME, "w", "EditFail: errore nell'apertura del file temporaneo");

    char riga[MAX_STR_LEN];
    char RigaUtente[MAX_STR_LEN];

    // Cerca la riga contenente il valore specificato nella colonna specificata
    if (SearchInFile(colonna, valore, RigaUtente) < 0)
    {
        printf("SearchFail: la colonna specificata non esiste nel file.\n");
        ChiudiFile(file, "EditFail: errore nella chiusura del file originale");
        ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo");
        return;
    }
    else if (SearchInFile(colonna, valore, RigaUtente) > 0)
    {
        printf("SearchFail: l'elemento specificato non esiste nella colonna indicata.\n");
        ChiudiFile(file, "EditFail: errore nella chiusura del file originale");
        ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo");
        return;
    }
    else
    {
        rewind(file);
        while (fgets(riga, MAX_BUFFER_LEN, file) != NULL)
        {
            if (strcmp(riga, RigaUtente) != 0)
            {
                fprintf(fileTemp, "%s", riga);
            }
        }
    }
    // Chiusura dei file dopo l'uso
    ChiudiFile(file, "EditFail: errore nella chiusura del file originale");
    ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo");
}

void FileUpdate(const char *OldFile, const char *NewFile) {
    // Elimina l'old file
    if (remove(OldFile) != 0) {
        perror("UpdateFail: Errore durante l'eliminazione del file [Utenti.csv] obsoleto");
        exit(EXIT_FAILURE);
    }

    // Rinomina il new file con il nome dell'old file
    if (rename(NewFile, OldFile) != 0) {
        perror("UpdateFail: Errore durante la rinomina del file [Temp.csv] in [Utenti.csv]");
        exit(EXIT_FAILURE);
    }
}

void Writing(char *NomeFile, const char *nomeUtente, const char *password, const char *iban, float saldo, const char *nome, const char *cognome)
{
    FILE *file = ApriFile(NomeFile, "a", "WriteFail: errore nell'apertura del file");

    fprintf(file, "%s;%s;%s;%0.2f;%s;%s;\n", nomeUtente, password, iban, saldo, nome, cognome);

    // Chiusura del file dopo l'uso
    ChiudiFile(file, "WriteFail: errore nella chiusura del file");
}

void Transazione(utente *user, float importo)
{
    user->Saldo += importo;
    RemoveLine("NomeUtente", user->NomeUtente);
    Writing(TEMP_FILE_NAME, user->NomeUtente, user->Password, user->IBAN, user->Saldo, user->Nome, user->Cognome);
}

void Consumer(utente user)
{
    int scelta;
    float importo = 0;

    do // Ciclo di visualizzazione menu
    {   
        Sleep(2000);
        system("cls");

        printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
        printf("----------------------------------------------------------------\n");
        printf("Menu:\n");
        printf("\n1 - Effettua un versamento\n");
        printf("2 - Evvettua un prelievo\n");
        printf("3 - Effettua un bonifico\n");
        printf("4 - Visualizza le tue coordinate bancarie\n");
        printf("5 - Chiudi il tuo conto\n");
        printf("\n0 - Esci dal conto\n");
        printf("----------------------------------------------------------------\n");
        printf("\nInserisci la tua scelta: ");

        // Controlla se l'input è un numero intero
        if (scanf("%d", &scelta) != 1)
        {
            // Pulisce l'input buffer
            while (getchar() != '\n');
                printf("Il valore inserito non e' valido. Perfavore riprova\n");

                printf("\nCaricamento in corso...\n");
                Sleep(2000);
                system("cls");

            continue; // Rientra nel ciclo per ottenere un valore corretto
        }

        // Scelta dell'opzione
        if (scelta == 1)
        {   
            printf("\nCaricamento in corso...\n");
            Sleep(2000);
            system("cls");
            printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
            printf("----------------------------------------------------------------\n");
            printf("Iserisci l'importo da versare: ");
            scanf("%f", &importo);
            Transazione(&user, importo);
            printf("\nVersamento effettuato. L'importo versato sara' disponibile immediatamente\n");
        }
        else if (scelta == 2)
        {   
            printf("\nCaricamento in corso...\n");
            Sleep(2000);
            system("cls");
            printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
            printf("----------------------------------------------------------------\n");
            printf("Iserisci l'importo da prelevare: ");
            scanf("%f", &importo);
            if (importo <= user.Saldo)
            {
                Transazione(&user, -importo);
                printf("\nPrelievo effettuato. Ritira i contanti dallo sportello in basso");
            }
            else
            {
                printf("\nSaldo insufficiente");
            }
        }
        else if (scelta == 4)
        {   
            int scelta2;
            printf("\nCaricamento in corso...\n");
            do
            {   
                Sleep(2000);
                system("cls");
                printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
                printf("----------------------------------------------------------------\n");
                printf("Nome utente: %s\n", user.NomeUtente);
                printf("IBAN: %s\n", user.IBAN);
                printf("Pasese: IT\n");
                printf("ABI: %s\n", ABI);
                printf("CAB: %s\n", CAB);
                printf("BIC/SWIFT: UNCODMMXXX\n");
                printf("\n0 - Torna al conto\n");
                printf("----------------------------------------------------------------\n");
                printf("\nInserisci la tua scelta: ");

                if (scanf("%d", &scelta2) != 1)
                {
                    // Pulisce l'input buffer
                    while (getchar() != '\n');
                        printf("\nIl valore inserito non e' valido. Inserisci [ 0 ] per tornare al conto\n");
                    continue; // Rientra nel ciclo per ottenere un valore corretto
                }
                if (scelta2 != 0)
                {
                    printf("\nIl valore inserito non e' valido. Inserisci [ 0 ] per tornare al conto\n");
                }
            } while (scelta2 != 0);
            printf("\nCaricamento in corso...\n");
        }   
        else if (scelta == 0)
        {
            printf("\nReindirizzamento al menu in corso...\n");
        }
        else
        {
            printf("\nIl valore inserito non e' valido. Perfavore riprova\n"); // Gestisci input non validi
        }
    } while (scelta != 0);
}

void Login()
{
    FILE *file = ApriFile(FILE_NAME, "r", "LoginFail: errore nell'apertura del file");

    char InputNomeUtente[MAX_STR_LEN], InputPassword[MAX_STR_LEN], RigaUtente[MAX_STR_LEN];
    int Trovato, tentativi = 5, daticorretti = 0;

    while (!daticorretti && tentativi > 0) // Ciclo visualizzazione menu di autenticazione
    {   
        printf("\nCaricamento in corso...\n");
        Sleep(2000);
        system("cls");
        printf("Accedi con le tue credenziali | Tentativi rimasti: %d\n", tentativi);
        printf("----------------------------------------------------------------\n");
        printf("Inserisci il tuo nome utente: ");
        scanf("%s", InputNomeUtente);
        printf("Inserisci la tua password: ");
        scanf("%s", InputPassword);

        // Cerca la riga contenente il valore specificato nella colonna specificata
        if (SearchInFile("NomeUtente", InputNomeUtente, RigaUtente) > 0)
        { // Username non esistente nel file
            printf("Username errato. Riprova\n");
            tentativi--;
        }
        else
        { // Se esiste l'utente, salvalo nella struttura
            utente user = CreateUserStruct(RigaUtente);

            if (strcmp(user.Password, InputPassword) == 0)
            { // Se la password e' corretta, entra nel conto
                daticorretti = 1;
                printf("\nUtente [ %s ] autenticato con successo\n\nIngresso nel conto...\n", user.NomeUtente);
                Consumer(user);
                FileUpdate(FILE_NAME, TEMP_FILE_NAME);
            }   
            else
            {
                printf("\nPassword errata. Riprova\n");
                tentativi--;
            }
        }

        if (!daticorretti)
        {
            if (tentativi == 0)
            { // Stampa avviso di numero di tentativi disponibili superato
                printf("\nTroppi tentativi errati\n\nUscita in corso...\n");
            }
        }
    }
}

void IsFileExists()
{
    FILE *file = ApriFile(FILE_NAME, "r", "CreateFail: errore nell'apertura del file");

    if (file == NULL) // Se il file non esiste, crealo
    {
        FILE *file = ApriFile(FILE_NAME, "w", "CreateFail: errore nella creazione del file del file");

        if (file != NULL) // Se il file e' stato appena creato, stampa l'header
        {
            fprintf(file, "NomeUtente;Password;IBAN;Saldo;Nome;Cognome;\n");
        }
    }
    ChiudiFile(file, "CreateFail: errore nella chiusura del file");
}

int UserExists(const char *InputNomeUtente)
{
    FILE *file = ApriFile(FILE_NAME, "r", "UserFail: errore nell'apertura del file");
    char buffer[MAX_BUFFER_LEN];

    // Salta la prima riga contenente l'header
    fgets(buffer, MAX_BUFFER_LEN, file);

    // Scansione del file riga per riga
    while (fgets(buffer, MAX_BUFFER_LEN, file) != NULL)
    {
        char StoredNomeUtente[100];
        sscanf(buffer, "%[^;]", StoredNomeUtente);

        if (strcmp(InputNomeUtente, StoredNomeUtente) == 0)
        {
            printf("\nUtente [ %s ] gia' registrato. Riprova\n", InputNomeUtente);
            ChiudiFile(file, "UserFail: errore nella chiusura del file");
            return 1; // Indica che il nome utente esiste nel file
        }
    }

    ChiudiFile(file, "UserFail: errore nella chiusura del file");
    return 0; // Indica che il nome utente non esiste nel file
}

void Register()
{
    IsFileExists();   // Richiamo la funzione per controllare se esiste il file CSV
    GeneraIBAN(IBAN); // Richiamo la funzione per generare un IBAN
    float Saldo = 0.00;
    int FineRegistrazione = 0, controlloUtente = 0, Delimiter, provaPassword;

    do // Ciclo visualizzazione menu registrazione
    {   
        printf("\nCaricamento in corso...\n");
        Sleep(2000);
        system("cls");

        printf("Compila il seguente form per aprire il conto\n");

        printf("Inserisci il tuo nome: ");
        scanf("%s", Nome);
        printf("Inserisci il tuo cognome: ");
        scanf("%s", Cognome);
        printf("Inserisci un nome utente: ");
        scanf("%s", NomeUtente);
        printf("Inserisci una password: ");
        scanf("%s", Password);
        printf("Conferma la tua password: ");
        scanf("%s", ConfermaPassword);

        provaPassword = strcmp(Password, ConfermaPassword);

        // Inizializzo il flag Delimiter a falso
        Delimiter = 0;

        // Verifica se l'utentente inserisce un punto e virgola
        for (int i = 0; NomeUtente[i]; i++)
        {   
            NomeUtente[i] = tolower(NomeUtente[i]);

            if (NomeUtente[i] == ';' || Nome[i] == ';' || Cognome[i] == ';' || Password[i] == ';' || ConfermaPassword[i] == ';')
            {
                Delimiter = 1;
                break;
            }
        }
        if (provaPassword != 0)
        {
            printf("\nLe due password non corrispondono\n");
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

    Writing(FILE_NAME, NomeUtente, Password, IBAN, Saldo, Nome, Cognome);

    printf("\nRegistrazione avvenuta con successo\n\nReindirizzamento al menu in corso...\n");
}

void Banca()
{
    int scelta;

    Sleep(2000);
    system("cls");
    do // Ciclo visualizzazione menu banca
    {
        printf("Benvenuto nella UniCode Bank\n");
        printf("----------------------------------------------------------------\n");
        printf("Menu:\n");
        printf("\n1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n");
        printf("\n0 - Esci dal programma\n");
        printf("----------------------------------------------------------------\n");
        printf("\nInserisci la tua scelta: ");

        // Controlla se l'input è un numero intero
        if (scanf("%d", &scelta) != 1)
        {
            // Pulisce l'input buffer
            while (getchar() != '\n');
            printf("\nIl valore inserito non e' valido. Perfavore riprova\n");

            printf("\nCaricamento in corso...\n");
            Sleep(2000);
            system("cls");

            continue; // Rientra nel ciclo per ottenere un valore corretto
        }

        // Scelta dell'opzione
        if (scelta == 1)
        {
            Login(); // Richiamo la funzione Login
        }
        else if (scelta == 2)
        {
            Register(); // Richiamo la funzione Register
        }
        else if (scelta == 0)
        {
            printf("\nUscita dal programma in corso...\n");
        }
        else
        {
            printf("\nIl valore inserito non e' valido. Perfavore riprova\n"); // Gestisci input non validi
        }
        Sleep(2000);
        system("cls");
    } while (scelta != 0);
}