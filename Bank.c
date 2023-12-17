#include <stdio.h>
#include "Consumer.c"
#include "Administrator.c"
#include "Register.c"

int main()
{
    // Generazione seed randomico per la generazione dell'Iban
    srand((unsigned int)time(NULL));
    Banca();
    return 0;
}