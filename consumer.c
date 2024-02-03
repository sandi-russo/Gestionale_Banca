#ifndef CONSUMER_C
#define CONSUMER_C

#include "General.c"

/*
    Parametro: array di caratteri vuoto

    Genera un IBAN casuale e lo copia nell'array inserito come parametro.
    Importante notare che l'array deve avere 28 celle.
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
    Primo parametro: punatore alla struttura di tipo utente (destinatario)
    Secondo parametro: float (importo della transazione)

    La funzione modifica il campo saldo della struttura puntata inserendo sommando
    l'importo specificato, che sia positivo o negativo in base al tipo della transazione.
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

    Effettua il bonifico, sottraendo l'importo al mittente e sommandolo al destinatario.
*/
void bonifico(utente *mittente, utente *destinatario, float importo)
{
    Transazione(mittente, -importo);
    Transazione(destinatario, importo);
    RemoveLine("NomeUtente", mittente->NomeUtente);
    WritingStruct(TEMP_FILE_NAME, mittente);
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
int EliminaConto(utente *user)
{
    char password[MAX_STR_LEN], confermaPassword[MAX_STR_LEN], IBAN[28], rigaDestinatario[MAX_BUFFER_LEN];
    int corrispondono, corretta, numeroConti = CountUserNum();

    do
    {   
        system("cls");
        printf("Titolare del conto: \033[0;32m%s %s\033[0m | Saldo Corrente: \033[0;32m%0.2f\n\033[0m", user->Nome, user->Cognome, user->Saldo);
        Divisore();
        TintedPrint('r', "CCONTINUANDO ACCETTERAI DI CHIUDERE IL TUO CONTO ELIMINANDO LE TUE CREDENZIALI D'ACCESSO!\n");

        if (user->Saldo > 0 && numeroConti > 1) // Se l'utente ha saldo disponibile e c'e' un conto su cui trasferirlo
        {   
            TintedPrint('r', "Dovrai specificare l'IBAN del conto su cui vuoi trasferire il tuo saldo attuale.");
            Divisore();
            printf("\nInserisci l'IBAN destinatario: ");
            scanf("%s", IBAN);

            if (SearchInFile("IBAN", IBAN, rigaDestinatario) == 0)
            {   
                if (strcmp(IBAN, user->IBAN) != 0)
                {
                    utente destinatario = CreateUserStruct(rigaDestinatario);

                    system("cls");
                    printf("Titolare del conto: %s %s | Saldo Corrente: %0.2f\n", user->Nome, user->Cognome, user->Saldo);
                    Divisore();
                    TintedPrint('r', "CONTINUANDO ACCETTERAI DI CHIUDERE IL TUO CONTO ELIMINANDO LE TUE CREDENZIALI D'ACCESSO!\n");
                    TintedPrint('r', "Dovrai specificare l'IBAN del conto su cui vuoi trasferire il tuo saldo attuale.");
                    Divisore();
                    printf("\033[0;32m"); // Sequenza di escape per il colore verde
                    printf("\nIBAN destinatario confermato: %s\n", IBAN);
                    printf("\033[0m"); // Sequenza di escape per il colore predefinito
                    bonifico(user, &destinatario, user->Saldo);
                }
                else
                {
                    TintedPrint('r', "\nImpossibile effettuare un bonifico a se stessi.");
                    return 0; // Conto non eliminato
                }
            }
            else
            {
                TintedPrint('r', "\nL'IBAN specificato non e' esistente. Annullamento operazione in corso...\n");
                return 0; // L'utente non è stato eliminato
            }
        }
        else  // Se l'utente non ha saldo o il conto e' l'unico registrato
        {   
            Divisore();
            char rigaAdmin[MAX_BUFFER_LEN];
            SearchInFile("NomeUtente", "admin_code", rigaAdmin);
            utente Admin = CreateUserStruct(rigaAdmin);
            bonifico(user, &Admin, user->Saldo);
        }

        printf("\nInserisci la tua password: ");
        scanf("%s", password);
        printf("Conferma la tua password: ");
        scanf("%s", confermaPassword);

        corrispondono = strcmp(password, confermaPassword);
        corretta = strcmp(password, user->Password);

        if (corrispondono != 0)
        {
            TintedPrint('r', "\nLe password non corrispondono.\n");
            Sleep(1500);
        }
        else if (corretta != 0)
        {
            TintedPrint('r', "\nPassword errata.\n");
            Sleep(1500);
        }
    } while (corretta != 0 || corrispondono != 0);

    RemoveLine("NomeUtente", user->NomeUtente);
    FileUpdate(FILE_NAME, TEMP_FILE_NAME);
    TintedPrint('g', "\nConto eliminato con successo. Reindirizzamento al menu in corso...\n");
    return 1;          // L'utente è stato eliminato
}

/*
    Primo parametro: struttura di tipo utente (utente autenticato)

    La funzione stampa il menu dell'utente consumer, tramite il quale
    è possibile effettuare le classiche operazioni bancarie
*/
void Consumer(utente user)
{
    int scelta;
    float importo = 0;

    do // Ciclo di visualizzazione menu
    {
        Sleep(1500); // Pausa
        system("cls");

        printf("Titolare del conto: \033[0;32m%s %s\033[0m | Saldo Corrente: \033[0;32m%0.2f\n\033[0m", user.Nome, user.Cognome, user.Saldo);
        Divisore();
        printf("Menu:\n\n");
        printf("1 - Effettua un versamento\n");
        printf("2 - Effettua un prelievo\n");
        printf("3 - Effettua un bonifico\n");
        printf("4 - Visualizza le tue coordinate bancarie\n");
        printf("5 - Chiudi il tuo conto\n\n");
        Divisore();
        TintedPrint('r', "0 - Esci dal conto\n");
        Divisore();

        scelta = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 5);
        if (scelta == -1)
        {
            continue;
        }

        if (scelta == 1) // Scelta dell'opzione
        {
            TintedPrint('b',"\nCaricamento in corso...");
            Sleep(1500);       // Pausa
            system("cls");
            printf("Titolare del conto: \033[0;32m%s %s\033[0m | Saldo Corrente: \033[0;32m%0.2f\n\033[0m", user.Nome, user.Cognome, user.Saldo);
            Divisore();
            printf("Iserisci l'importo da versare: ");
            scanf("%f", &importo);
            pulisciBuffer();
            if (importo < 0)
            {
                TintedPrint('r', "Importo Errato. Operazione annullata.");
            }
            else
            {
                Transazione(&user, importo);
                FileUpdate(FILE_NAME, TEMP_FILE_NAME);
                TintedPrint('g', "\nVersamento effettuato. L'importo versato sara' disponibile immediatamente.\n");
            }
        }
        else if (scelta == 2)
        {
            TintedPrint('b',"\nCaricamento in corso...");
            Sleep(1500);       // Pausa
            system("cls");
            printf("Titolare del conto: \033[0;32m%s %s\033[0m | Saldo Corrente: \033[0;32m%0.2f\n\033[0m", user.Nome, user.Cognome, user.Saldo);
            Divisore();
            printf("Iserisci l'importo da prelevare: ");
            scanf("%f", &importo);
            pulisciBuffer();
            if (importo <= 0)
            {
                TintedPrint('r', "\nImporto Errato. Operazione annullata.\n");
            }
            else if (importo <= user.Saldo)
            {
                Transazione(&user, -importo);
                FileUpdate(FILE_NAME, TEMP_FILE_NAME);
                TintedPrint('g', "\nPrelievo effettuato. Ritira i contanti dallo sportello in basso.\n");
            }
            else
            {
                TintedPrint('r', "\nSaldo insufficiente. Operazione annullata.\n");
            }
        }

        else if (scelta == 3)
        {
            float importoBonifico = 0;
            char IBANdestinatario[28], rigaDestinatario[MAX_BUFFER_LEN];
            TintedPrint('b',"\nCaricamento in corso...");
            Sleep(1500);       // Pausa
            system("cls");
            printf("Titolare del conto: \033[0;32m%s %s\033[0m | Saldo Corrente: \033[0;32m%0.2f\n\033[0m", user.Nome, user.Cognome, user.Saldo);
            Divisore();
            printf("Iserisci l'IBAN del destinatario: ");
            scanf("%s", IBANdestinatario);
            pulisciBuffer();
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
                        if (importoBonifico >= user.Saldo)
                        {
                            TintedPrint('r', "\nSaldo insufficiente. Operazione annullata.\n");
                        }
                        else
                        {
                            TintedPrint('r', "\nImporto Errato. Operazione annullata.\n");
                        }

                    }
                }
                else
                {
                    TintedPrint('r', "\nL'IBAN specificato risulta inesistente.\n");
                }
            }
            else
            {
                TintedPrint('r', "\nImpossibile effettuare un bonifico a se stessi.\n");
            }
        }

        else if (scelta == 4)
        {
            TintedPrint('b',"\nCaricamento in corso...");
            Sleep(1500); // Pausa
            system("cls");
            printf("Titolare del conto: \033[0;32m%s %s\033[0m | Saldo Corrente: \033[0;32m%0.2f\n\033[0m", user.Nome, user.Cognome, user.Saldo);
            Divisore();
            printf("Nome utente: \033[0;32m%s\033[0m\n", user.NomeUtente);
            printf("IBAN: \033[0;32m%s\033[0m\n", user.IBAN);
            printf("Paese: \033[0;32mIT\033[0m\n");
            printf("ABI: \033[0;32m%s\033[0m\n", ABI);
            printf("CAB: \033[0;32m%s\033[0m\n", CAB);
            printf("BIC/SWIFT: \033[0;32mUNCODMMXXX\033[0m\n");
            Divisore();

            TintedPrint('r', "\nPremi invio per uscire...");
            pulisciBuffer();

            TintedPrint('b',"\nCaricamento in corso...");
        }

        else if (scelta == 5)
        {
            int scelta3;
            TintedPrint('b',"\nCaricamento in corso...");
            do
            {
                Sleep(1500); // Pausa
                system("cls");
                printf("Titolare del conto: \033[0;32m%s %s\033[0m | Saldo Corrente: \033[0;32m%0.2f\n\033[0m", user.Nome, user.Cognome, user.Saldo);
                Divisore();
                TintedPrint('r', "CONTINUANDO ACCETTERAI DI CHIUDERE IL TUO CONTO ELIMINANDO LE TUE CREDENZIALI D'ACCESSO!\n");
                Divisore();
                TintedPrint('r', "\n1 - Continua e chiudi il conto\n\n");
                Divisore();
                TintedPrint('g', "0 - Annulla ed esci\n");
                Divisore();
                scelta3 = ReadInputChoice("Inserisci il numero corrispondente alla tua scelta: ", 0, 2);
                if (scelta3 == -1)
                {   
                    continue;
                }

                if (scelta3 == 1)
                {
                    if (EliminaConto(&user) == 1)
                    {
                        scelta = 0;
                    }
                    pulisciBuffer();
                }
            } while (scelta3 != 0 && scelta3 != 1);
        }

        else if (scelta == 0)
        {
            TintedPrint('b', "\nReindirizzamento al menu in corso...\n");
        }
    } while (scelta != 0);
}

/*
    La funzione permette di aprire un conto salvandone le informazioni sul file
    csv principale.
*/
void Register()
{
    char Nome[100], Cognome[100], NomeUtente[100], Password[100], ConfermaPassword[100], IBAN[28];
    IsFileExists();   // Richiamo la funzione per controllare se esiste il file CSV
    GeneraIBAN(IBAN); // Richiamo la funzione per generare un IBAN
    float Saldo = 0.00;
    int HasDelimiter = 0;
    char delimitatore = ';';

    do // Ciclo visualizzazione menu registrazione
    {
        HasDelimiter = 0;
        TintedPrint('b',"\nCaricamento in corso...");
        Sleep(1500); // Pausa
        system("cls");
        printf("\033[0;34mUniCode\033[0m \033[0;32mBank\033[0m | Compila il seguente form per aprire il conto:\n");
        Divisore();

        printf("Inserisci il tuo nome: ");
        // scanf("%s", Nome);
        fgets(Nome, MAX_STR_LEN, stdin);
        Nome[strcspn(Nome, "\n")] = '\0'; // Rimuove il carattere newline dalla fine della stringa

        printf("Inserisci il tuo cognome: ");
        fgets(Cognome, MAX_STR_LEN, stdin);
        Cognome[strcspn(Cognome, "\n")] = '\0'; // Rimuove il carattere newline dalla fine della stringa

        do
        { 
            
            system("cls");
            printf("\033[0;34mUniCode\033[0m \033[0;32mBank\033[0m | Compila il seguente form per aprire il conto:\n");
            Divisore();
            printf("Inserisci il tuo nome: %s\n", Nome);
            printf("Inserisci il tuo cognome: %s\n", Cognome);
            printf("Inserisci un nome utente: ");
            scanf("%s", NomeUtente);
            ToLower(NomeUtente);

            if (UserExists(NomeUtente) != 0)
            {
                printf("\033[0;31m\nIl nome utente [%s] e' gia' esistente.\033[0m\n", NomeUtente);
                Sleep(1500);
            }

        } while (UserExists(NomeUtente) != 0);

        do
        {
            system("cls");

            printf("\033[0;34mUniCode\033[0m \033[0;32mBank\033[0m | Compila il seguente form per aprire il conto:\n");
            Divisore();
            printf("Inserisci il tuo nome: %s\n", Nome);
            printf("Inserisci il tuo cognome: %s\n", Cognome);
            printf("Inserisci un nome utente: %s\n", NomeUtente);
            Divisore();
            TintedPrint('b', "Criteri minimi password: una maiuscola, una minuscola, un numero e un carattere speciale\n");
            TintedPrint('b', "Lunghezza minima password: 5 caratteri\n");
            Divisore();
            printf("\nInserisci una password: ");
            scanf("%s", Password);
            pulisciBuffer();

            if (!VerificaPassword(Password))
            {
                TintedPrint('r', "\nLa tua password non soddisfa i criteri specificati. Riprova:\n");
                Sleep(1500);
            }
        } while (!VerificaPassword(Password));

        do
        {
            system("cls");

            printf("\033[0;34mUniCode\033[0m \033[0;32mBank\033[0m | Compila il seguente form per aprire il conto:\n");
            Divisore();
            printf("Inserisci il tuo nome: %s\n", Nome);
            printf("Inserisci il tuo cognome: %s\n", Cognome);
            printf("Inserisci un nome utente: %s\n", NomeUtente);
            Divisore();
            TintedPrint('b', "Criteri minimi password: una maiuscola, una minuscola, un numero e un carattere speciale\n");
            TintedPrint('b', "Lunghezza minima password: 5 caratteri\n");
            Divisore();
            printf("\nInserisci una password: %s\n", Password);
            printf("Conferma la tua password: ");
            scanf("%s", ConfermaPassword);
            pulisciBuffer();

            if (strcmp(Password, ConfermaPassword) != 0) // Comparazione tra la prima e la seconda password
            {
                TintedPrint('r', "\nLe due password non corrispondono.\n");
                Sleep(1500);
            }
        } while (strcmp(Password, ConfermaPassword) != 0);

        // Verifica se l'utentente inserisce un punto e virgola
        if (HasChar(Nome, delimitatore) || HasChar(Cognome, delimitatore) || HasChar(NomeUtente, delimitatore) || HasChar(Password, delimitatore))
        {
            HasDelimiter = 1;
        }

        if (HasDelimiter) // Se i dati inseriti dall'utente contengono il delimiter, richiede di reinserire i dati
        {
            printf("\033[0;31m\nCarattere \"%c\" non valido. Riprova:\n\033[0m", delimitatore);
        }
    } while (HasDelimiter);

    Writing(FILE_NAME, NomeUtente, Password, IBAN, Saldo, Nome, Cognome);
    TintedPrint('g', "\nRegistrazione avvenuta con successo. Reindirizzamento al menu in corso...\n");
}

#endif // CONSUMER_C
