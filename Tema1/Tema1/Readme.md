### Tema 1 - SCD
#### Ionescu Cristina 341C3

# Precizari
- am adaugat newline in fisierele de test (s-a spus in cerinta ca putem)
- am modificat *oauth_svc.c* pentru a ma folosi de mainul lui pentru a 
gestiona argumentele de la prornirea serverului si retinerea datelor din
fisiere
- am modificat si "oauth_client.c" si "oauth_server.c" pentru a face
implementarile pentru rezolvartea requesturilor\operatiilor
- sa nu se ruleze rpcgen sau Makefile.oauth pentru a nu se suprascrie 
fisierele mentionate mai sus
- vor fi cateva inconcordante de nume pentru ca nu intelesesem pe deplin
cerinta cand m-am apucat de tema (a fost cam greu de inteles) si nu am 
mai avut timp sa le modific

# oauth.x
- fisierul pentru protocolul de comunicatie
- avem 6 functii (4 necesare pentru implementare, 2 facute de mine ca sa ma
ajute):
    * REQUEST_AUTHORIZATION - functia pntru autorizare
    * REQUEST_AUTHORIZATION_TOKEN - functie pentru generarea tokenurilor de 
    acces si refresh(la nevoie)
    * VALIDATE_DELEGATED_ACTION - fucunctie pentru gestionarea operatiilor
    date de client
    * APPROVE_REQUEST_TOKEN - functie pentru validarea tokenului de autentificare
    * GET_ACCES_TOKEN_AND_HANDEL_TTL - returneaza tokenul de acces si ttl-ul pe
    baza id-ului pentru un client
    * INCREASE_COUNT - creste indexul la care s-a ajuns in vectorul de drepturi
    (functia nu mai este folosita dar nu am mai avut timp sa o elimin)

- pe langa functii am mai facut si niste structuri pentru cand se paseaza mai
multe argumente la o functie sau cand aceasta intoarce mai multe elemente

# server
- implementarea mainului a fost facuta in *oauth_svc.c*
- aici am incarcat din fisiere toate datele in "baza de date"
- pentru useri am ales sa fac o lista simplu inlantuita in care sa imi pun mai
multe date pe parcurs, la inceput incep doar cu id si ttl
- pentru resurse am un vector de stringur care este dinamic, incepe de la lungimea
de 10 apoi va creste cu 10 de fiecare data cand este nevoie de mai mult spatiu
- pentru aprobari am un vector de liste (fiecare lista va contine ce este pe o 
linie din fisier)
- ttl-ul va fi retinut global
- dupa inchiderea serverului eliberam resursele

- in *oauth_server.c* avem implementarea functiilor din oauth.x
    * REQUEST_AUTHORIZATION:
        * facem tratarea requestului in server, daca exita un user cu id-ul
        primit generam token de autenificare, daca nu trimitem clientului 
        eroare
    * REQUEST_AUTHORIZATION_TOKEN:
        * pe langa argumentele date in cerinta am mai adaugat inca unul care
        spune daca este request de refresh sau normal
        * generam tokenul de acces, si daca este cazul si pe cel de refresh + 
        facem afisarea respectiva in server
    * VALIDATE_DELEGATED_ACTION:
        * verificam daca are drepturi, daca resursa data de client exista, daca
        clientul are tokenul expirat (pt cazul normal), pentru cazurile astea 
        intoarcem eroare
        * daca am ajuns pana aici, verificam daca tokenul e expirat si daca are 
        refresh => regeneram acc token si refresh token, apoi verificam daca 
        userul are permisiunile si resursele necesare executarii operatiei cerute
    * APPROVE_REQUEST_TOKEN:
        * aici se face validarea tokenului de autentificare; eu am ales sa fac o 
        operatie de genul 'Z' - c pe ultimul caracter al tokenului
    * GET_ACCES_TOKEN_AND_HANDEL_TTL:
        * deoarece in client eu nu pot sa fac rost de datele userului respectiv, 
        mi-am facut functia asta care sa ma ajute (imi intoarce acc tokeul si ttl-ul)
    * INCREASE_COUNT:
        * ma ajuta la incrementarea pozitiei din vectorul de liste de drepturi, dar 
        am ajuns sa nu o mai folosesc

# client
- in main parsez documentul clients.in primit in argument, iau linie cu linie
avand grija sa elimin '\n' apoi iau fiecare token cu strtok si trimit la functia
*oauth_1*
- in functia asta gestionez actiunile facute\cerute de clienti si afisez rezultatele
lor
- am impartit aceste operatii in 2 tipuri:
    * requesturi (cu sau fara refresh):
        * cerem de la server auth token
        * trimitem tokenul la semnat, apoi validam tokenul primit, dand revert 
        la operatia facuta la semnare (daca tokenul rezultat e identic cu cel initial
        semnarea a fost facuta cu succes)
        * cerem de la server tokenurile de acces si refresh
    * operatii (read, execute, ...):
        * cerem de la server tokenul de acces al clientului pe care il avem
        * cu acest token trimitem la server operatia pe care vrea clientul sa o faca
        * verificam ce response am primit si afisam mesajul corespunzator

# server_functions.c
- chit ca numele sugereaza ca este doar pentru server, vor fi functii din el 
folosite si in client
- aici sunt functii ajutatoare (functionalitatile sunt scrise in comentarii)
- aici facem si gestionarea listelor de useri si drepturi (init list. init noe, ...)

# server_functions.c 
- am definit functiile din .c
- am definit structurile de care ne vom folosi (cea de useri si de drepturi)
- avem si cateva variabile globale (functionalitatea lor e descrisa in comentarii)

- lista de useri contine structuri cu:
    * id, token acces, token refresh, ttl, daca are sau nu refresh
    * si approval (va determina ce lista de drepturi are din vectorul de liste de
    drepturi, cand este pe -1 inseamna ca inca nu i s-a dat niciun drept)

- lista de drepturi contine structuri cu:
    * numele resursei si operatiile