#include "general.c"

/*
    Parametro: array di caratteri vuoto

    Genera un IBAN casuale e lo copia nell'array inserito come parametro.
    Importante notare che l'array deve avere 28 celle/elementi.
*/
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

/*
    Primo parametro: 
*/
void Transazione(utente *user, float importo)
{
    user->Saldo += importo;
    RemoveLine("NomeUtente", user->NomeUtente);
    WritingStruct(TEMP_FILE_NAME, user);
}

/*
    Primo parametro:  puntatore a struct mittente del bonifico;
    Secondo parametro: puntatore a struct destinatario bonifico;
    Terzo parametro: float importo del bonifico.

    Effettua il bonifico, sottraendo l'importo al mittende e sommandolo al destinatario.
*/
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

/*
    Parametro: puntatore a struct dell'utente autenticato

    Elimina dal file utenti la riga contenente i dati dell'utente. 
    Trasferisce il saldo disponibile sul conto specificato tramite IBAN.
*/
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
        printf("Menu:\n\n");
        printf("1 - Effettua un versamento\n");
        printf("2 - Effettua un prelievo\n");
        printf("3 - Effettua un bonifico\n");
        printf("4 - Visualizza le tue coordinate bancarie\n");
        printf("5 - Chiudi il tuo conto\n\n");
        printf("----------------------------------------------------------------\n");
        printf("0 - Esci dal conto\n");
        printf("----------------------------------------------------------------\n");

        scelta = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 5);
        if (scelta == -1)
        {   
            continue;
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
                printf("Importo Errato! Operazione annullata");
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
            if (importo <= 0)
            {
                printf("\nImporto Errato! Operazione annullata\n");
            }
            else if (importo <= user.Saldo)
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

                scelta2 = ReadInputChoice("Inserisci 0 per uscire: ", 0, 0);
                if (scelta2 == -1)
                {   
                    continue;
                }
                if (scelta2 != 0)
                {
                    printf("\nIl valore inserito non e' valido. Inserisci 0 per tornare al conto\n");
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
                scelta3 = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 2);
                if (scelta3 == -1)
                {   
                    Sleep(1000);
                    system("cls");
                    continue;
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

void Register()
{   
    char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100], IBAN[28];
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
        // scanf("%s", Nome);
        fgets(Nome, MAX_STR_LEN, stdin);
        Nome[strcspn(Nome, "\n")] = '\0'; // Rimuove il carattere newline dalla fine della stringa

        printf("Inserisci il tuo cognome: ");
        fgets(Cognome, MAX_STR_LEN, stdin);
        Cognome[strcspn(Cognome, "\n")] = '\0'; // Rimuove il carattere newline dalla fine della stringa

        printf("Inserisci un nome utente: ");
        scanf("%s", NomeUtente);
        printf("Inserisci una password: ");
        scanf("%s", Password);
        pulisciBuffer();
        printf("Conferma la tua password: ");
        scanf("%s", ConfermaPassword);
        pulisciBuffer();

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
