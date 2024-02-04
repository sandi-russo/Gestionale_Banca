#ifndef BANK_C // Include guard per evitare inclusioni multiple
#define BANK_C

#include "consumer.c"
#include "admin.c"

void Login()
{   
    IsFileExists();   // Richiamo la funzione per controllare se esiste il file CSV

    FILE *file = ApriFile(FILE_NAME, "r", "LoginFail: errore nell'apertura del file."); // Apertura file tramite funzione 'ApriFile'

    char InputNomeUtente[MAX_STR_LEN], InputPassword[MAX_STR_LEN], RigaUtente[MAX_STR_LEN];
    int Trovato, tentativi = 5, daticorretti = 0;
    while (!daticorretti && tentativi > 0) // Ciclo visualizzazione menu di autenticazione
    {
        TintedPrint('b',"\nCaricamento in corso...");
        Sleep(1500);       // Pausa
        system("cls");
        printf("Accedi con le tue credenziali | Tentativi rimasti: \033[0;32m%d\033[0m\n", tentativi);
        Divisore();
        printf("Inserisci il tuo nome utente: ");
        scanf("%s", InputNomeUtente);
        ToLower(InputNomeUtente);
        pulisciBuffer();
        printf("Inserisci la tua password: ");
        scanf("%s", InputPassword);
        pulisciBuffer();

        if (SearchInFile("NomeUtente", InputNomeUtente, RigaUtente) > 0) // Cerca la riga contenente il valore specificato nella colonna specificata
        {
            TintedPrint('r', "\nNome utente inesistente. Riprova: ");
            tentativi--;
        }
        else
        {
            utente user = CreateUserStruct(RigaUtente); // Se esiste l'utente, salvalo nella struttura

            if (strcmp(user.Password, InputPassword) == 0) // Se la password e' corretta, entra nel conto
            {
                daticorretti = 1;
                ChiudiFile(file, "LoginFail: il file non e' stato chiuso correttamente."); // Chiusura file tramite funzione 'ChiudiFile'
                if (strcmp("admin_code", user.NomeUtente) == 0)
                {
                    TintedPrint('g', "\nAmministratore autenticato con successo. Ingresso nella console...\n");
                    Nodo *listaUtenti = ListFromCSV();
                    Admin(listaUtenti);
                    FreeUserList(listaUtenti);
                }
                else
                {
                    printf("\033[0;32m"); // Sequenza di escape per il colore verde
                    printf("\nUtente [%s] autenticato con successo. Ingresso nel conto...\n", user.NomeUtente);
                    printf("\033[0m"); // Sequenza di escape per il colore predefinito
                    Consumer(user);
                }
            }
            else
            {
                TintedPrint('r', "\nPassword errata. Riprova: \n");
                tentativi--;
            }
        }

        if (!daticorretti)
        {
            if (tentativi == 0)
            {
                TintedPrint('r', "\nTroppi tentativi errati. Uscita in corso...\n");
                ChiudiFile(file, "LoginFail: il file non e' stato chiuso correttemente."); // Chiusura file tramite funzione 'ChiudiFile'
            }
        }
    }
}

void Banca()
{
    int scelta;

    Sleep(1500); // Pausa
    system("cls");
    do // Ciclo visualizzazione menu banca
    {
        printf("\033[0;34mUniCode\033[0m \033[0;32mBank\033[0m | Pagina iniziale\n");
        Divisore();
        printf("Menu:\n\n");
        printf("1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n\n");
        Divisore();
        TintedPrint('r', "0 - Esci dal programma\n");
        Divisore();

        scelta = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 2);
        if (scelta == -1)
        {
            Sleep(1500);
            system("cls");
            continue;
        }

        if (scelta == 1) // Scelta dell'opzione
        {
            Login(); // Richiamo la funzione Login
        }
        else if (scelta == 2)
        {
            Register(); // Richiamo la funzione Register
        }
        else if (scelta == 0)
        {
            TintedPrint('b', "\nUscita dal programma in corso...\n");
        }

        Sleep(1500); // Pausa
        system("cls");
    } while (scelta != 0); // Fino a quando la scelta è diversa da 0, ripete il ciclo
}

#endif // BANK_C