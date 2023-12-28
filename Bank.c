#include "bank.h"

int main()
{   
    system("cls");
    printf("Caricamento in corso...\n");
    srand((unsigned int)time(NULL)); // Seed randomico per la generazione dell'IBANa
    Banca(); // Richiamo la funzione Banca dalla libreria
    return 0;
}