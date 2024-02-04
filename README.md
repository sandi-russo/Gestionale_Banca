# README - Progetto di Gestione Bancaria: Unicode Bank

Programma scritto in C che simula la gestione di una banca, fornendo funzionalità sia per gli utenti consumer che per gli utenti amministratori.

## Sviluppatori

Il programma è stato programmato e scritto con impegno e dedizione da tre studenti:

- Danil Parrello
- Samuele Bruno
- Sandi Russo

## Contesto Accademico e Scopo del Progetto

Questo progetto informatico è stato concepito nel contesto del corso di Programmazione condotto dal Professore Francesco Maiorana, svolto durante l'anno accademico 2023/2024 presso l'Università degli Studi di Messina.

Il principale obiettivo del progetto era l'applicazione e la solidificazione di un ampio spettro di conoscenze acquisite nel corso, integrandole attraverso approfondimenti individuali. L'obiettivo finale era conferire al sistema di gestione bancaria un elevato grado di praticità e funzionalità.

Il codice implementa competenze avanzate, comprese l'uso di puntatori, la gestione dei file, la creazione di strutture dati complesse, la definizione di funzioni modulari, la manipolazione di vari tipi di dati e conversioni, la gestione di vettori e la manipolazione di stringhe, oltre all'allocazione dinamica di memoria.

In aggiunta, un obiettivo centrale del progetto è stato promuovere la cooperazione, la coordinazione e il lavoro di gruppo tra gli sviluppatori. Questa sinergia ha contribuito a consolidare le competenze individuali e a garantire un risultato finale coerente e di qualità.

## Struttura e Organizzazione del Codice

Il codice sorgente è organizzato in modo chiaro e modulare:
 
- Il file "Main.h" funge da header, contenente tutte le strutture di dati, le librerie e le costanti neccessarie per il corretto funzionamento del programma;

- Il file "general.c" contiene la definizione delle funzioni a scopo generale che sono utili durante tutta l'esecuzione del programma, ad esempio la parte relativa alla gestione file;

- Il file "consumer.c" contiene la definizione delle funzioni relative al campo dell'utente consumer;

- Il file "admin.c" contiene la definizione delle funzioni relative alla gestione dei conti bancari da parte dell'amministrazione;

- Il file "bank.c" contiene la fase di autenticazione e l'implementazione finale delle funzioni nella simulazione bancaria;

- Il file "main.c" contiene l'esecuzione finale del programma, garantendo una struttura ordinata e comprensibile;

- Il file "utenti.csv" funge da dispositivo di archiviazione permanente, memorizzando in una riga le informazioni riguardanti un singolo utente, organizzate per campi adeguatamente separati, facilitandone la lettura ed aumentando l'efficienza generale del programma;

- il file "temp.c", visibile solo durante l'esecuzione di specifici comandi, consente di gestire in modo sicuro e funzionale le modifiche all'archiviazione permanente primaria contenuta "in utenti.csv".

## Istruzioni per l'Esecuzione

Per eseguire il programma, è sufficiente compilare il file sorgente utilizzando il compilatore C di tua scelta. Assicurati di avere installato tutte le dipendenze necessarie.

Dopo la compilazione, esegui il programma e segui le istruzioni visualizzate a schermo per utilizzare le diverse funzionalità bancarie simulate.

## Nozioni e Conoscenze Applicate

Nel processo di sviluppo del codice, come precedentemente menzionato, sono state progressivamente integrate le competenze acquisite durante il corso di Programmazione, tenuto dal Professore Francesco Maiorana presso l'Università degli Studi di Messina. L'obiettivo principale era applicare e consolidare il più ampio spettro possibile di nozioni, arricchendole attraverso approfondimenti e studio individuale, al fine di conferire al sistema di gestione bancaria un livello di praticità e funzionalità elevato.

Il codice sfrutta diverse competenze avanzate, tra cui l'uso di puntatori, gestione dei file, implementazione di strutture dati complesse, definizione di funzioni modulari, manipolazione di vari tipi di dati e conversioni, gestione di vettori e manipolazione di stringhe, l'allocazione dinamica di memoria nonché l'implementazione delle liste.

## Stato Attuale del Progetto e Prospettive Future

Al momento, il programma si trova in una fase di sviluppo continuo, con l'obiettivo di raffinare ulteriormente le sue funzionalità e migliorare l'efficienza del codice. Gli sviluppatori stanno continuamente ampliando le proprie conoscenze al fine di implementare migliorie significative nel sistema.

Si prevede che, man mano che gli sviluppatori acquisiranno competenze aggiuntive e rafforzeranno la loro comprensione delle dinamiche del progetto, verranno pubblicati regolari aggiornamenti per garantire una continua ottimizzazione delle prestazioni e l'introduzione di nuove funzionalità.

Per favorire la collaborazione e coinvolgere la comunità di utenti, gli sviluppatori accettano con gratitudine feedback relativi a bug o suggerimenti che possano contribuire al perfezionamento del progetto. Eventuali segnalazioni possono essere comunicate attraverso i canali dedicati, promuovendo un dialogo aperto e costruttivo.

Grazie per il vostro interesse e supporto continuo. Siamo entusiasti di crescere e migliorare insieme alla nostra comunità di utenti.