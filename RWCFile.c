#include <stdio.h>
#include <Windows.h>
#include <stdBool.h>
#include <string.h>
//RWCFile è un alias che sta per Read And Write File Control


void controlloFileX(FILE *f , char percorso[100]) // Controlla se il file è stato trovato o meno;
{
    if(f == NULL)
    {
        printf("Il file non esiste o non e' stato aperto.\n");
        f = fopen(percorso, "w+"); // Se il file non esiste crea un file con il nome del percorso.
        exit(1);
    }else
    {
        printf("Il file e' stato aperto con successo!"); // Il file esiste ed è stato aperto;
    }
}

void cFileS1(FILE *f, char a[100], char percorso[100]) // Scrive nel file 1 array di caratteri solo nel caso il file csv contenga una sola colonna;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s\n", a);
    fclose(f);
}

void cFileS2(FILE *f, char a[100], char b[100], char percorso[100]) // Scrive nel file 2 array di caratteri solo nel caso il file csv contenga due colonne;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s,%s\n", a, b);
    fclose(f);
}

void cFileS3(FILE *f, char a[100], char b[100], char c[100], char percorso[100]) // Scrive nel file 3 array di caratteri solo nel caso il file csv contenga tre colonne;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s,%s, ", a, b);
    fprintf(f, "%s\n", c);
    fclose(f);
}

void cFileS4(FILE *f, char a[100], char b[100], char c[100], char d[100], char percorso[100]) // Scrive nel file 4 array di caratteri solo nel caso il file csv contenga quattro colonne;
{
    f = fopen(percorso, "a");
    fprintf(f, "%s,%s,", a, b);
    fprintf(f, "%s,%s\n", c, d);
    fclose(f);
}

void cFileS5(FILE *f, char a[100], char b[100], char c[100], char d[100], char e[100], char percorso[100]) // Scrive nel file 5 array di caratteri solo nel caso il file csv contenga cinque colonne;
{
    //printf("Nome:%s\n Cognome:%s\n NomeUtente:%s\n Password:%s\n ConfermaPassword:%s", a, b, c, d, e);//Riga di debug
    f = fopen(percorso, "a");
    fprintf(f, "%s,%s,", a, b);
    fprintf(f, "%s,%s,", c, d);
    fprintf(f, "%s\n", e);
    fclose(f);
}

bool rFile(FILE *f, char percorso[100], char d[100], char *Titolo){ // Il parametro char d serve per ottenere le informazioni che devono essere controllate nel file
    char a[1000], *t, *g;
    int colonne = 0, righe = 0, comparazioneStringhe, cDiversi = 0, cUguali = 0;
    bool r;

    f = fopen(percorso, "r"); // Apre il file in modalità lettura; 
        while(fgets(a, sizeof(a), f) != NULL){//Cicla per tutto il file
            righe++; // Inizia alla prima riga
            colonne = 0; // Rinizializza la colonna a zero dopo che finisce di comparare la terza colonna

            t = strtok(a, ",");
            while(t){
                if(colonne == 2){
                    g = t; // Il token della terza colonna viene assegnato ad una variabile di appoggio
                    comparazioneStringhe = strcmp(g, d); // Compara il token con la stringa in entrata
                    if(comparazioneStringhe) // Se vero
                    {
                        cDiversi++; // I nomi sono diversi
                    }else // Altrimenti
                    {
                        cUguali++; // I nomi sono uguali
                    }
                }
                t = strtok(NULL, ",");
                colonne++;
            }
        }
    fclose(f); // Chiude il file
    if(cUguali > 0){
        r = true; // Ci sono token uguali;
        printf("%s", Titolo); // Stampa quello che si vuole come messaggio di errore
    }else{
        r = false; // Non ci sono token uguali;
    }
    return r;
}