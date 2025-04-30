# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------#

#      Projet Chiffrement de César

#----------------------------------------------------------------------------#


# -------------- Importation des modules nécessaires -------------------------#

# (aucun module n'est nécessaire)

# --------------------- Déclaration des variables globales -------------------#

alphabet = [ "A", "B", "C", "D",
             "E", "F", "G", "H",
             "I", "J", "K", "L",
             "M", "N", "O", "P",
             "Q", "R", "S", "T",
             "U", "V", "W", "X",
             "Y", "Z" ]

# --------------------- Définition des fonctions -----------------------------#


def chaine_to_liste(chaine) :
    """ Décompose une chaine de caractère en liste """
    liste =[]
    for i in range(len(chaine)) :
        liste.append(chaine[i])
    return liste


def liste_to_chaine(liste) :
    """ Rassemble une liste en chaine de caractère """
    chaine = ""
    for element in liste :
        chaine = chaine + element
    return chaine


def c_to_n(liste) :
    """Converti une liste en suite de chiffre selon la position des lettres dans l'alphabet """
    suite_chiffre = []
    global alphabet
    for i in range(len(liste)) :
        if liste[i] in alphabet :
            for k in range(len(alphabet)) :
                if liste[i] == alphabet[k] :
                    suite_chiffre.append(k)
    return suite_chiffre


def n_to_c(liste) :
    """Converti une suite de chiffre en liste de caractère selon la position des lettres dans l'alphabet """
    liste_caractere = []
    global alphabet
    for chiffre in liste :
        liste_caractere.append(alphabet[chiffre])
    return liste_caractere


def decalage(suite, nombre) :
    """ ajoute chaque chiffre de la suite, au nombre """
    effectuer = []
    for chiffre in suite :
        effectuer.append(chiffre+nombre)
    return effectuer


def inverse_decalage(suite, nombre):
    """ soustrait chaque chiffre de la suite, au nombre """
    effectuer = []
    for chiffre in suite :
        effectuer.append(chiffre-nombre)
    return effectuer


def recadre(suite_chiffre) :
    """ Soustrait 26 jusqu'à obtenir une suite de nombre où chaque terme < 26  """
    correct = []
    for chiffre in suite_chiffre :
        while chiffre >= 26 :
            chiffre = chiffre - 26
        while chiffre < 0 :
            chiffre = chiffre + 26
        correct.append(chiffre)
    return correct


def chiffrement(text, k) :
    """Effectue le chiffrement de césar avec text qui est le message à chiffré
       et k un entier symbolisant le décalage"""
    a = chaine_to_liste(text)
    b = c_to_n(a)

    suite = decalage(b, k)
    bonne_suite = recadre(suite)

    mot_chiffre = n_to_c(bonne_suite)
    mot_final = liste_to_chaine(mot_chiffre)

    return mot_final


def dechiffrement(text, k) :
    """Effectue le déchiffrement de césar avec text qui est le message à chiffré
       et k un entier symbolisant le décalage"""
    a = chaine_to_liste(text)
    b = c_to_n(a)

    suite = inverse_decalage(b, k)
    bonne_suite = recadre(suite)

    mot_dechiffre = n_to_c(bonne_suite)
    mot_final = liste_to_chaine(mot_dechiffre)

    return mot_final



# ----------------- Boucle de Jeu ou programme principal ---------------------#

if __name__ == '__main__' :

    choix = int(input("Tapez 1 si vous voulez chiffré un message et tapez 2 si vous voulez déchiffré un message : "))
    if choix == 1 :
        message_a_chiffrer = input("Rentrez le message à chiffrer : ")
        decaler = int(input("Rentrez le décalage de votre chiffrement : "))
        print(chiffrement(message_a_chiffrer, decaler))
    elif choix == 2 :
        message_a_dechiffrer = input("Rentrez le message à déchiffrer : ")
        decaler = int(input("Rentrez le décalage de votre chiffrement : "))
        print(dechiffrement(message_a_dechiffrer, decaler))
    else :
        print("Ne rentrez pas d'autres caractères autre que 1 ou 2 ")