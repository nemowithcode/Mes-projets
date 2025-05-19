DROP SCHEMA données_jeu CASCADE;
DROP SCHEMA IF EXISTS données_jeu;
CREATE SCHEMA données_jeu;
SET search_path TO données_jeu;


CREATE TABLE A_choisie_de_jouer (
  PRIMARY KEY (id_Partie, numero_tour, id_Tuile),
  id_Partie   integer NOT NULL,
  numero_tour integer NOT NULL,
  id_Tuile    integer NOT NULL
);

CREATE TABLE Classement (
  PRIMARY KEY (taille_grille,niveau_difficulte),
  taille_grille integer,
  niveau_difficulte VARCHAR(42) NOT NULL,
  nom VARCHAR(42),
  date_creation_ VARCHAR(42),
  date_mise_a_jour VARCHAR(42),
  id_Grille integer NOT NULL
);

CREATE TABLE Comporte (
  PRIMARY KEY (id_Partie, nb_tuile_decouverte, id_Tuile),
  id_Partie           integer NOT NULL,
  nb_tuile_decouverte integer NOT NULL,
  id_Tuile            integer NOT NULL
);

CREATE TABLE Contrainte (
  PRIMARY KEY (id_Contraite),
  id_Contraite  integer NOT NULL,
  type_C        VARCHAR(42) NULL,
  libelle       VARCHAR(42) NULL,
  nb_elements_1 integer NULL,
  id_element integer NULL
);

CREATE TABLE Element (
  PRIMARY KEY (id_Element),
  id_Element integer NOT NULL,
  nom        VARCHAR(42),
  symbole    VARCHAR(42)
);

CREATE TABLE est_Classe_dans (
  PRIMARY KEY (taille_grille,niveau_difficulte, id_Joueur),
  taille_grille integer ,
  niveau_difficulte VARCHAR(42) NOT NULL,
  id_Joueur         integer NOT NULL,
  classe            integer
);

CREATE TABLE est_definie_par (
  PRIMARY KEY (id_Contraite, id_Tuile),
  id_Contraite integer NOT NULL,
  id_Tuile     integer NOT NULL
);

CREATE TABLE est_former (
  PRIMARY KEY (id_Element, id_Tuile),
  id_Element integer NOT NULL,
  id_Tuile   integer NOT NULL
);

CREATE TABLE est_former_de (
  PRIMARY KEY (id_Element, id_Contraite),
  id_Element   integer NOT NULL,
  id_Contraite integer NOT NULL
);

CREATE TABLE est_initialise_par (
  PRIMARY KEY (id_Grille, id_Tuile),
  id_Grille integer NOT NULL,
  id_Tuile  integer NOT NULL
);

CREATE TABLE Grille (
  PRIMARY KEY (id_Grille),
  id_Grille         integer NOT NULL,
  taille_grille     integer,
  niveau_difficulte VARCHAR(42)
);

CREATE TABLE Joueur (
  PRIMARY KEY (id_Joueur),
  id_Joueur   integer NOT NULL,
  nom         VARCHAR(42),
  prenom      VARCHAR(42),
  annee_naiss integer,
  pseudo      VARCHAR(42)
);

CREATE TABLE Partie (
  PRIMARY KEY (id_Partie),
  id_Partie            integer NOT NULL,
  date                 VARCHAR(42),
  horaire_debut_partie VARCHAR(42),
  etat                 VARCHAR(42),
  id_Grille            integer NULL,
  id_Joueur            integer NULL
);

CREATE TABLE Pioche (
  PRIMARY KEY (id_Partie, nb_tuile_decouverte),
  id_Partie           integer NOT NULL,
  nb_tuile_decouverte integer NOT NULL
);

CREATE TABLE Tour (
  PRIMARY KEY (id_Partie, numero_tour),
  id_Partie         integer NOT NULL,
  numero_tour       integer NOT NULL,
  tuile_choisie     integer,
  emplacement_tuile integer
);

CREATE TABLE Tuile (
  PRIMARY KEY (id_Tuile),
  id_Tuile    integer NOT NULL,
  type_T      VARCHAR(42),
  fichier     VARCHAR(42),
  num_ligne   integer NULL,
  num_colonne integer NULL,
  niveau      integer NULL,
  points      integer NULL
);

ALTER TABLE A_choisie_de_jouer ADD FOREIGN KEY (id_Tuile) REFERENCES Tuile (id_Tuile);
ALTER TABLE A_choisie_de_jouer ADD FOREIGN KEY (id_Partie, numero_tour) REFERENCES Tour (id_Partie, numero_tour);

ALTER TABLE Classement ADD FOREIGN KEY (id_Grille) REFERENCES Grille (id_Grille);

ALTER TABLE Comporte ADD FOREIGN KEY (id_Tuile) REFERENCES Tuile (id_Tuile);
ALTER TABLE Comporte ADD FOREIGN KEY (id_Partie, nb_tuile_decouverte) REFERENCES Pioche (id_Partie, nb_tuile_decouverte);

ALTER TABLE est_Classe_dans ADD FOREIGN KEY (id_Joueur) REFERENCES Joueur (id_Joueur);
ALTER TABLE est_Classe_dans ADD FOREIGN KEY (taille_grille,niveau_difficulte) REFERENCES Classement (taille_grille,niveau_difficulte);

ALTER TABLE est_definie_par ADD FOREIGN KEY (id_Tuile) REFERENCES Tuile (id_Tuile);
ALTER TABLE est_definie_par ADD FOREIGN KEY (id_Contraite) REFERENCES Contrainte (id_Contraite);

ALTER TABLE est_former ADD FOREIGN KEY (id_Tuile) REFERENCES Tuile (id_Tuile);
ALTER TABLE est_former ADD FOREIGN KEY (id_Element) REFERENCES Element (id_Element);
ALTER TABLE est_former_de ADD FOREIGN KEY (id_Contraite) REFERENCES Contrainte (id_Contraite);
ALTER TABLE est_former_de ADD FOREIGN KEY (id_Element) REFERENCES Element (id_Element);

ALTER TABLE est_initialise_par ADD FOREIGN KEY (id_Tuile) REFERENCES Tuile (id_Tuile);
ALTER TABLE est_initialise_par ADD FOREIGN KEY (id_Grille) REFERENCES Grille (id_Grille);

ALTER TABLE Partie ADD FOREIGN KEY (id_Joueur) REFERENCES Joueur (id_Joueur);
ALTER TABLE Partie ADD FOREIGN KEY (id_Grille) REFERENCES Grille (id_Grille);

ALTER TABLE Pioche ADD FOREIGN KEY (id_Partie) REFERENCES Partie (id_Partie);

ALTER TABLE Tour ADD FOREIGN KEY (id_Partie) REFERENCES Partie (id_Partie);

INSERT INTO données_jeu.contrainte ( id_contraite,type_c,nb_elements_1,id_element)
SELECT distinct ce.id,apc.typec,ce.nb_elements,ce.id_element
FROM données_fournies.tuile_contrainte tc
JOIN données_fournies.a_pour_contrainte apc on apc.id_tuile=tc.id
JOIN données_fournies.contrainte_element ce on ce.id=apc.id_contrainte
order by ce.id
; 
INSERT INTO données_jeu.contrainte ( id_contraite,type_c,libelle,nb_elements_1)
SELECT cn.id, cn.typec,cn.libellé, cn.nb_elements
FROM données_fournies.tuile_contrainte tc
JOIN données_fournies.a_pour_contrainte apc on apc.id_tuile=tc.id
JOIN données_fournies.contrainte_nombre cn on cn.id=apc.id_contrainte
order by cn.id
; 




INSERT INTO données_jeu.est_definie_par ( id_contraite, id_tuile )
	SELECT c.id_contraite,t.id_tuile 
	From données_jeu.contrainte c 
	Join données_fournies.a_pour_contrainte a on a.id_contrainte=c.id_contraite
	join données_jeu.tuile t on t.id_tuile=a.id_tuile
;


INSERT INTO données_jeu.est_former ( id_element, id_tuile )
	SELECT e.id_element, t.id_tuile  
	FROM données_jeu.element e 
	JOIN données_fournies.tuile_jeu tj on tj.id_element=e.id_element
	Join données_jeu.tuile t on t.id_tuile=tj.id
;


INSERT INTO données_jeu.est_former_de ( id_element, id_contraite )
	SELECT e.id_element,c.id_contraite  
	FROM données_jeu.element e
	JOIN données_fournies.contrainte_element ce on ce.id_element=e.id_element
	Join données_jeu.contrainte c on c.id_contraite=ce.id
;



INSERT INTO données_jeu.tuile(id_tuile, type_T, fichier) 

	SELECT distinct T.id, T.typet, T.fichier FROM données_fournies.tuile T
		JOIN données_fournies.tuile_jeu TE ON TE.id=T.id
		order by T.id
;



INSERT INTO données_jeu.tuile(id_tuile, type_T, fichier, niveau, points) 
	SELECT TC.id,T.typet,T.fichier,TC.niveau,TC.points
	FROM données_fournies.tuile T
	join données_fournies.tuile_contrainte TC on TC.id=T.id
	order by T.id
;


INSERT INTO données_jeu.tuile(niveau,points) 
  	SELECT TC.niveau, TC.points
  	FROM données_fournies.tuile T
		JOIN données_fournies.tuile_contrainte TC ON TC.id=T.id
		where TC.id==101
    Group by T.id,TC.niveau, TC.points
;



INSERT INTO données_jeu.grille(id_grille, taille_grille) 

	SELECT G.id, MAX(G.num_ligne) FROM données_fournies.grille G
  group by G.id
;




INSERT INTO données_jeu.joueur(id_Joueur, nom, prenom, annee_naiss, pseudo) 

	SELECT J.idj, J.nom, J.prénom, J.année_naiss, J.pseudo FROM données_fournies.joueur J 
;


INSERT INTO données_jeu.element(id_Element, nom, symbole) 
	SELECT E.id, E.nom, E.symbole FROM données_fournies.element E

;



INSERT INTO données_jeu.est_initialise_par ( id_grille, id_tuile )
SELECT g.id_grille,t.id_tuile
From données_jeu.grille g
join données_fournies.grille ge on ge.id=g.id_grille
join données_jeu.tuile t on t.id_tuile=ge.id_tuile
;









