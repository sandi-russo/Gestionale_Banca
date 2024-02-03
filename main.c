#include "Bank.c"

int main()
{
    //Sleep(2000);                   // Debug
    system("cls");
    TintedPrint('b',"\nCaricamento in corso...");
    srand((unsigned int)time(NULL)); // Seed randomico per la generazione dell'IBAN
    Banca();                         // Richiamo la funzione Banca dalla libreria

    return 0;
}