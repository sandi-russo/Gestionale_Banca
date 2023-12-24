#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "bank.h"

int main() {
    // while (1) {
    //     // Ottenere il tempo corrente
    //     time_t t = time(NULL);
    //     struct tm *current_time = localtime(&t);

    //     printf ("Ciao, Donw!");
    //     // Utilizzare una sequenza di escape per tornare all'inizio della riga
    //     // e cancellare la riga precedente
    //     printf("\033[1A\033[K");

    //     // Formattare e stampare la nuova data
    //     printf("Data e ora attuali: %02d/%02d/%04d %02d:%02d:%02d\n",
    //            current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900,
    //            current_time->tm_hour, current_time->tm_min, current_time->tm_sec);

    //     // Aggiornare ogni secondo
    //     sleep(1);
        
    // }
    // FileUpdate("lollobrigida", 3);
    return 0;
}
