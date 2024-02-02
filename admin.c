#include "general.c"

// Funzione per scrivere la lista dinamica in un file csv temporaneo
int SaveListToCSV(const char *FileName, Nodo* head)
{
    FILE* file = ApriFile(FileName, "w", "ListWriteError: Errore nell'apertura del file");
    // Stampa l'intestazione
    fprintf(file, "NomeUtente;Password;IBAN;Saldo;Nome;Cognome;\n");
    fprintf(file, "admin_code;@UniCode2024;IT00A0000000000000000000000;0.00;Admin;Admin;\n");

    Nodo* current = head;

    while (current != NULL) {
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

    ChiudiFile(file, "ListWriteError: Errore nella chiusura del file");
    return 1;  // Ritorna 1 se l'operazione è riuscita
}

// Funzione per stampare un singolo nodo della lista dinamica
void PrintUser(Nodo *current)
{
    printf("Nome Utente: %s\n", current->utente.NomeUtente);
    printf("Password: %s\n", current->utente.Password);
    printf("IBAN: %s\n", current->utente.IBAN);
    printf("Saldo: %.2f\n", current->utente.Saldo);
    printf("Nome: %s\n", current->utente.Nome);
    printf("Cognome: %s\n", current->utente.Cognome);
    printf("----------------------------------------------------------------\n");
}

// Funzione che crea una lista dinamica dal file csv principale
Nodo* ListFromCSV()
{
    FILE* file = ApriFile(FILE_NAME, "r", "ListError: Errore nell'apertura del file");

    Nodo* head = NULL;
    Nodo* tail = NULL;

    char buffer[MAX_BUFFER_LEN];
    fgets(buffer, MAX_BUFFER_LEN, file);  // Ignora la prima riga (intestazione)

    while (fgets(buffer, MAX_BUFFER_LEN, file) != NULL) {
        utente nuovoUtente;
        sscanf(buffer, "%[^;];%[^;];%[^;];%f;%[^;];%[^;];",
               nuovoUtente.NomeUtente,
               nuovoUtente.Password,
               nuovoUtente.IBAN,
               &nuovoUtente.Saldo,
               nuovoUtente.Nome,
               nuovoUtente.Cognome);

        // Salta l'utente amministratore
        if (strcmp(nuovoUtente.NomeUtente, "admin_code") == 0) {
            continue;
        }

        Nodo* nuovoNodo = (Nodo*)malloc(sizeof(Nodo)); 
        nuovoNodo->utente = nuovoUtente;
        nuovoNodo->next = NULL;

        if (head == NULL) {
            head = tail = nuovoNodo;
        }
        else
        {
            tail->next = nuovoNodo;
            tail = nuovoNodo;
        }
    }

    ChiudiFile(file, "ListError: Errore nella chiusura del file");
    return head;
}

// Funzione che stampa gli elementi della lista dinamica
void PrintUserList(Nodo* head)
{
    Sleep(1000);
    system("cls");
    Nodo* current = head;
    int indice = 1;  // Inizializza l'indice a 1

    printf("Unicode Bank | Lista utenti:\n");
    printf("----------------------------------------------------------------\n");
    while (current != NULL)
    {
        printf("ID utente: %d\n", indice);
        PrintUser(current);
        current = current->next;
        indice++;
    }
}

// Funzione per liberare la memoria allocata per ogni singolo nodo della lista dinamica
void FreeUserList(Nodo* head)
{
    Nodo* current = head;
    while (current != NULL)
    {
        Nodo* next = current->next;
        free(current);
        current = next;
    }
}

// Funzione che permette di modificare i dati di un singolo utente
void EditUser(Nodo *head, int numeroUtenti)
{
    int sceltaUtente, sceltaParametro, edits;
    char buffer[MAX_STR_LEN];

    do
    {
        edits = 0;
        PrintUserList(head);
        printf("\nInserisci l'indice dell'utente da modificare (0 per uscire): ");
        sceltaUtente = ReadInputChoice("", 0, numeroUtenti);

        if (sceltaUtente == 0)
        {
            printf("Reindirizzamento al menu di amministrazione in corso...\n");
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
            printf("Utente non trovato\n");
            continue;
        }

        do {
            Sleep(1000);
            system("cls");
            printf("Utente: %d\n", sceltaUtente);
            PrintUser(current);
            printf("Modifiche effettuate: %d | Scegli il parametro da modificare:\n", edits);
            printf("----------------------------------------------------------------\n");
            printf("1 - Nome Utente\n");
            printf("2 - Password\n");
            printf("3 - IBAN\n");
            printf("4 - Saldo\n");
            printf("5 - Nome\n");
            printf("6 - Cognome\n\n");
            printf("----------------------------------------------------------------\n");
            printf("7 - Applica modifiche\n");
            printf("0 - Esci\n");
            printf("----------------------------------------------------------------\n");

            sceltaParametro = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 7);

            switch (sceltaParametro)
            {
            case 0:
                printf("Uscita in corso...\n");
                break;
            case 1:
                printf("Nome Utente corrente: %s\n", current->utente.NomeUtente);
                printf("Nuovo Nome Utente: ");
                if (fgets(current->utente.NomeUtente, sizeof(current->utente.NomeUtente), stdin) == NULL) {
                    printf("Errore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                ToLower(current->utente.NomeUtente);
                current->utente.NomeUtente[strcspn(current->utente.NomeUtente, "\n")] = '\0'; // Rimuovi il carattere di nuova riga
                edits++;
                break;
            case 2:
                printf("Password corrente: %s\n", current->utente.Password);
                printf("Nuova Password: ");
                if (fgets(current->utente.Password, sizeof(current->utente.Password), stdin) == NULL)
                {
                    printf("Errore nella lettura dell'input.\n");
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
                    printf("Errore nella lettura dell'input.\n");
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
                    printf("Errore nella lettura dell'input.\n");
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
                    printf("Errore nella lettura dell'input.\n");
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
                    printf("Errore nella lettura dell'input.\n");
                    exit(EXIT_FAILURE);
                }
                current->utente.Cognome[strcspn(current->utente.Cognome, "\n")] = '\0'; // Rimuovi il carattere di nuova riga
                edits++; 
                break;
            case 7:
                if (edits > 0)
                    {   
                        printf("Salvataggio delle modifiche in corso...\n");
                        FileUpdate(FILE_NAME, TEMP_FILE_NAME);
                        edits = 0;
                    }
                    else
                    {
                        printf("Non ci sono modifiche da salvare");
                    }
                break;
            default:
                printf("Scelta non valida\n");
                break;
            }
            if (edits > 0) // Salva le modifiche effettuate nel file temporaneo
            {
                SaveListToCSV(TEMP_FILE_NAME, head);
            }

        } while (sceltaParametro != 0);
    } while (sceltaUtente != 0);
}

// Funzione che stampa il menu di amministrazione
void Admin(Nodo* listaUtenti)
{
    int scelta, numero_utenti = CountUserNum();

    do
    {
        Sleep(1000);
        system("cls");
        printf("UniCode Bank | Console di amministrazione | %d Conti Bancari\n", numero_utenti);
        printf("----------------------------------------------------------------\n");
        printf("Menu:\n\n");
        printf("1 - Gestisci un conto corrente\n\n");
        printf("----------------------------------------------------------------\n");
        printf("0 - Esci\n");
        printf("----------------------------------------------------------------\n");

        scelta = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 1);

        switch (scelta) {
        case 1:
            EditUser(listaUtenti, numero_utenti);
            break;
        case 0:
            printf("Uscita dalla console di amministrazione in corso...\n");
            break;
        default:
            printf("Scelta non valida.\n");
            break;
        }
    } while (scelta != 0);
}