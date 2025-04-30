# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------#

#      Projet Generateur_mot_de_passe

#----------------------------------------------------------------------------#


# -------------- Importation des modules nécessaires -------------------------#

import random

# --------------------- Déclaration des variables globales -------------------#

lettre_p = [ "a", "b", "c", "d",
             "e", "f", "g", "h",
             "i", "j", "k", "l",
             "m", "n", "o", "p",
             "q", "r", "s", "t",
             "u", "v", "w", "x",
             "y", "z" ]

lettre_m = [ "A", "B", "C", "D",
             "E", "F", "G", "H",
             "I", "J", "K", "L",
             "M", "N", "O", "P",
             "Q", "R", "S", "T",
             "U", "V", "W", "X",
             "Y", "Z" ]

symbole = [ "&", "'", "@", "!", "?", ";", ".", ":", "/",
            "(", ")", "[", "]", "+", "°", "*", "<", ">",
            "£", "¤", "#", "%", "§", "$", "{", "}", "-"]

chiffre = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]


# --------------------- Définition des fonctions -----------------------------#

def prend_p() :
    indice = random.randint(0, len(lettre_p)-1)
    return lettre_p[indice]

def prend_m() :
    indice = random.randint(0, len(lettre_m)-1)
    return lettre_m[indice]


def prend_sym() :
    indice = random.randint(0, len(symbole)-1)
    return symbole[indice]

def prend_chi() :
    indice = random.randint(0, len(chiffre)-1)
    return chiffre[indice]

def genere(nb):
    """ Genere un mot de passe aléatoire avec un nombre de caractère précis définit par nb """
    mdp_brut = []

    for i in range(nb) :
        alea = random.randint(0, 3)
        if alea == 0 :
            mdp_brut.append(prend_p())

        elif alea == 1 :
            mdp_brut.append(prend_m())

        elif alea == 2 :
            mdp_brut.append(prend_sym())

        else :
            mdp_brut.append(prend_chi())

    mdp = ""
    for element in mdp_brut :
        mdp = mdp + element
    return mdp

def trouver_mdp(mdp, s, c) :
    compteur_s = 0
    compteur_c = 0
    for element in mdp :
        if element in symbole :
            compteur_s += 1
        elif element in chiffre :
            compteur_c += 1
    if compteur_s == s and compteur_c == c :
        return True
    else :
        return False




# ----------------- Boucle de Jeu ou programme principal ---------------------#


if __name__ == '__main__' :

    long = int(input("Entrez le nombre de caractères que contiendra votre mot de passe : "))
    s = int(input("Combien de symboles voulez-vous : "))
    c = int(input("Combien de chiffre voulez-vous : "))
    while s+c > long :
        print("Vous êtes incroyable stupide")
        long = int(input("Entrez le nombre de caractères que contiendra votre mot de passe : "))
        s = int(input("Combien de symboles voulez-vous : "))
        c = int(input("Combien de chiffre voulez-vous : "))
    mdp = genere(long)
    while trouver_mdp(mdp, s, c) == False :
        mdp = genere(long)
    print(mdp)



