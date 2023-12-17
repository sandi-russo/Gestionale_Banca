// RWCFile è un alias che sta per Read And Write File Control

#include <stdio.h>
#include <string.h>
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

void Writing(const char *nome, const char *cognome, const char *nomeUtente, const char *password, const char *iban)
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