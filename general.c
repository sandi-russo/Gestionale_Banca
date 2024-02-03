#include "Main.h"

#ifndef GENERAL_C // Include guard per evitare inclusioni multiple
#define GENERAL_C
/*
    La funzione serve a pulire eventuali buffer rimasti in input da tastiera tramite
    getchar()
*/
void pulisciBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/*
    Primo parametro: array di caratteri (Messaggio di input)
    Secondo parametro: intero (Valore Minimo)
    Terzo parametro: intero (Valore Massimo)

    La funzione stampa il messaggio e prende in input il valore dell'utente
    verificando che sia un intero compreso tra i valori specificati.
    La funzione e' ottimizzata per numeri naturali positivi.

    Return: int input se il valore inserito e' corretto
    Return: int -1 se il valore inserito non e' corretto
*/
int ReadInputChoice(const char *messaggio, int minimo, int massimo)
{
    int input;
    char buffer[MAX_STR_LEN];
    int conversione;

    do
    {
        printf("%s", messaggio);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Errore nella lettura dell'input.\n");
            exit(EXIT_FAILURE);
        }

        conversione = sscanf(buffer, "%d", &input);

        if (conversione != 1 || buffer[0] == '\n')
        {
            printf("Inserire un numero intero valido\n");
            return -1;
        }
        else if (input < minimo || input > massimo)
        {
            if (massimo != minimo)
            {
                printf("Inserire un numero compreso tra %d e %d\n", minimo, massimo);
            }
            else
            {
                printf("Inserisci %d per continuare\n", minimo);
            }

            return -1;
        }
    } while (conversione != 1 || input < minimo || input > massimo);

    return input;
}

/*
    Primo parametro: array di caratteri (Nome del File)
    Secondo parametro: array di caratteri (Modalita' di Apertura)
    Terzo parametro: array di caratteri (Messaggio di Errore)

    La funzione apre il file con il nome e modalita' specificate.
    In caso di errore, stampa il messaggio specificato.
*/
FILE *ApriFile(const char *nomeFile, const char *modalita, const char *messaggioErrore)
{
    FILE *file = fopen(nomeFile, modalita); // Apertura file
    if (!file)                              // Se il file non si è aperto, mostra un determinato messaggio di errore
    {
        printf("\033[0;31m");    // Sequenza di escape per il colore rosso
        perror(messaggioErrore); // Stampiamo un messaggio prima del codice di errore stesso
        printf("\033[0m");       // Sequenza di escape per il colore predefinito
        exit(EXIT_FAILURE);
    }
    return file;
}

/*
    Primo parametro: puntatore a file (file aperto)
    Secondo parametro: array di caratteri (Messaggio di Errore)

    La funzione chiudee il file e in caso di errore stampa il messaggio specificato.
*/
void ChiudiFile(FILE *file, const char *messaggioErrore)
{
    if (fclose(file) != 0) // Se il file non si è chiuso, mostra un determinato messaggio di errore
    {
        printf("\033[0;31m");    // Sequenza di escape per il colore rosso
        perror(messaggioErrore); // Stampiamo un messaggio prima del codice di errore stesso
        printf("\033[0m");       // Sequenza di escape per il colore predefinito
        exit(EXIT_FAILURE);
    }
}

/*
    La funzione verifica se esiste il file FILE_NAME tramite la modalita' "r".
    Se non esiste, allora crea il file tramite la modalita' "w" scrivendo al
    suo interno le due righe di intestazione (header e admin).
*/
void IsFileExists()
{
    FILE *file = ApriFile(FILE_NAME, "r", "CreateFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'

    if (file == NULL) // Se il file non esiste, crealo
    {
        FILE *file = ApriFile(FILE_NAME, "w", "CreateFail: errore nella creazione del file del file"); // Apertura file tramite funzione 'ApriFile'

        if (file != NULL) // Se il file e' stato appena creato, stampa l'header
        {
            fprintf(file, "NomeUtente;Password;IBAN;Saldo;Nome;Cognome;\n");
            fprintf(file, "admin_code;@UniCode2024;IT00A0000000000000000000000;0.00;Admin;Admin;\n");
        }
    }
    ChiudiFile(file, "CreateFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
}

/*
    Parametro: array di caratteri (Nome Utente)

    La funzione apre il file principale in modalita "r" e cerca nella sua prima colonna il nome utente
    inserito come parametro.

    Return: int 1 se il nome utente e' stato trovato
    Return: int 0 se il nome utente non esiste nel file
*/
int UserExists(const char *InputNomeUtente)
{
    FILE *file = ApriFile(FILE_NAME, "r", "UserFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'
    char buffer[MAX_BUFFER_LEN];

    fgets(buffer, MAX_BUFFER_LEN, file); // Salta la prima riga contenente l'header

    while (fgets(buffer, MAX_BUFFER_LEN, file) != NULL) // Scansione del file riga per riga
    {
        char StoredNomeUtente[100];
        sscanf(buffer, "%[^;]", StoredNomeUtente);

        if (strcmp(InputNomeUtente, StoredNomeUtente) == 0)
        {
            printf("\033[0;31m"); // Sequenza di escape per il colore rosso
            printf("\nUtente [%s] gia' registrato. Riprova\n", InputNomeUtente);
            printf("\033[0m");                                            // Sequenza di escape per il colore predefinito
            ChiudiFile(file, "UserFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
            return 1;                                                     // Indica che il nome utente esiste nel file
        }
    }

    ChiudiFile(file, "UserFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
    return 0;                                                     // Indica che il nome utente non esiste nel file
}

/*
    Primo parametro: array di caratteri (nome della colonna del file principale)
    Secondo parametro: array di caratteri (valore UNIVOCO della colonna specificata)
    Terzo parametro: array di caratteri vuoto (per contenere la riga dell'utente)

    La funzione cerca all'interno del file principale la colonna specificata dall'utente. Se esiste la colonna,
    cerca il valore inserito. Trovato il valore, copia all'interno dell'array vuoto (terzo parametro) l'intera riga
    del file csv associata.

    Return: int 0 se l'elemento è stato trovato e la stringa è stata copiata correttamente;
    Return: int -1 se la colonna specificata non esiste;
    Return: int 1 se l'elemento specificato non esiste nella colonna.
*/
int SearchInFile(const char *colonna, const char *valore, char *rigaVuota)
{
    FILE *file = ApriFile(FILE_NAME, "r", "SearchFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'
    char rigaTemp[MAX_STR_LEN];                                                         // Dichiaro la riga temporanea dove inserire il valore temporaneo
    char riga[MAX_STR_LEN];                                                             // Dichiaro riga dove ci sarà il valore originale
    char *token;                                                                        // Dichiaro il delimitatore
    int indiceColonna = -1, userExists = 0, columnExists = 0;
    if (fgets(riga, MAX_BUFFER_LEN, file) != NULL) // Leggi il file e cerca la colonna specificata
    {
        strcpy(rigaTemp, riga);        // Copia la riga per non perdere dati con strcpy
        token = strtok(rigaTemp, ";"); // Assegno il delimitatore
        for (int i = 0; token; i++)
        {
            if (strcmp(token, colonna) == 0) // Controlla se la comparazione tra il token e la colonna è uguale a 0 (è vera)
            {
                indiceColonna = i; // Assegniamo l'indice all'indice della colonna
                columnExists = 1;  // Portiamo a true (1) il valore di 'columnExists
                break;             // Se trova la colonna, ferma il ciclo
            }
            token = strtok(NULL, ";"); // Se non trova la colonna, andiamo a specificargli il delimitatore da trovare
        }
    }
    if (!columnExists) // Se la colonna non è stata trovata
    {
        ChiudiFile(file, "SearchFail: la colonna specificata non esiste nel file"); // Chiusura file tramite funzione 'ChiudiFile'
        return -1;                                                                  // Ritorna -1
    }
    while (fgets(riga, MAX_BUFFER_LEN, file) != NULL) // Leggi il file e cerca la riga contenente il valore specificato
    {
        strcpy(rigaTemp, riga);        // Copia la riga per non perdere dati con strcpy
        token = strtok(rigaTemp, ";"); // Assegno il delimitatore
        for (int i = 0; token; i++)
        {
            if (i == indiceColonna && strcmp(token, valore) == 0)
            {
                strcpy(rigaVuota, riga);                                        // Copia nella stringa vuota la riga contenente i dati dell'utente
                ChiudiFile(file, "SearchFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
                return 0;                                                       // L'elemento è stato trovato
            }
            token = strtok(NULL, ";"); // Se non trova la colonna, andiamo a specificargli il delimitatore da trovare
        }
    }
    ChiudiFile(file, "SearchFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
    return 1;                                                       // L'elemento non esiste nel file
}

/*
    Primo parametro: array di caratteri (nome della colonna nel file principale)
    Secondo parametro: array di caratteri (valore UNIVOCO della colonna specificata)

    La funzione apre il file principale in modalita' "r" e cerca al suo interno il valore specificato tramite
    la funzione SearchInFile. Se trovato, apre in modalita' "w" un file temporaneo, scrivendo al suo interno
    tutte le righe contenute nel file principale, esclusa quella contenente il valore.
*/
void RemoveLine(const char *colonna, const char *valore)
{
    FILE *file = ApriFile(FILE_NAME, "r", "EditFail: errore nell'apertura del file originale");           // Apertura file tramite funzione 'ApriFile'
    FILE *fileTemp = ApriFile(TEMP_FILE_NAME, "w", "EditFail: errore nell'apertura del file temporaneo"); // Apertura file tramite funzione 'ApriFile'
    char riga[MAX_STR_LEN];
    char RigaUtente[MAX_STR_LEN];
    if (SearchInFile(colonna, valore, RigaUtente) < 0) // Cerca la riga contenente il valore specificato nella colonna specificata
    {
        printf("\033[0;31m"); // Sequenza di escape per il colore rosso
        printf("SearchFail: la colonna specificata non esiste nel file.\n");
        printf("\033[0m");                                                           // Sequenza di escape per il colore predefinito
        ChiudiFile(file, "EditFail: errore nella chiusura del file originale");      // Chiusura file tramite funzione 'ChiudiFile'
        ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo"); // Chiusura file tramite funzione 'ChiudiFile'
        return;
    }
    else if (SearchInFile(colonna, valore, RigaUtente) > 0)
    {
        printf("\033[0;31m"); // Sequenza di escape per il colore rosso
        printf("SearchFail: l'elemento specificato non esiste nella colonna indicata.\n");
        printf("\033[0m");                                                           // Sequenza di escape per il colore predefinito
        ChiudiFile(file, "EditFail: errore nella chiusura del file originale");      // Chiusura file tramite funzione 'ChiudiFile'
        ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo"); // Chiusura file tramite funzione 'ChiudiFile'
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
    ChiudiFile(file, "EditFail: errore nella chiusura del file originale");      // Chiusura file tramite funzione 'ChiudiFile'
    ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo"); // Chiusura file tramite funzione 'ChiudiFile'
}

/*
    Primo parametro: array di caratteri (nome file originale)
    Secondo parametro: array di caratteri (nome file temporaneo)

    La funzione apre in modalita' "r" il file temporaneo (secondo parametro).
    Se esiste, ELIMINA il file originale (primo parametro) sostituendolo con il file temporaneo.
*/
void FileUpdate(const char *OldFile, const char *NewFile)
{
    FILE *NewFileT = ApriFile(OldFile, "r", "UpdateFail: Errore durante l'apertura del file");
    bool aperturaFile = false;
    int rimozione, rinomina;
    if (!NewFileT) // Controlla se il file temporaneo esiste
    {
        aperturaFile = false; // Il file temporaneo non esiste
    }
    else // Il file temporaneo esiste
    {
        aperturaFile = true;
        ChiudiFile(NewFileT, "UpdateFail: Errore durante la chiusura del file");
    }
    if (aperturaFile == true) // Se esiste il file temporaneo
    {
        rimozione = remove(OldFile);         // Elimina il file originale
        rinomina = rename(NewFile, OldFile); // Sostituisce il file originale con il file temporaneo
        aperturaFile = false;
    }
    if ((rimozione < 0) || (rinomina < 0)) // Gestisce gli errori di remove e rename
    {
        printf("\033[0;31m"); // Sequenza di escape per il colore rosso
        perror("UpdateFail: Errore durante la modifica del file [utenti.csv]");
        printf("\033[0m"); // Sequenza di escape per il colore predefinito
        exit(EXIT_FAILURE);
    }
}

/*
    Primo parametro: array di caratteri (Nome del File)
    Secondo parametro: array di caratteri (Nome Utente)
    Terzo parametro: array di caratteri (Password)
    Quarto parametro: array di caratteri (IBAN)
    Quinto parametro: float (Saldo)
    Sesto parametro: array di caratteri (Nome)
    Settimo parametro: array di caratteri (Cognome)

    La funzione apre il file con il nome specificato in modalita' "a" e scrive al suo interno
    una nuova riga utente contenente i parametri specificati nel loro ordine e separati da ";".
*/
void Writing(char *NomeFile, const char *nomeUtente, const char *password, const char *iban, float saldo, const char *nome, const char *cognome)
{
    FILE *file = ApriFile(NomeFile, "a", "WriteFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'
    fprintf(file, "%s;%s;%s;%0.2f;%s;%s;\n", nomeUtente, password, iban, saldo, nome, cognome);
    ChiudiFile(file, "WriteFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
}

/*
    Primo parametro: array di caratteri (nome del file)
    Secondo parametro: struttura di tipo "utente"

    La funzione apre il file con il nome specificato in modalita "a".
    Successivamente, tramite fprintf(), scrive al suo interno i singoli
    campi della struttura separati da ";" per poi chiudere il file.
*/
void WritingStruct(char *NomeFile, utente *user)
{
    FILE *file = ApriFile(NomeFile, "a", "WriteFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'
    fprintf(file, "%s;%s;%s;%0.2f;%s;%s;\n", user->NomeUtente, user->Password, user->IBAN, user->Saldo, user->Nome, user->Cognome);
    ChiudiFile(file, "WriteFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
}

/*
    Parametro: un array di caratteri (riga contenente i dati dell'utente)

    La funzione analizza questa riga identificando i campi separati dal carattere ";" e inserisce i
    valori corrispondenti nei campi appropriati della struttura "utente".

    Return: la funzione ritorna la struttura di tipo "utente" completa di dati.
*/
utente CreateUserStruct(char *RigaUtente)
{
    utente user;
    sscanf(RigaUtente, "%[^;];%[^;];%[^;];%f;%[^;];%[^;];",
           user.NomeUtente, user.Password, user.IBAN,
           &user.Saldo, user.Nome, user.Cognome);
    return user;
}
/*
    La funzione conta le righe contenute all'interno del file utenti principale, non tenendo
    conto delle righe di intestazione (header e admin).

    Return: int numero di utenti
*/
int CountUserNum()
{
    FILE *file = ApriFile(FILE_NAME, "r", "CountFail: Errore nell'apertura del file");

    char line[MAX_STR_LEN];
    int numero_utenti = 0;

    // Ignora la prima riga (intestazione)
    if (fgets(line, sizeof(line), file) == NULL)
    {
        printf("Errore nella lettura del file %s.\n", FILE_NAME);
        ChiudiFile(file, "CountFail: Errore nella chiusura del file");
        return 0;
    }

    // Conta il numero di utenti nel file
    while (fgets(line, sizeof(line), file) != NULL)
    {
        numero_utenti++;
    }

    ChiudiFile(file, "CountFail: Errore nella chiusura del file");
    return numero_utenti - 1; // Tolto l'utente amministratore
}

void ToLower(char Stringa[])
{
    for (int i = 0; Stringa[i]; i++) // Verifica se l'utentente inserisce un punto e virgola
    {
        Stringa[i] = tolower(Stringa[i]);
    }
}

void Divisore()
{
    int n = 80; // Numero di -
    for (int i = 0; i <= n; i++)
    {
        printf("-");
    }
    printf("\n");
}

#endif // GENERAL_C