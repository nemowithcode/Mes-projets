# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------#

#      Projet Juste Prix

#----------------------------------------------------------------------------#


# -------------- Importation des modules nécessaires -------------------------#

import random

# --------------------- Déclaration des variables globales -------------------#


decompteur = 10

# --------------------- Définition des fonctions -----------------------------#

def prix_alea():
    """ Retourne le prix a deviner (choisie de manière aléatoire) """
    return random.randint(0, 1000)



# ----------------- Boucle de Jeu ou programme principal ---------------------#


if __name__ == '__main__' :
    prix_a_deviner = prix_alea()
    prix_en_cours = prix_a_deviner + 1

    while prix_a_deviner != prix_en_cours and decompteur != 0 :
        prix_en_cours = int(input("Quel est le bon prix ? "))
        if prix_en_cours > prix_a_deviner :
            print("C'est moins !")
        elif prix_en_cours < prix_a_deviner :
            print("C'est plus !")
        decompteur = decompteur - 1


    if prix_a_deviner == prix_en_cours :
        print("BRAVO !!! VOUS AVEZ GAGNE !")

    elif decompteur == 0 :
        print("VOUS AVEZ PERDUE ! Le bon prix était : ", prix_a_deviner)

