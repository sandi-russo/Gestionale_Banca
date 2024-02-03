#include "Consumer.c"
#include "Admin.c"

void Login()
{
    FILE *file = ApriFile(FILE_NAME, "r", "LoginFail: errore nell'apertura del file."); // Apertura file tramite funzione 'ApriFile'

    char InputNomeUtente[MAX_STR_LEN], InputPassword[MAX_STR_LEN], RigaUtente[MAX_STR_LEN];
    int Trovato, tentativi = 5, daticorretti = 0;

    while (!daticorretti && tentativi > 0) // Ciclo visualizzazione menu di autenticazione
    {
        printf("\033[0;34m"); // Sequenza di escape per il colore blu
        printf("\nCaricamento in corso...\n");
        printf("\033[0m"); // Sequenza di escape per il colore predefinito
        Sleep(1000);       // Pausa
        system("cls");
        printf("Accedi con le tue credenziali | Tentativi rimasti: %d\n", tentativi);
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
            printf("\033[0;31m");                   // Sequenza di escape per il colore rosso
            printf("Username errato.\nRiprova.\n"); // Username non esistente nel file
            printf("\033[0m");                      // Sequenza di escape per il colore predefinito
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
                    printf("\033[0;32m"); // Sequenza di escape per il colore verde
                    printf("\nAmministratore autenticato con successo.\n\nIngresso nella console...\n");
                    printf("\033[0m"); // Sequenza di escape per il colore predefinito
                    Nodo *listaUtenti = ListFromCSV();
                    Admin(listaUtenti);
                    FreeUserList(listaUtenti);
                }
                else
                {
                    printf("\033[0;32m"); // Sequenza di escape per il colore verde
                    printf("\nUtente [%s] autenticato con successo\n\nIngresso nel conto...\n", user.NomeUtente);
                    printf("\033[0m"); // Sequenza di escape per il colore predefinito
                    Consumer(user);
                }
            }
            else
            {
                printf("\033[0;31m"); // Sequenza di escape per il colore rosso
                printf("\nPassword errata.\nRiprova.\n");
                printf("\033[0m"); // Sequenza di escape per il colore predefinito
                tentativi--;
            }
        }

        if (!daticorretti)
        {
            if (tentativi == 0)
            {
                printf("\033[0;31m");                                                      // Sequenza di escape per il colore rosso
                printf("\nTroppi tentativi errati.\n\nUscita in corso...\n");              // Stampa avviso di numero di tentativi disponibili superato
                printf("\033[0m");                                                         // Sequenza di escape per il colore predefinito
                ChiudiFile(file, "LoginFail: il file non e' stato chiuso correttemente."); // Chiusura file tramite funzione 'ChiudiFile'
            }
        }
    }
}

void Banca()
{
    int scelta;

    Sleep(1000); // Pausa
    system("cls");
    do // Ciclo visualizzazione menu banca
    {
        printf("UniCode Bank | Benvenuto!\n");
        Divisore();
        printf("Menu:\n\n");
        printf("1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n\n");
        Divisore();
        printf("0 - Esci dal programma\n");
        Divisore();

        scelta = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 2);
        if (scelta == -1)
        {
            Sleep(1000);
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
            printf("\033[0;34m"); // Sequenza di escape per il colore blu
            printf("\nUscita dal programma in corso...\n");
            printf("\033[0m"); // Sequenza di escape per il colore predefinito
        }
        else
        {
            printf("\033[0;31m");                                                 // Sequenza di escape per il colore rosso
            printf("\nIl valore inserito non e' valido.\nPer favore riprova.\n"); // Gestisci input non validi
            printf("\033[0m");                                                    // Sequenza di escape per il colore predefinito
        }
        Sleep(1000); // Pausa
        system("cls");
    } while (scelta != 0); // Fino a quando la scelta è diversa da 0, ripete il ciclo
}