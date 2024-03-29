#ifndef BANK_H
#define BANK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_STR_LEN 100           // Lunghezza massima della stringa
#define MAX_BUFFER_LEN 1024       // Lunghezza massima del buffer
#define MAX_UTENTI 255            // Numero massimo di utenti
#define FILE_NAME "utenti.csv"    // Nome del file principale
#define TEMP_FILE_NAME "temp.csv" // Nome del file temporaneo
#define ABI "07648"               // Codice dell'Associazione Bancaria Italiana
#define CAB "05433"               // Codice di Avviamento Bancario

typedef struct utente // Struttura dati contenente le informazioni del singolo utente
{
    char NomeUtente[MAX_STR_LEN];
    char Password[MAX_STR_LEN];
    char IBAN[28];
    float Saldo;
    char Nome[MAX_STR_LEN];
    char Cognome[MAX_STR_LEN];
} utente;

typedef struct Nodo // Struttura dati contenente un nodo della lista utenti usata in amministrazione
{
    utente utente;
    struct Nodo* next;
} Nodo;

#endif // BANK_H