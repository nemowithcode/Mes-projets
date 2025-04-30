# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------#

#      Projet  Pendu

#----------------------------------------------------------------------------#


# -------------- Importation des modules nécessaires -------------------------#

import random

# --------------------- Définition des fonctions -----------------------------#


def choisit_mot() :
    """Choisit et retourne un mot de manière aléatoire dans le dictionnaire """
    global Dictionnaire
    long = len(Dictionnaire)
    mot = Dictionnaire[random.randint(0, long)]
    return mot

def position(compare, comparer):
    """ Retourne sous la forme d'une liste le nombre de fois
        que la lettre apparait et où elle apparait"""
    nombre_position = 0
    position_lettre = []
    for k in range(longueur) :
        if compare == comparer[k] :
            nombre_position +=  1
            position_lettre.append(k)
    return [nombre_position, position_lettre]

def met_a_jour(emplacement, aff) :
    """ Met a jour aff """
    for x in range(emplacement[0]) :
        aff[emplacement[1][x]] = lettre + "   "
    return aff

def chaine_propre(af) :
    """ convertie une liste en chaine de cararctère propre """
    mt = ""
    for x in af :
        mt = mt + x[0]
    return mt

def chaine(brut) :
    """ Convertir la liste brut en chaine de caractère """
    net = ""
    for s in range(len(brut)) :
        net = net + brut[s]
    return net

def vrai_faux(liste) :
    retour = ""
    for t in liste :
        retour = retour + t + ",   "
    return retour

# --------------------- Déclaration des variables globales -------------------#

# Dictionnaire de mot devinable
Dictionnaire = ["TAUPE", "ABRICOT", "RENARD", "TRACTEUR", "ASTICOT", "POUBELLE", "AMOUR", "VAUTOUR",
                "FIOLE", "SABRE", "ENTENDRE", "GUIGNOL", "CIEL", "MIEL", "HACHE", "CROCODILE",
                "NOURISSON", "NOUGAT", "ARNAQUE", "PROJET", "INFORMATIQUE", "INFORMATION", "CONTROLE", "BARON",
                "ROTULE", "POUMON", "PLANETE", "PLANETE", "PENDRE", "EAU", "OURS", "CORAIL","BOUTEILLE", "COULEUR",
                "SERPENT", "VERRE", "VERT", "QUESTION", "POLICE", "PEAU", "FAIM", "FIN", "OEIL", "COEUR", "MOURIR",
                "NAVIRE", "CHAVIRE", "RIVIERE", "GOUROU", "GIRAFE", "ARMURE", "ETOILE", "ARC", "CALCAIRE", "COTON",
                "VERDURE", "ORDURE", "AMICALE", "REUSSITE", "HONNEUR", "HELICOPTERE", "AMBULANCE", "FORTERESSE", "MECANICIEN",]

# Mot que rentre l'utilisateur
mot_en_cours = ""


# le mot a deviner générer par le bot
mot_a_deviner = choisit_mot()

# longueur du mot a deviner
longueur = len(mot_a_deviner)

# Texte affiché de ce qu'on a trouvé et ce qui nous manque sour forme de liste
affichage = ["_   "]*longueur


# Mot a deviner sous forme de liste
mot_a_comparer = []
for i in range(longueur) : mot_a_comparer.append(mot_a_deviner[i])

# compteur de lettre fausse
compteur = 0

# Liste de lettres déjà utilisées et fausses
faux = []

# liste de lettres déjà utilisées et vraies
vraies = []

# élément de le pendaison

e1 = "_"
e2 = "|"
e3 = "/"
e4 = "o"
e5 =  "\\"

# ----------------- Boucle de Jeu ou programme principal ---------------------#

if __name__ == '__main__':

    print("----------------------------------------------------DEBUT DU JEU----------------------------------------------------")
    print(chaine(affichage))
    while mot_a_deviner != mot_en_cours and compteur < 9:

        lettre = input("Rentrez une lettre en majuscule : ")
        print("")
        print("")


        if not (lettre in mot_a_deviner) :
            compteur += 1
        else :
            rien_faire = 0
        print("PENDAISON :")
        print("")

        if compteur == 1 :
            print("   ")
            print("  ")
            print("  ")
            print("  ")
            print("  ")
            print(e1*6)
        elif compteur == 2 :
            print("  ", e2)
            print("  ", e2)
            print("  ", e2)
            print("  ", e2)
            print(e1*6)

        elif compteur == 3 :
            print("   ", e1*7)
            print("  ", e2)
            print("  ", e2)
            print("  ", e2)
            print("  ", e2)
            print(e1*6)


        elif compteur == 4 :
            print("   ", e1*7)
            print("  ", e2, e3)
            print("  ", e2)
            print("  ", e2)
            print("  ", e2)
            print(e1*6)

        elif compteur == 5 :
            print("   ", e1*7)
            print("  ", e2, e3,"  ", e2)
            print("  ", e2)
            print("  ", e2)
            print("  ", e2)
            print(e1*6)


        elif compteur == 6 :
            print("   ", e1*7)
            print("  ", e2, e3,"  ", e2)
            print("  ", e2, "    ", e4)
            print("  ", e2)
            print("  ", e2)
            print(e1*6)


        elif compteur == 7 :
            print("   ", e1*7)
            print("  ", e2, e3,"  ", e2)
            print("  ", e2, "    ", e4)
            print("  ", e2, "    ", e2)
            print("  ", e2)
            print(e1*6)



        elif compteur == 8:
            print("   ", e1*7)
            print("  ", e2, e3,"  ", e2)
            print("  ", e2, "  ", e5, e4, e3)
            print("  ", e2, "    ", e2)
            print("  ", e2)
            print(e1*6)
        print("")
        print("")
        print("")
        print("")
        print("")


        print("")
        print("")



        if lettre in mot_a_deviner :
            pos = position(lettre, mot_a_comparer)
            affichage = met_a_jour(pos, affichage)
            mot_en_cours = chaine_propre(affichage)
            if not(lettre in vraies) :
                vraies.append(lettre)
        else :
            if not(lettre in faux) :
                faux.append(lettre)
        print(chaine(affichage))
        print("")
        print("")
        print("Liste de lettres déjà utilisées et qui marches : ", vrai_faux(vraies))
        print("Liste de lettres déjà utilisées mais qui ne marche pas : ", vrai_faux(faux))
        print("")
        print("")

    if mot_en_cours == mot_a_deviner :
        print("BRAVO !!! VOUS AVEZ TROUVER LE BON MOT !")
    else :

        print("   ", e1*7)
        print("  ", e2, e3,"  ", e2)
        print("  ", e2, "  ", e5, e4, e3)
        print("  ", e2, "    ", e2)
        print("  ", e2, "   ", e3, e5)
        print(e1*6)


        print("")
        print("")
        print("")

        print("VOUS AVEZ PERDUE !!! Le mot était : ", mot_a_deviner)







    print("----------------------------------------------------FIN DU JEU----------------------------------------------------")




