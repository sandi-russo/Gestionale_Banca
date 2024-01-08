#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define NUM_COLONNE 5             // Definizamo il numero delle colonne del file "Utenti.csv"
#define MAX_STR_LEN 100           // Definizamo la lunghezza massima della stringa
#define MAX_BUFFER_LEN 1024       // Definizamo la lunghezza massima del buffer
#define MAX_UTENTI 255            // Definiamo il numero massimo di utenti inseribili nella struct
#define FILE_NAME "Utenti.csv"    // Definiamo il nome del file principale
#define TEMP_FILE_NAME "Temp.csv" // Definiamo il nome del file temporaneo
#define ABI "07648"               // Definiamo il Codice dell'Associazione Bancaria Italiana
#define CAB "05433"               // Definiamo il Codice di Avviamento Bancario



typedef struct // Sruttura contenente i dati del singolo utente
{
    char NomeUtente[MAX_STR_LEN];
    char Password[MAX_STR_LEN];
    char IBAN[MAX_STR_LEN];
    float Saldo;
    char Nome[MAX_STR_LEN];
    char Cognome[MAX_STR_LEN];
} utente;
