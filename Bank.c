#include "bank.h"

char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100], IBAN[28]; // Dichiarazione variabili per il form di Login e di Registrazione3

FILE *ApriFile(const char *nomeFile, const char *modalita, const char *messaggioErrore)
{
    FILE *file = fopen(nomeFile, modalita); // Apertura file
    if (!file)                              // Se il file non si è aperto, mostra un determinato messaggio di errore
    {
        perror(messaggioErrore); // Stampiamo un messaggio prima del codice di errore stesso
        exit(EXIT_FAILURE);
    }
    return file;
}

void ChiudiFile(FILE *file, const char *messaggioErrore)
{
    if (fclose(file) != 0) // Se il file non si è chiuso, mostra un determinato messaggio di errore
    {
        perror(messaggioErrore); // Stampiamo un messaggio prima del codice di errore stesso
        exit(EXIT_FAILURE);
    }
}

void GeneraIBAN(char stringa[]) // Dichiarazione dei parametri che compongono un IBAN generico
{
    int min = 10, max = 99;

    char it[] = "IT";                           // Sigla nazionale
    int cineu = rand() % (max - min + 1) + min; // Codice di controllo europeo, genero un numero casuale tra il massimo e il minimo
    char cin = 'A' + rand() % 26;               // Codice di controllo interno, genero una lettera casuale dalla A alla Z (inclusi)
    char abi[] = "07648";                       // Codice identificativo banca
    char cab[] = "05433";                       // Codice identificativo filiale
    char zeri[] = "00000";                      // Zeri identificaativi numero conto
    char conto[8];                              // Numero conto, al quale gli assegniamo 8 caratteri

    for (int i = 0; i < 7; i++) // Generazione casuale del numero conto
    {
        conto[i] = '0' + rand() % (10); // Genera il valore causale partendo da 0 fino a 9 (inclusi)
    }
    conto[7] = '\0'; // Gli assegno il terminatore di stringhe '\0' all'ultima posizione della stringa 'conto'

    snprintf(stringa, 28, "%s%02d%c%s%s%s%s", it, cineu, cin, abi, cab, zeri, conto); // Unione dei singoli elementi In un'unica stringa IBAN
}

int SearchInFile(const char *colonna, const char *valore, char *rigaVuota)
{
    FILE *file = ApriFile(FILE_NAME, "r", "SearchFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'

    char rigaTemp[MAX_STR_LEN]; // Dichiaro la riga temporanea dove inserire il valore temporaneo
    char riga[MAX_STR_LEN];     // Dichiaro riga dove ci sarà il valore originale
    char *token;                // Dichiaro il delimitatore
    int indiceColonna = -1, userExists = 0, columnExists = 0;

    if (fgets(riga, MAX_BUFFER_LEN, file) != NULL) // Leggi il file e cerca la colonna specificata
    {
        strcpy(rigaTemp, riga);        // Copia la riga per non perdere dati con strcoy
        token = strtok(rigaTemp, ";"); // Assegno il delimitatore
        for (int i = 0; token; i++)
        {
            if (strcmp(token, colonna) == 0) // Controlla se la comparazione tra il token e la colonna è uguale a 0 (ovvero è vera)
            {
                indiceColonna = i; // Assegniamo l'indice all'indice della colonna
                columnExists = 1;  // Portiamo a true (1) il valore di 'columnExists
                break;             // Se trova la colonna, ferma il ciclo
            }
            token = strtok(NULL, ";"); // Se non trova la colonna, andiamo a specificargli il delimitatore da trovare
        }
    }

    if (!columnExists) // Se la colonna non è stata trovata
    {
        ChiudiFile(file, "SearchFail: la colonna specificata non esiste nel file"); // Chiusura file tramite funzione 'ChiudiFile'
        return -1;                                                                  // Ritorna -1
    }

    while (fgets(riga, MAX_BUFFER_LEN, file) != NULL) // Leggi il file e cerca la riga contenente il valore specificato
    {
        strcpy(rigaTemp, riga);        // Copia la riga per non perdere dati con strcpy
        token = strtok(rigaTemp, ";"); // Assegno il delimitatore
        for (int i = 0; token; i++)
        {
            if (i == indiceColonna && strcmp(token, valore) == 0)
            {
                strcpy(rigaVuota, riga);                                        // Copia nella stringa vuota la riga contenente i dati dell'utente
                ChiudiFile(file, "SearchFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
                return 0;                                                       // L'elemento è stato trovato
            }
            token = strtok(NULL, ";"); // Se non trova la colonna, andiamo a specificargli il delimitatore da trovare
        }
    }

    ChiudiFile(file, "SearchFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
    return 1;                                                       // L'elemento non esiste nel file
}

utente CreateUserStruct(char *RigaUtente)
{
    utente user;
    sscanf(RigaUtente, "%[^;];%[^;];%[^;];%f;%[^;];%[^;];",
           user.NomeUtente, user.Password, user.IBAN,
           &user.Saldo, user.Nome, user.Cognome);
    return user;
}

void RemoveLine(const char *colonna, const char *valore)
{
    FILE *file = ApriFile(FILE_NAME, "r", "EditFail: errore nell'apertura del file originale");           // Apertura file tramite funzione 'ApriFile'
    FILE *fileTemp = ApriFile(TEMP_FILE_NAME, "w", "EditFail: errore nell'apertura del file temporaneo"); // Apertura file tramite funzione 'ApriFile'

    char riga[MAX_STR_LEN];
    char RigaUtente[MAX_STR_LEN];

    if (SearchInFile(colonna, valore, RigaUtente) < 0) // Cerca la riga contenente il valore specificato nella colonna specificata
    {
        printf("SearchFail: la colonna specificata non esiste nel file.\n");
        ChiudiFile(file, "EditFail: errore nella chiusura del file originale");      // Chiusura file tramite funzione 'ChiudiFile'
        ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo"); // Chiusura file tramite funzione 'ChiudiFile'
        return;
    }
    else if (SearchInFile(colonna, valore, RigaUtente) > 0)
    {
        printf("SearchFail: l'elemento specificato non esiste nella colonna indicata.\n");
        ChiudiFile(file, "EditFail: errore nella chiusura del file originale");      // Chiusura file tramite funzione 'ChiudiFile'
        ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo"); // Chiusura file tramite funzione 'ChiudiFile'
        return;
    }
    else
    {
        rewind(file);
        while (fgets(riga, MAX_BUFFER_LEN, file) != NULL)
        {
            if (strcmp(riga, RigaUtente) != 0)
            {
                fprintf(fileTemp, "%s", riga);
            }
        }
    }

    ChiudiFile(file, "EditFail: errore nella chiusura del file originale");      // Chiusura file tramite funzione 'ChiudiFile'
    ChiudiFile(fileTemp, "EditFail: errore nella chiusura del file temporaneo"); // Chiusura file tramite funzione 'ChiudiFile'
}

void FileUpdate(const char *OldFile, const char *NewFile)
{
    FILE *NewFileT = fopen(TEMP_FILE_NAME, "r");
    bool aperturaFile = false;
    int rimozione, rinomina;
    if (!NewFileT) // Controlla se il file temporaneo esiste
    {
        // Il file non è aperto quindi no modifiche
        aperturaFile = false;
    }
    else // Il file si è aperto
    {
        aperturaFile = true;
        ChiudiFile(NewFileT, "UpdateFail: Errore durante la chiusura del file [utenti.csv]");
    }

    if (aperturaFile == true) // Elimina l'old file
    {
        rimozione = remove(OldFile);
        rinomina = rename(NewFile, OldFile);
        aperturaFile = false;
    }

    if ((rimozione < 0) || (rinomina < 0))
    {
        perror("UpdateFail: Errore durante la modifica del file [utenti.csv]");
        exit(EXIT_FAILURE);
    }
}

void Writing(char *NomeFile, const char *nomeUtente, const char *password, const char *iban, float saldo, const char *nome, const char *cognome)
{
    FILE *file = ApriFile(NomeFile, "a", "WriteFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'

    fprintf(file, "%s;%s;%s;%0.2f;%s;%s;\n", nomeUtente, password, iban, saldo, nome, cognome);

    ChiudiFile(file, "WriteFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
}

void WritingStruct(char *NomeFile, utente *user)
{
    FILE *file = ApriFile(NomeFile, "a", "WriteFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'
    fprintf(file, "%s;%s;%s;%0.2f;%s;%s;\n", user->NomeUtente, user->Password, user->IBAN, user->Saldo, user->Nome, user->Cognome);
    ChiudiFile(file, "WriteFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
}

void Transazione(utente *user, float importo)
{
    user->Saldo += importo;
    RemoveLine("NomeUtente", user->NomeUtente);
    WritingStruct(TEMP_FILE_NAME, user);
}

bool modificaUtente(utente *user)
{
    int scelta, contatoreModifiche = 0;
    float saldo;
    bool fineLoop = false;
    char nomeutente[MAX_STR_LEN], cognome[MAX_STR_LEN], nome[MAX_STR_LEN], password[MAX_STR_LEN];

    do
    {
        system("cls");
        printf("----------------------------------------------------------------\n");
        printf("Cosa vorresti modificare dell'utente: %s?\n", user->NomeUtente);
        printf("1 - NomeUtente\n");
        printf("2 - Password\n");
        printf("3 - Saldo\n");
        printf("4 - Nome\n");
        printf("5 - Cognome\n\n");
        printf("----------------------------------------------------------------\n");
        printf("6 - Applica le modifiche | 0 - Esci\n");
        printf("----------------------------------------------------------------\n");
        printf("Seleziona un'opzione: ");
        scanf("%d", &scelta);
        switch (scelta)
        {
        case 0:
            FileUpdate(FILE_NAME, TEMP_FILE_NAME);
            contatoreModifiche = 0;
            printf("Uscita In corso...\n");
            Sleep(1000); // Pausa
            fineLoop = true;
            return fineLoop;
            break;

        case 1:
            printf("Vecchio Nome Utente: %s\n", user->NomeUtente);
            RemoveLine("NomeUtente", user->NomeUtente);
            printf("Inserisci nuovo Nome Utente: ");
            scanf("%s", nomeutente);
            strcpy(user->NomeUtente, nomeutente);
            printf("Modifica Eseguita con successo!\n");
            contatoreModifiche++;
            Sleep(1000); // Pausa
            break;
        case 2:
            printf("Vecchia Password: %s\n", user->Password);
            RemoveLine("Password", user->Password);
            printf("Inserisci nuova Password: ");
            scanf("%s", password);
            strcpy(user->Password, password);
            printf("Modifica Eseguita con successo!\n");
            contatoreModifiche++;
            Sleep(1000); // Pausa
            break;
        case 3:
            printf("Vecchio Saldo: %0.2f\n", user->Saldo);
            printf("Inserisci l'importo: ");
            scanf("%f", &saldo);
            user->Saldo = saldo;
            RemoveLine("IBAN", user->IBAN);
            contatoreModifiche++;
            printf("Modifica Eseguita con successo!\n");
            Sleep(1000); // Pausa
            break;
        case 4:
            printf("Vecchio Nome: %s\n", user->Nome);
            RemoveLine("Nome", user->Nome);
            printf("Inserisci nuovo Nome: ");
            scanf("%s", nome);
            strcpy(user->Nome, nome);
            contatoreModifiche++;
            printf("Modifica Eseguita con successo!\n");
            Sleep(1000); // Pausmod
            break;
        case 5:
            printf("Vecchio Cognome: %s\n", user->Cognome);
            RemoveLine("Cognome", user->Cognome);
            printf("Inserisci nuova Cognome: ");
            scanf("%s", cognome);
            strcpy(user->Cognome, cognome);
            contatoreModifiche++;
            printf("Modifica Eseguita con successo!\n");
            Sleep(1000); // Pausa
            break;
        case 6:
            if (contatoreModifiche > 0)
            {
                RemoveLine("IBAN", user->IBAN);
                WritingStruct(TEMP_FILE_NAME, user);
                contatoreModifiche = 0;
            }
            else
            {
                printf("Non hai apportato modifiche all'utente selezionato.");
                Sleep(1000); // Pausa
            }
            break;
        default:
            system("cls");
            printf("InputFail: non puoi insere numeri diversi da quelli che sono scritti!\n");
            Sleep(1000); // Pausa
            break;
        }
    } while (fineLoop != true);
}

void stampaListaUtenti(FILE *f)
{
    char buffer[MAX_BUFFER_LEN], *tokens;
    int colonne = 0, posNC = 1;

    f = ApriFile(FILE_NAME, "r", "Non e' stato possibile aprire il file!"); // Stampa Nomi e Cognomi
    fgets(buffer, sizeof(buffer), f);
    printf("----------------------------------------------------------------\n");
    printf("   Utente             Nome      Cognome\n\n");
    while (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        colonne = 0;

        tokens = strtok(buffer, ";"); // Assegno il delimitatore
        printf("%d) ", posNC);
        printf("%-18s ", tokens);
        while (tokens)
        {
            if (colonne == 4)
            {
                printf("%-9s ", tokens);
            }
            if (colonne == 5)
            {
                printf("%-10s\n", tokens);
            }
            tokens = strtok(NULL, ";"); // Se non trova la colonna, andiamo a specificargli il delimitatore da trovare
            colonne++;
        }
        posNC++;
    }
    printf("----------------------------------------------------------------\n");
    ChiudiFile(f, "Non e' stato possibile chiudere il file!"); // Chiusura file tramite funzione 'ChiudiFile'
}

void utentiModifiche(FILE *f)
{
    bool fineModifiche = false;
    bool fineModificheUtente;
    char rigaUtente[MAX_STR_LEN];
    char nomeUtente[MAX_STR_LEN];
    int scelta;

    do
    {
        system("cls");
        printf("----------------------------------------------------------------\n");
        printf("1 - Scegli Utente\n\n");
        printf("----------------------------------------------------------------\n");
        printf("0 - Esci\n");
        printf("----------------------------------------------------------------\n");
        printf("Seleziona un'opzione: ");
        scanf("%d", &scelta);
        switch (scelta)
        {
        case 0:
            printf("Uscita In corso...\n");
            Sleep(1000); // Pausa
            fineModifiche = true;
            fineModificheUtente = true;
            break;
        case 1:
            system("cls");
            stampaListaUtenti(f);
            printf("Inserisci nome utente:");
            scanf("%s", nomeUtente);
            if (SearchInFile("NomeUtente", nomeUtente, rigaUtente) == 0)
            {
                utente user = CreateUserStruct(rigaUtente);
                fineModificheUtente = modificaUtente(&user);
            }
            break;

        default:
            system("cls");
            printf("InputFail: non puoi insere numeri diversi da quelli che sono scritti!\n");
            Sleep(1000); // Pausa
            break;
        }

    } while (fineModifiche != true && !fineModifiche);
}

void Admin()
{
    int scelta = 0;
    FILE *utenti;

    do
    {
        system("cls");
        printf("----------------------------------------------------------------\n");
        printf("Benvenuto Admin!\nDa questo pannello puoi controllare l'intero sistema bancario!\n");
        printf("Che cosa vorresti fare oggi?\n\n");
        printf("1 - Modifica Utenti\n\n");
        printf("----------------------------------------------------------------\n");
        printf("0 - Esci\n");
        printf("----------------------------------------------------------------\n");
        printf("Seleziona un'opzione: ");
        scanf("%d", &scelta);
        switch (scelta)
        {
        case 0:
            printf("Arrivederci!");
            break;
        case 1:
            system("cls");
            utentiModifiche(utenti);
            break;
        default:
            system("cls");
            printf("InputFail: non puoi insere numeri diversi da quelli che sono scritti!\n");
            Sleep(1000); // Pausa
            break;
        }

    } while (scelta != 0);
}
void bonifico(utente *user, utente *destinatario, float importo)
{
    user->Saldo -= importo;
    destinatario->Saldo += importo;
    RemoveLine("NomeUtente", user->NomeUtente);
    WritingStruct(TEMP_FILE_NAME, user);
    FileUpdate(FILE_NAME, TEMP_FILE_NAME);
    RemoveLine("NomeUtente", destinatario->NomeUtente);
    WritingStruct(TEMP_FILE_NAME, destinatario);
    FileUpdate(FILE_NAME, TEMP_FILE_NAME);
}

void EliminaConto(utente *user)
{
    char password[MAX_STR_LEN], confermaPassword[MAX_STR_LEN], IBAN[28], rigaDestinatario[MAX_BUFFER_LEN];
    int corrispondono, corretta;

    printf("\nInserisci l'IBAN destinatario: ");
    scanf("%s", IBAN);
    if (SearchInFile("IBAN", IBAN, rigaDestinatario) == 0)
    {
        utente destinatario = CreateUserStruct(rigaDestinatario);

        do
        {   
            printf("Inserisci la tua password: ");
            scanf("%s", password);
            printf("Conferma la tua password: ");
            scanf("%s", confermaPassword);

            corrispondono = strcmp(password, confermaPassword);
            corretta = strcmp(password, user->Password);

            if (corrispondono != 0)
            {
                printf("Le password non corrispondono");
            }
            else if (corretta != 0)
            {
                printf("Password errata");
            }
            Sleep(1000);
            system("cls");
            printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user->Nome, user->Cognome, user->Saldo);
            printf("----------------------------------------------------------------\n");
            printf("Continuando accetterai di chiudere il conto, eliminando le tue credenziali d'accesso\n");
            printf("Potrai specificare l'IBAN del conto su cui vuoi trasferire il tuo saldo attuale\n");
            printf("----------------------------------------------------------------\n");
            printf("IBAN destinatario confermato: %s\n", IBAN);
        } while (corretta != 0 || corrispondono != 0);

        bonifico(user, &destinatario, user->Saldo);
        RemoveLine("NomeUtente", user->NomeUtente);
        FileUpdate(FILE_NAME, TEMP_FILE_NAME);
    }
    else
    {
    printf("L'IBAN specificato non e' esistente");
    }
}

void Consumer(utente user)
{
    int scelta;
    float importo = 0;

    do // Ciclo di visualizzazione menu
    {
        Sleep(1000); // Pausa
        system("cls");

        printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
        printf("----------------------------------------------------------------\n");
        printf("Cosa si vuole fare:\n\n");
        printf("1 - Effettua un versamento\n");
        printf("2 - Effettua un prelievo\n");
        printf("3 - Effettua un bonifico\n");
        printf("4 - Visualizza le tue coordinate bancarie\n");
        printf("5 - Chiudi il tuo conto\n\n");
        printf("----------------------------------------------------------------\n");
        printf("0 - Esci dal conto\n");
        printf("----------------------------------------------------------------\n");
        printf("\nSeleziona un'opzione: ");

        if (scanf("%d", &scelta) != 1) // Controlla se l'input è un numero intero
        {

            while (getchar() != '\n'); // Pulisce l'input buffer
            printf("Il valore inserito non e' valido. Perfavore riprova\n");

            printf("\nCaricamento in corso...\n");
            Sleep(1000); // Pausa
            system("cls");

            continue; // Rientra nel ciclo per ottenere un valore corretto
        }

        if (scelta == 1) // Scelta dell'opzione
        {
            printf("\nCaricamento in corso...\n");
            Sleep(1000); // Pausa
            system("cls");
            printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
            printf("----------------------------------------------------------------\n");
            printf("Iserisci l'importo da versare: ");
            scanf("%f", &importo);
            if (importo < 0)
            {
                printf("Importo Errato!");
            }
            else
            {
                Transazione(&user, importo);
                FileUpdate(FILE_NAME, TEMP_FILE_NAME);
                printf("\nVersamento effettuato. L'importo versato sara' disponibile immediatamente\n");
            }
        }
        else if (scelta == 2)
        {
            printf("\nCaricamento in corso...\n");
            Sleep(1000); // Pausa
            system("cls");
            printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
            printf("----------------------------------------------------------------\n");
            printf("Iserisci l'importo da prelevare: ");
            scanf("%f", &importo);
            if (importo <= user.Saldo)
            {
                Transazione(&user, -importo);
                FileUpdate(FILE_NAME, TEMP_FILE_NAME);
                printf("\nPrelievo effettuato. Ritira i contanti dallo sportello in basso");
            }
            else
            {
                printf("\nSaldo insufficiente");
            }
        }

        else if (scelta == 3)
        {
            float importoBonifico = 0;
            char IBANdestinatario[28], rigaDestinatario[MAX_BUFFER_LEN];
            printf("\nCaricamento in corso...\n");
            Sleep(1000); // Pausa
            system("cls");
            printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
            printf("----------------------------------------------------------------\n");
            printf("Iserisci l'IBAN del destinatario: ");
            scanf("%s", IBANdestinatario);
            if (strcmp(IBANdestinatario, user.IBAN) != 0)
            {
                if (SearchInFile("IBAN", IBANdestinatario, rigaDestinatario) == 0)
                {
                    printf("Inserisci l'importo del bonifico: ");
                    scanf("%f", &importoBonifico);
                    if (importoBonifico <= user.Saldo && importoBonifico > 0)
                    {
                        utente destinatario = CreateUserStruct(rigaDestinatario);
                        bonifico(&user, &destinatario, importoBonifico);
                    }
                    else
                    {
                        printf("Saldo insufficiente / Formato errato");
                    }
                }
                else
                {
                    printf("L'IBAN specificato e' inesistente");
                }
            }
            else
            {
                printf("Impossibile effettuare un bonifico indirizzato a se stessi");
            }
        }

        else if (scelta == 4)
        {
            int scelta2;
            printf("\nCaricamento in corso...\n");
            do
            {
                Sleep(1000); // Pausa
                system("cls");
                printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
                printf("----------------------------------------------------------------\n");
                printf("Nome utente: %s\n", user.NomeUtente);
                printf("IBAN: %s\n", user.IBAN);
                printf("Paese: IT\n");
                printf("ABI: %s\n", ABI);
                printf("CAB: %s\n", CAB);
                printf("BIC/SWIFT: UNCODMMXXX\n\n");
                printf("----------------------------------------------------------------\n");
                printf("0 - Torna al conto\n");
                printf("----------------------------------------------------------------\n");
                printf("\nSeleziona un'opzione: ");

                if (scanf("%d", &scelta2) != 1)
                {

                    while (getchar() != '\n'); // Pulisce l'input buffer
                    printf("\nIl valore inserito non e' valido. Inserisci [ 0 ] per tornare al conto\n");
                    continue; // Rientra nel ciclo per ottenere un valore corretto
                }
                if (scelta2 != 0)
                {
                    printf("\nIl valore inserito non e' valido. Inserisci [ 0 ] per tornare al conto\n");
                }
            } while (scelta2 != 0);
            printf("\nCaricamento in corso...\n");
        }

        else if (scelta == 5)
        {
            int scelta3;
            printf("\nCaricamento in corso...\n");
            do
            {   
                Sleep(1000); // Pausa
                system("cls");
                printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
                printf("----------------------------------------------------------------\n");
                printf("Continuando accetterai di chiudere il conto, eliminando le tue credenziali d'accesso\n");
                printf("Potrai specificare l'IBAN del conto su cui vuoi trasferire il tuo saldo attuale\n");
                printf("----------------------------------------------------------------\n");
                printf("\n1 - Continua e chiudi il conto\n\n");
                printf("----------------------------------------------------------------\n");
                printf("0 - Annulla e torna al conto\n");
                printf("----------------------------------------------------------------\n");
                printf("\nIserisci un'opzione: ");
                if (scanf("%d", &scelta3) != 1) // Controlla se l'input è un numero intero
                {

                    while (getchar() != '\n'); // Pulisce l'input buffer
                    printf("Il valore inserito non e' valido. Perfavore riprova\n");
                    printf("\nCaricamento in corso...\n");
                    continue; // Rientra nel ciclo per ottenere un valore corretto
                }

                if (scelta3 == 1)
                {   
                    system("cls");
                    printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user.Nome, user.Cognome, user.Saldo);
                    printf("----------------------------------------------------------------\n");
                    printf("Continuando accetterai di chiudere il conto, eliminando le tue credenziali d'accesso\n");
                    printf("Potrai specificare l'IBAN del conto su cui vuoi trasferire il tuo saldo attuale\n");
                    printf("----------------------------------------------------------------");
                    EliminaConto(&user);
                    printf("\nConto eliminato con successo. Reindirizzamento al menu in corso...");
                    scelta = 0;
                }
            } while (scelta3 != 0 && scelta3 != 1);
        }

        else if (scelta == 0)
        {
            printf("\nReindirizzamento al menu in corso...\n");
        }
        else
        {
            printf("\nIl valore inserito non e' valido. Perfavore riprova\n"); // Gestisci input non validi
        }
    } while (scelta != 0);
}

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
        printf("Inserisci la tua password: ");
        scanf("%s", InputPassword);

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
                printf("\nUtente [ %s ] autenticato con successo\n\nIngresso nel conto...\n", user.NomeUtente);
                Consumer(user);
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

void IsFileExists()
{
    FILE *file = ApriFile(FILE_NAME, "r", "CreateFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'

    if (file == NULL) // Se il file non esiste, crealo
    {
        FILE *file = ApriFile(FILE_NAME, "w", "CreateFail: errore nella creazione del file del file"); // Apertura file tramite funzione 'ApriFile'

        if (file != NULL) // Se il file e' stato appena creato, stampa l'header
        {
            fprintf(file, "NomeUtente;Password;IBAN;Saldo;Nome;Cognome;\n");
        }
    }
    ChiudiFile(file, "CreateFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
}

int UserExists(const char *InputNomeUtente)
{
    FILE *file = ApriFile(FILE_NAME, "r", "UserFail: errore nell'apertura del file"); // Apertura file tramite funzione 'ApriFile'
    char buffer[MAX_BUFFER_LEN];

    fgets(buffer, MAX_BUFFER_LEN, file); // Salta la prima riga contenente l'header

    while (fgets(buffer, MAX_BUFFER_LEN, file) != NULL) // Scansione del file riga per riga
    {
        char StoredNomeUtente[100];
        sscanf(buffer, "%[^;]", StoredNomeUtente);

        if (strcmp(InputNomeUtente, StoredNomeUtente) == 0)
        {
            printf("\nUtente [ %s ] gia' registrato. Riprova\n", InputNomeUtente);
            ChiudiFile(file, "UserFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
            return 1;                                                     // Indica che il nome utente esiste nel file
        }
    }

    ChiudiFile(file, "UserFail: errore nella chiusura del file"); // Chiusura file tramite funzione 'ChiudiFile'
    return 0;                                                     // Indica che il nome utente non esiste nel file
}

void Register()
{
    IsFileExists();   // Richiamo la funzione per controllare se esiste il file CSV
    GeneraIBAN(IBAN); // Richiamo la funzione per generare un IBAN
    float Saldo = 0.00;
    int FineRegistrazione = 0, controlloUtente = 0, Delimiter, provaPassword;

    do // Ciclo visualizzazione menu registrazione
    {
        printf("\nCaricamento in corso...\n");
        Sleep(1000); // Pausa
        system("cls");

        printf("Compila il seguente form per aprire il conto\n");

        printf("Inserisci il tuo nome: ");
        scanf("%s", Nome);
        printf("Inserisci il tuo cognome: ");
        scanf("%s", Cognome);
        printf("Inserisci un nome utente: ");
        scanf("%s", NomeUtente);
        printf("Inserisci una password: ");
        scanf("%s", Password);
        printf("Conferma la tua password: ");
        scanf("%s", ConfermaPassword);

        provaPassword = strcmp(Password, ConfermaPassword); // Comparazione tra la prima e la seconda password

        Delimiter = 0; // Inizializzo il flag Delimiter a falso

        for (int i = 0; NomeUtente[i]; i++) // Verifica se l'utentente inserisce un punto e virgola
        {
            NomeUtente[i] = tolower(NomeUtente[i]);
            if (NomeUtente[i] == ';' || Nome[i] == ';' || Cognome[i] == ';' || Password[i] == ';' || ConfermaPassword[i] == ';')
            {
                Delimiter = 1;
                break;
            }
        }
        if (provaPassword != 0)
        {
            printf("\nLe due password non corrispondono\n");
        }

        if (Delimiter) // Se i dati inseriti dall'utente contengono il delimiter, richiede di reinserire i dati
        {
            printf("\nCarattere \";\" non valido. Riprova:\n");
        }
        if (Delimiter == 0 && provaPassword == 0)
        {
            FineRegistrazione = 1;
        }
    } while (Delimiter || UserExists(NomeUtente) != 0 || FineRegistrazione != 1);

    Writing(FILE_NAME, NomeUtente, Password, IBAN, Saldo, Nome, Cognome);

    printf("\nRegistrazione avvenuta con successo\n\nReindirizzamento al menu in corso...\n");
}

void Banca()
{
    int scelta;

    Sleep(1000); // Pausa
    system("cls");
    do // Ciclo visualizzazione menu banca
    {
        printf("Benvenuto nella UniCode Bank\n");
        printf("----------------------------------------------------------------\n");
        printf("Cosa si vuole fare:\n\n");
        printf("1 - Entrare nel conto\n");
        printf("2 - Aprire un conto\n\n");
        printf("----------------------------------------------------------------\n");
        printf("0 - Esci dal programma\n");
        printf("----------------------------------------------------------------\n");
        printf("\nSeleziona un'opzione: ");

        if (scanf("%d", &scelta) != 1) // Controlla se l'input è un numero intero
        {
            while (getchar() != '\n') // Pulisce l'input buffer
                printf("\nIl valore inserito non e' valido. Perfavore riprova\n");
            printf("\nCaricamento in corso...\n");
            Sleep(1000); // Pausa
            system("cls");
            continue; // Rientra nel ciclo per ottenere un valore corretto
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