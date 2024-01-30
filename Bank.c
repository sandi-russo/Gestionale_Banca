#include "admin.c"
#include "consumer.c"

void Login()
{
    FILE *file = ApriFile(FILE_NAME, "r", "LoginFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'

    char InputNomeUtente[MAX_STR_LEN], InputPassword[MAX_STR_LEN], RigaUtente[MAX_STR_LEN];
    int Trovato, tentativi = 5, daticorretti = 0;

    while (!daticorretti && tentativi > 0) // Ciclo visualizzazione menu di autenticazione
    {
        printf("\nCaricamento in corso...\n");
        Sleep(1000); // Pausa
        system("cls");
        printf("Accedi con le tue credenziali | Tentativi rimasti: %d\n", tentativi);
        printf("----------------------------------------------------------------\n");
        printf("Inserisci il tuo nome utente: ");
        scanf("%s", InputNomeUtente);
        pulisciBuffer();
        printf("Inserisci la tua password: ");
        scanf("%s", InputPassword);
        pulisciBuffer();

        if (SearchInFile("NomeUtente", InputNomeUtente, RigaUtente) > 0) // Cerca la riga contenente il valore specificato nella colonna specificata
        {
            printf("Username errato. Riprova\n"); // Username non esistente nel file
            tentativi--;
        }
        else
        {
            utente user = CreateUserStruct(RigaUtente); // Se esiste l'utente, salvalo nella struttura

            if (strcmp(user.Password, InputPassword) == 0) // Se la password e' corretta, entra nel conto
            {
                daticorretti = 1;
                ChiudiFile(file, "LoginFail: il file non e' stato chiuso correttamente"); // Chiusura file tramite funzione 'ChiudiFile'
                if (strcmp("admin_code", user.NomeUtente) == 0)
                {
                    printf("\nAmministratore autenticato con successo\n\nIngresso nella console...\n");
                    Nodo* listaUtenti = ListFromCSV();
                    Admin(listaUtenti);
                    FreeUserList(listaUtenti);
                }
                else
                {
                    printf("\nUtente [%s] autenticato con successo\n\nIngresso nel conto...\n", user.NomeUtente);
                    Consumer(user);
                }
            }
            else
            {
                printf("\nPassword errata. Riprova\n");
                tentativi--;
            }
        }

        if (!daticorretti)
        {
            if (tentativi == 0)
            {
                printf("\nTroppi tentativi errati\n\nUscita in corso...\n");              // Stampa avviso di numero di tentativi disponibili superato
                ChiudiFile(file, "LoginFail: il file non e' stato chiuso correttemente"); // Chiusura file tramite funzione 'ChiudiFile'
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
        printf("UniCode Bank: Benvenuto!\n");
        printf("----------------------------------------------------------------\n");
        printf("Menu:\n\n");
        printf("1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n\n");
        printf("----------------------------------------------------------------\n");
        printf("0 - Esci dal programma\n");
        printf("----------------------------------------------------------------\n");

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
            printf("\nUscita dal programma in corso...\n");
        }
        else
        {
            printf("\nIl valore inserito non e' valido. Perfavore riprova\n"); // Gestisci input non validi
        }
        Sleep(1000); // Pausa
        system("cls");
    } while (scelta != 0); // Fino a quando la scelta è diversa da 0, ripete il ciclo
}

// bool modificaUtente(utente *user)
// {
//     int scelta, contatoreModifiche = 0;
//     float saldo;
//     bool fineLoop = false;
//     char nomeutente[MAX_STR_LEN], cognome[MAX_STR_LEN], nome[MAX_STR_LEN], password[MAX_STR_LEN];

//     do
//     {
//         system("cls");
//         printf("----------------------------------------------------------------\n");
//         printf("Cosa vorresti modificare dell'utente: %s?\n", user->NomeUtente);
//         printf("1 - NomeUtente\n");
//         printf("2 - Password\n");
//         printf("3 - Saldo\n");
//         printf("4 - Nome\n");
//         printf("5 - Cognome\n\n");
//         printf("----------------------------------------------------------------\n");
//         printf("6 - Applica le modifiche | 0 - Esci\n");
//         printf("----------------------------------------------------------------\n");
//         printf("Seleziona un'opzione: ");
//         scanf("%d", &scelta);
//         switch (scelta)
//         {
//         case 0:
//             FileUpdate(FILE_NAME, TEMP_FILE_NAME);
//             contatoreModifiche = 0;
//             printf("Uscita In corso...\n");
//             Sleep(1000); // Pausa
//             fineLoop = true;
//             return fineLoop;
//             break;

//         case 1:
//             printf("Vecchio Nome Utente: %s\n", user->NomeUtente);
//             RemoveLine("NomeUtente", user->NomeUtente);
//             printf("Inserisci nuovo Nome Utente: ");
//             scanf("%s", nomeutente);
//             strcpy(user->NomeUtente, nomeutente);
//             printf("Modifica Eseguita con successo!\n");
//             contatoreModifiche++;
//             Sleep(1000); // Pausa
//             break;
//         case 2:
//             printf("Vecchia Password: %s\n", user->Password);
//             RemoveLine("Password", user->Password);
//             printf("Inserisci nuova Password: ");
//             scanf("%s", password);
//             strcpy(user->Password, password);
//             printf("Modifica Eseguita con successo!\n");
//             contatoreModifiche++;
//             Sleep(1000); // Pausa
//             break;
//         case 3:
//             printf("Vecchio Saldo: %0.2f\n", user->Saldo);
//             printf("Inserisci l'importo: ");
//             scanf("%f", &saldo);
//             user->Saldo = saldo;
//             RemoveLine("IBAN", user->IBAN);
//             contatoreModifiche++;
//             printf("Modifica Eseguita con successo!\n");
//             Sleep(1000); // Pausa
//             break;
//         case 4:
//             printf("Vecchio Nome: %s\n", user->Nome);
//             RemoveLine("Nome", user->Nome);
//             printf("Inserisci nuovo Nome: ");
//             scanf("%s", nome);
//             strcpy(user->Nome, nome);
//             contatoreModifiche++;
//             printf("Modifica Eseguita con successo!\n");
//             Sleep(1000); // Pausmod
//             break;
//         case 5:
//             printf("Vecchio Cognome: %s\n", user->Cognome);
//             RemoveLine("Cognome", user->Cognome);
//             printf("Inserisci nuova Cognome: ");
//             scanf("%s", cognome);
//             strcpy(user->Cognome, cognome);
//             contatoreModifiche++;
//             printf("Modifica Eseguita con successo!\n");
//             Sleep(1000); // Pausa
//             break;
//         case 6:
//             if (contatoreModifiche > 0)
//             {
//                 RemoveLine("IBAN", user->IBAN);
//                 WritingStruct(TEMP_FILE_NAME, user);
//                 FileUpdate(FILE_NAME, TEMP_FILE_NAME); // SOS
//                 contatoreModifiche = 0;
//             }
//             else
//             {
//                 printf("Non hai apportato modifiche all'utente selezionato.");
//                 Sleep(1000); // Pausa
//             }
//             break;
//         default:
//             system("cls");
//             printf("InputFail: non puoi insere numeri diversi da quelli che sono scritti!\n");
//             Sleep(1000); // Pausa
//             break;
//         }
//     } while (fineLoop != true);
// }

// void stampaListaUtenti(FILE *f)
// {
//     char buffer[MAX_BUFFER_LEN], *tokens;
//     int colonne = 0, posNC = 1;

//     f = ApriFile(FILE_NAME, "r", "Non e' stato possibile aprire il file!"); // Stampa Nomi e Cognomi
//     fgets(buffer, sizeof(buffer), f);
//     printf("----------------------------------------------------------------\n");
//     printf("   Utente             Nome      Cognome\n\n");
//     while (fgets(buffer, sizeof(buffer), f) != NULL)
//     {
//         colonne = 0;

//         tokens = strtok(buffer, ";"); // Assegno il delimitatore
//         printf("%d) ", posNC);
//         printf("%-18s ", tokens);
//         while (tokens)
//         {
//             if (colonne == 4)
//             {
//                 printf("%-9s ", tokens);
//             }
//             if (colonne == 5)
//             {
//                 printf("%-10s\n", tokens);
//             }
//             tokens = strtok(NULL, ";"); // Se non trova la colonna, andiamo a specificargli il delimitatore da trovare
//             colonne++;
//         }
//         posNC++;
//     }
//     printf("----------------------------------------------------------------\n");
//     ChiudiFile(f, "Non e' stato possibile chiudere il file!"); // Chiusura file tramite funzione 'ChiudiFile'
// }

// void utentiModifiche(FILE *f)
// {
//     bool fineModifiche = false;
//     bool fineModificheUtente;
//     char rigaUtente[MAX_STR_LEN];
//     char nomeUtente[MAX_STR_LEN];
//     int scelta;

//     do
//     {
//         system("cls");
//         printf("----------------------------------------------------------------\n");
//         printf("1 - Scegli Utente\n\n");
//         printf("----------------------------------------------------------------\n");
//         printf("0 - Esci\n");
//         printf("----------------------------------------------------------------\n");
//         printf("Seleziona un'opzione: ");
//         scanf("%d", &scelta);
//         switch (scelta)
//         {
//         case 0:
//             printf("Uscita In corso...\n");
//             Sleep(1000); // Pausa
//             fineModifiche = true;
//             fineModificheUtente = true;
//             break;
//         case 1:
//             system("cls");
//             stampaListaUtenti(f);
//             printf("Inserisci nome utente:");
//             scanf("%s", nomeUtente);
//             if (SearchInFile("NomeUtente", nomeUtente, rigaUtente) == 0)
//             {
//                 utente user = CreateUserStruct(rigaUtente);
//                 fineModificheUtente = modificaUtente(&user);
//             }
//             break;

//         default:
//             system("cls");
//             printf("InputFail: non puoi insere numeri diversi da quelli che sono scritti!\n");
//             Sleep(1000); // Pausa
//             break;
//         }

//     } while (fineModifiche != true && !fineModifiche);
// }

// void Admin()
// {
//     int scelta = 0;
//     FILE *utenti;

//     do
//     {   
//         Sleep(1000);
//         system("cls");
//         printf("UniCode Bank: Console di Amministrazione\n\n");
//         printf("----------------------------------------------------------------\n");
//         printf("Menu:\n\n");
//         printf("1 - Modifica Utenti\n\n");
//         printf("----------------------------------------------------------------\n");
//         printf("0 - Esci\n");
//         printf("----------------------------------------------------------------\n");
//         printf("Seleziona un'opzione: ");
//         scanf("%d", &scelta);
//         switch (scelta)
//         {
//         case 0:
//             printf("Arrivederci!");
//             break;
//         case 1:
//             system("cls");
//             utentiModifiche(utenti);
//             break;
//         default:
//             system("cls");
//             printf("InputFail: non puoi insere numeri diversi da quelli che sono scritti!\n");
//             Sleep(1000); // Pausa
//             break;
//         }

//     } while (scelta != 0);
// }