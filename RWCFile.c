#include <stdio.h>
#include <Windows.h>
//RWCFile è un alias che sta per Read And Write File Control


void controlloFileX(FILE *f , char percorso[100]) // Controlla se il file è stato trovato o meno;
{
    if(f == NULL)
    {
        printf("Il file non esiste o non e' stato aperto.\n");
        exit(1);
    }else
    {
        printf("Il file e' stato aperto con successo!"); // Il file esiste ed è stato aperto;
    }
}

void cFileS1(FILE *f, char a[100], char percorso[100]) // Scrive nel file 1 array di caratteri solo nel caso il file csv contenga una sola colonna;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s", a);
    fclose(f);
}

void cFileS2(FILE *f, char a[100], char b[100], char percorso[100]) // Scrive nel file 2 array di caratteri solo nel caso il file csv contenga due colonne;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s, %s", a, b);
    fclose(f);
}

void cFileS3(FILE *f, char a[100], char b[100], char c[100], char percorso[100]) // Scrive nel file 3 array di caratteri solo nel caso il file csv contenga tre colonne;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s, %s, ", a, b);
    fprintf(f, "%s", c);
    fclose(f);
}

void cFileS4(FILE *f, char a[100], char b[100], char c[100], char d[100], char percorso[100]) // Scrive nel file 4 array di caratteri solo nel caso il file csv contenga quattro colonne;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s, %s, ", a, b);
    fprintf(f, "%s, %s", c, d);
    fclose(f);
}

void cFileS5(FILE *f, char a[100], char b[100], char c[100], char d[100], char e[100], char percorso[100]) // Scrive nel file 5 array di caratteri solo nel caso il file csv contenga cinque colonne;
{
    //printf("Nome:%s\n Cognome:%s\n NomeUtente:%s\n Password:%s\n ConfermaPassword:%s", a, b, c, d, e);//Riga di debug
    f = fopen(percorso, "a");
    fprintf(f, "%s, %s, ", a, b);
    fprintf(f, "%s, %s, ", c, d);
    fprintf(f, "%s\n", e);
    fclose(f);
}

