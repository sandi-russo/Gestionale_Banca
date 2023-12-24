#include "bank.h"

int main()
{
    srand((unsigned int)time(NULL)); // Generazione seed randomico per la generazione dell'Iban
    Banca(); // Richiamo la funzione Banca dal file RWCFile
    return 0;
}