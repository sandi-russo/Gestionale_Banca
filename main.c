#include "bank.c"

int main()
{   
    system("cls");
    printf("Caricamento in corso...\n");
    srand((unsigned int)time(NULL)); // Seed randomico per la generazione dell'IBAN
    Banca(); // Richiamo la funzione Banca dalla libreria

    //Admin();
    return 0;
}