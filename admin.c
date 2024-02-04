#ifndef ADMIN_C
#define ADMIN_C

#include "general.c"

/*
    Primo parametro: array di caratteri (nome del file)
    Secondo parametro: puntatore alla testa della lista

    La funzione apre il file con il nome specificato in modalita' "w"
    scrivendo al suo interno l'intestazione e successivamente gli utenti
    della lista dinamica, ciascuno per riga, con i parametri separati da ";".
*/
int SaveListToCSV(const char *FileName, Nodo *head)
{
    FILE *file = ApriFile(FileName, "w", "ListWriteError: Errore nell'apertura del file");
    // Stampa l'intestazione
    fprintf(file, "NomeUtente;Password;IBAN;Saldo;Nome;Cognome;\n");
    fprintf(file, "admin_code;@UniCode2024;IT00A0000000000000000000000;0.00;Admin;Admin;\n");

    Nodo *current = head;

    while (current != NULL)
    {
        // Stampa i dati di ogni utente
        fprintf(file, "%s;%s;%s;%.2f;%s;%s;\n",
                current->utente.NomeUtente,
                current->utente.Password,
                current->utente.IBAN,
                current->utente.Saldo,
                current->utente.Nome,
                current->utente.Cognome);

        current = current->next;
    }

    ChiudiFile(file, "ListWriteError: Errore nella chiusura del file.");
    return 1; // Ritorna 1 se l'operazione è riuscita
}

/*
    Parametro: puntatore ad un nodo della lista dinamica

    La funzione stampa in maniera formattata i parametri che caratterizzano
    il singolo utente specificato dal puntatore.
*/
void PrintUser(Nodo *current)
{
    printf("Nome Utente: \033[0;32m%-23s\033[0m | Password: \033[0;32m%s\033[0m\n", current->utente.NomeUtente, current->utente.Password);
    printf("IBAN: \033[0;32m%-30s\033[0m | Saldo: \033[0;32m%.2f\033[0m\n", current->utente.IBAN, current->utente.Saldo);
    printf("Nome: \033[0;32m%-30s\033[0m | Cognome: \033[0;32m%s\033[0m\n", current->utente.Nome, current->utente.Cognome);
    Divisore();
}

/*
    La funzione apre il file principale in modalita' "r" e crea una lista
    con inserimento in coda per mantenere l'ordine degli utenti dettato dal
    file principale.

    Return: puntatore alla testa della lista
*/
Nodo *ListFromCSV()
{
    FILE *file = ApriFile(FILE_NAME, "r", "ListError: Errore nell'apertura del file.");

    Nodo *head = NULL;
    Nodo *tail = NULL;

    char buffer[MAX_BUFFER_LEN];
    fgets(buffer, MAX_BUFFER_LEN, file); // Ignora la prima riga (intestazione)

    while (fgets(buffer, MAX_BUFFER_LEN, file) != NULL)
    {
        utente nuovoUtente;
        sscanf(buffer, "%[^;];%[^;];%[^;];%f;%[^;];%[^;];",
               nuovoUtente.NomeUtente,
               nuovoUtente.Password,
               nuovoUtente.IBAN,
               &nuovoUtente.Saldo,
               nuovoUtente.Nome,
               nuovoUtente.Cognome);

        // Salta l'utente amministratore
        if (strcmp(nuovoUtente.NomeUtente, "admin_code") == 0)
        {
            continue;
        }

        Nodo *nuovoNodo = (Nodo *)malloc(sizeof(Nodo));
        nuovoNodo->utente = nuovoUtente;
        nuovoNodo->next = NULL;

        if (head == NULL)
        {
            head = tail = nuovoNodo;
        }
        else
        {
            tail->next = nuovoNodo;
            tail = nuovoNodo;
        }
    }

    ChiudiFile(file, "ListError: Errore nella chiusura del file.");
    return head;
}

/*
    Parametro: puntatore alla testa della lista

    La funzione stampa tutti gli utenti contenuti nella lista dinamica
    in ordine di inserimento, utilizzando la funzione PrintUser().
*/
void PrintUserList(Nodo *head)
{
    Sleep(1500);
    system("cls");
    Nodo *current = head;
    int indice = 1; // Inizializza l'indice a 1

    printf("\033[0;34mUniCode\033[0m \033[0;32mBank\033[0m | Lista utenti:\n");
    Divisore();
    while (current != NULL)
    {
        printf("ID utente: \033[0;34m%d\033[0m\n", indice);
        PrintUser(current);
        current = current->next;
        indice++;
    }
}

/*
    Parametro: puntatore alla testa della lista

    La funzione dealloca la memoria occupata da ogni singolo nodo all'interno della lista
    puntata tramite un ciclo while con condizione current != NULL, con current corrispondente
    al nodo puntato ad ogni iterazione, aggiornato tramite current = next alla fine del ciclo.
*/
void FreeUserList(Nodo *head)
{
    Nodo *current = head;
    while (current != NULL)
    {
        Nodo *next = current->next;
        free(current);
        current = next;
    }
}

/*
    Primo parametro: puntatore alla testa della lista
    Secondo parametro: intero (numero degli utenti)

    La funzione stampa a schermo tramite PrintUserList() la lista degli utenti, associando ad
    ognuno di essi un indice. Inserendo l'indice dell'utente sul quale si desidera effettuare
    le modifiche, è possibile modificarne i parametri e successivamente salvarli all'interno del
    file temporaneo. Una volta applicate le modifiche, saranno salvate anche all'interno del file
    principale.
*/
void EditUser(Nodo *head, int numeroUtenti)
{
    int sceltaUtente, sceltaParametro, edits;
    char buffer[MAX_STR_LEN];

    do
    {
        edits = 0;
        PrintUserList(head);
        printf("\nInserisci l'indice dell'utente da modificare \033[0;31m(0 per uscire)\033[0m: ");
        sceltaUtente = ReadInputChoice("", 0, numeroUtenti);

        if (sceltaUtente == 0)
        {
            printf("\033[0;34m"); // Sequenza di escape per il colore blu
            printf("\nReindirizzamento al menu di amministrazione in corso...\n");
            printf("\033[0m"); // Sequenza di escape per il colore predefinito
            break;
        }

        Nodo *current = head;
        int count = 1;

        while (current != NULL && count < sceltaUtente)
        {
            current = current->next;
            count++;
        }

        if (current == NULL || count != sceltaUtente)
        {
            TintedPrint('r', "Utente non trovato. Riprova: ");
            continue;
        }

        do
        {
            Sleep(1500);
            system("cls");
            printf("Utente: \033[0;34m%d\033[0m\n", sceltaUtente);
            PrintUser(current);
            printf("Modifiche effettuate: \033[0;32m%-14d\033[0m | Scegli il parametro da modificare:\n", edits);
            Divisore();
            printf("\n");
            printf("1 - Nome Utente\n");
            printf("2 - Password\n");
            printf("3 - IBAN\n");
            printf("4 - Saldo\n");
            printf("5 - Nome\n");
            printf("6 - Cognome\n\n");
            Divisore();
            TintedPrint('g', "7 - Applica modifiche\n");
            TintedPrint('r', "0 - Esci\n");
            Divisore();

            sceltaParametro = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 7);

            switch (sceltaParametro)
            {
            case 0:
                TintedPrint('b', "\nUscita in corso...\n");
                break;
            case 1:
                printf("Nome Utente corrente: %s\n", current->utente.NomeUtente);
                printf("Nuovo Nome Utente: ");
                if (fgets(current->utente.NomeUtente, sizeof(current->utente.NomeUtente), stdin) == NULL)
                {
                    TintedPrint('r', "\nErrore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                ToLower(current->utente.NomeUtente);
                current->utente.NomeUtente[strcspn(current->utente.NomeUtente, "\n")] = '\0'; // Rimuovi il carattere di nuova riga
                edits++;
                break;
            case 2:
                printf("Password corrente: %s\n", current->utente.Password);
                printf("Nuova password: ");
                if (fgets(current->utente.Password, sizeof(current->utente.Password), stdin) == NULL)
                {
                    TintedPrint('r', "\nErrore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                current->utente.Password[strcspn(current->utente.Password, "\n")] = '\0'; // Rimuovi il carattere di nuova riga
                edits++;
                break;
            case 3:
                printf("IBAN corrente: %s\n", current->utente.IBAN);
                printf("Nuovo IBAN: ");
                if (fgets(current->utente.IBAN, sizeof(current->utente.IBAN), stdin) == NULL)
                {
                    TintedPrint('r', "\nErrore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                current->utente.IBAN[strcspn(current->utente.IBAN, "\n")] = '\0'; // Rimuovi il carattere di nuova riga
                edits++;
                break;
            case 4:
                printf("Saldo corrente: %.2f\n", current->utente.Saldo);
                printf("Nuovo Saldo: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                {
                    TintedPrint('r', "\nErrore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                sscanf(buffer, "%f", &current->utente.Saldo);
                edits++;
                break;
            case 5:
                printf("Nome corrente: %s\n", current->utente.Nome);
                printf("Nuovo Nome: ");
                if (fgets(current->utente.Nome, sizeof(current->utente.Nome), stdin) == NULL)
                {
                    TintedPrint('r', "\nErrore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                current->utente.Nome[strcspn(current->utente.Nome, "\n")] = '\0'; // Rimuovi il carattere di nuova riga
                edits++;
                break;
            case 6:
                printf("Cognome corrente: %s\n", current->utente.Cognome);
                printf("Nuovo Cognome: ");
                if (fgets(current->utente.Cognome, sizeof(current->utente.Cognome), stdin) == NULL)
                {
                    TintedPrint('r', "\nErrore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                current->utente.Cognome[strcspn(current->utente.Cognome, "\n")] = '\0'; // Rimuovi il carattere di nuova riga
                edits++;
                break;
            case 7:
                if (edits > 0)
                {
                    TintedPrint('g', "\nSalvataggio delle modifiche in corso...\n");
                    FileUpdate(FILE_NAME, TEMP_FILE_NAME);
                    edits = 0;
                }
                else
                {
                    TintedPrint('r', "\nNon ci sono modifiche da salvare.");
                }
                break;
            default:
                TintedPrint('r', "\nScelta non valida.\n");
                break;
            }
            if (edits > 0) // Salva le modifiche effettuate nel file temporaneo
            {
                SaveListToCSV(TEMP_FILE_NAME, head);
            }

        } while (sceltaParametro != 0);
    } while (sceltaUtente != 0);
}

/*
    Parametro: puntatore alla testa della lista utenti

    La funzione stampa il menu di amministrazione tramite il quale e'
    possibile modificare la lista puntata inserita come parametro.
*/
void Admin(Nodo *listaUtenti)
{
    int scelta, numero_utenti = CountUserNum();

    do
    {
        Sleep(1500);
        system("cls");
        printf("\033[0;34mUniCode\033[0m \033[0;32mBank\033[0m | Console di amministrazione | \033[0;32m%d\033[0m Conti Bancari\n", numero_utenti);
        Divisore();
        printf("Menu:\n\n");
        printf("1 - Gestisci un conto corrente\n\n");
        Divisore();
        TintedPrint('r', "0 - Esci\n");
        Divisore();

        scelta = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 1);

        switch (scelta)
        {
        case 1:
            EditUser(listaUtenti, numero_utenti);
            break;
        case 0:
            TintedPrint('b', "\nUscita dalla console di amministrazione in corso...\n");
            break;
        default:
            TintedPrint('r', "\nScelta non valida.\n");
            break;
        }
    } while (scelta != 0);
}

#endif // ADMIN_C