#include "bank.c"

int main()
{
    // Sleep(2000); // Debug
    system("cls");
    printf("\033[0;34m"); // Sequenza di escape per il colore blu
    printf("Caricamento in corso...\n");
    printf("\033[0m");               // Sequenza di escape per il colore predefinito
    srand((unsigned int)time(NULL)); // Seed randomico per la generazione dell'IBAN
    Banca();                         // Richiamo la funzione Banca dalla libreria

    return 0;
}