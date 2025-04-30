# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------#

#      Projet

#----------------------------------------------------------------------------#


# -------------- Importation des modules nécessaires -------------------------#


# --------------------- Déclaration des variables globales -------------------#

alphabet = [ "A", "B", "C", "D",
             "E", "F", "G", "H",
             "I", "J", "K", "L",
             "M", "N", "O", "P",
             "Q", "R", "S", "T",
             "U", "V", "W", "X",
             "Y", "Z" ]


substitution = {}



# --------------------- Définition des fonctions -----------------------------#


def trouve_cle(valeur, dico) :
    """ Trouve la clé du dictionnaire à partir de la valeur """
    liste = dico.keys()
    for cle in liste :
        if dico[cle] == valeur :
            return cle

def chiffrement(message, substitution) :
    """ Chiffre avec la substitution """
    nouveau_message = ""
    for lettre in message :
        if lettre in alphabet :
            nouveau_message = nouveau_message + substitution[lettre]
        else :
            nouveau_message = nouveau_message + " "

    return nouveau_message

def dechiffrement(message, substitution) :
    """ Déchiffre avec la substitution """
    nouveau_message = ""
    for lettre in message :
        if lettre in alphabet :
            nouveau_message = nouveau_message + trouve_cle(lettre, substitution)
        else :
            nouveau_message = nouveau_message + " "

    return nouveau_message


# ----------------- Boucle de Jeu ou programme principal ---------------------#


if __name__ == '__main__' :
    for lettre in alphabet :
        var = input("Quel lettre assignez vous à la lettre "+ str(lettre) +" : ")
        substitution[lettre] = var
    choix = int(input("Tapez 1 si vous souhaitez chiffrer, tapez 2 si vous souhaitez déchiffrer : "))
    if choix == 1 :
        message_a_chiffrer = input("Rentrez le message à chiffrer : ")
        print(chiffrement(message_a_chiffrer, substitution))
    elif choix == 2 :
        message_a_dechiffrer = input("Rentrez le message à déchiffrer : ")
        print(dechiffrement(message_a_dechiffrer, substitution))
    else :
        print("Ne rentrez pas d'autres caractères autre que 1 ou 2 ")
