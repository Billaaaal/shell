
CREATE TABLE
    techno (
        numTechno integer PRIMARY KEY AUTOINCREMENT,
        nomTechno varchar
    );

CREATE TABLE
    application (
        numApplication integer PRIMARY KEY AUTOINCREMENT,
        nomApplication varchar,
        descApplication varchar,
        tarifInitApplication integer,
        tarifPageApplication integer
    );

CREATE TABLE
    developpeur (
        numDeveloppeur integer PRIMARY KEY AUTOINCREMENT,
        nomDeveloppeur varchar,
        prenomDeveloppeur varchar,
        dateEntreeDeveloppeur varchar,
        statutDeveloppeur varchar
    );

CREATE TABLE
    competenceDev (
        numCompetenceDev integer PRIMARY KEY AUTOINCREMENT,
        numDeveloppeur integer,
        numTechno integer,
        FOREIGN KEY (numDeveloppeur) REFERENCES developpeur (numDeveloppeur),
        FOREIGN KEY (numTechno) REFERENCES techno (numTechno)
    );

CREATE TABLE
    client (
        numClient integer PRIMARY KEY AUTOINCREMENT,
        nomClient varchar,
        numSiretClient integer,
        adresseClient varchar,
        mailClient varchar,
        telClient varchar
    );

CREATE TABLE
    referentClient (
        numReferentClient integer PRIMARY KEY AUTOINCREMENT,
        prenomReferentClient varchar,
        nomReferentClient varchar,
        mailReferentClient varchar,
        telReferentClient varchar,
        typeReferentClient varchar,
        numClient integer,
        FOREIGN KEY (numClient) REFERENCES client (numClient)
    );

CREATE TABLE
    projet (
        numProjet integer PRIMARY KEY AUTOINCREMENT,
        numClient integer,
        statutProjet varchar,
        deadlineProjet varchar,
        descProjet varchar,
        urlPageTrelloProjet varchar,
        FOREIGN KEY (numClient) REFERENCES client (numClient)
    );

CREATE TABLE
    facture (
        numFacture integer PRIMARY KEY AUTOINCREMENT,
        numProjet integer,
        numClient integer,
        urlIngeneoFacture varchar,
        totalHTFacture integer,
        totalTTCFacture integer,
        FOREIGN KEY (numProjet) REFERENCES projet (numProjet),
        FOREIGN KEY (numClient) REFERENCES client (numClient)
    );

CREATE TABLE
    applicationProjet (
            numProjet integer,
            numApplication integer,
            numApplicationProjet integer PRIMARY KEY AUTOINCREMENT,
            nbPagesApplicationProjet integer,
            nbDeveloppeursApplicationProjet integer,
            indiceComplexiteApplicationProjet integer,
            totalTarifApplicationProjet integer,
            FOREIGN KEY (numProjet) REFERENCES projet (numProjet),
            FOREIGN KEY (numApplication) REFERENCES application (numApplication)
        );



CREATE TABLE
    technoApplicationProjet (
        numTechnoApplicationProjet integer PRIMARY KEY AUTOINCREMENT,
        numApplicationProjet integer,
        numTechno integer,
        FOREIGN KEY (numApplicationProjet) REFERENCES applicationProjet (numApplicationProjet),
        FOREIGN KEY (numTechno) REFERENCES techno (numTechno)
    );

CREATE TABLE
    developpeurProjet (
        numDeveloppeurProjet integer PRIMARY KEY AUTOINCREMENT,
        numDeveloppeur integer,
        numProjet integer,
        roleDeveloppeur varchar,
        FOREIGN KEY (numDeveloppeur) REFERENCES developpeur (numDeveloppeur),
        FOREIGN KEY (numProjet) REFERENCES projet (numProjet)
    );

