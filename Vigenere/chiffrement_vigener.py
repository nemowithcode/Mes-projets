# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------#

#      Projet Chiffrement_vigener

#----------------------------------------------------------------------------#


# -------------- Importation des modules nécessaires -------------------------#

# (aucun module nécessaire)

# --------------------- Déclaration des variables globales -------------------#

alphabet = [ "A", "B", "C", "D",
             "E", "F", "G", "H",
             "I", "J", "K", "L",
             "M", "N", "O", "P",
             "Q", "R", "S", "T",
             "U", "V", "W", "X",
             "Y", "Z" ]

caractere_tolerable = [" ", ".", ",", ";", "!", "?", "'", "-", "_"]


numero = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]

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


def trouver_le_num(num) :
    """Recupere le numéro en caractère et le convertie en entier pour les calcules """
    a = 0
    while num != numero[a] :
        a = a + 1
    return a


def c_to_n(liste) :
    """Converti une liste en suite de chiffre selon la position des lettres dans l'alphabet """
    suite_chiffre = []
    global alphabet
    for i in range (len(liste)) :

        if liste[i] in alphabet :
            for j in range (len(alphabet)) :
                if liste[i] == alphabet[j] :
                    suite_chiffre.append([j, "L"])

        elif liste[i] in caractere_tolerable :
            suite_chiffre.append([liste[i], "C"])

        elif liste[i] in numero :
            n = trouver_le_num(liste[i])
            suite_chiffre.append([n, "N"])
        else :
            suite_chiffre.append(["*", "I"])
    return suite_chiffre


def n_to_c(convertion) :
    """Convertie une suite de nombre en chaine de caractere"""
    converti = []
    global alphabet
    for r in range(len(convertion)) :
        if convertion[r][1] == "L" :
            lettre = convertion[r][0]
            converti.append(alphabet[lettre])
        elif convertion[r][1] == "C" :
            converti.append(convertion[r][0])
        elif convertion[r][1] == "N" :
            nombre = convertion[r][0]
            converti.append(numero[nombre])
        elif convertion[r][1] == "I" :
            converti.append("*")
    return converti


def ajuster(longueur1, longueur2) :
    """Ajuste longueur2 en fonction de longueur1"""
    while len(longueur1) > len(longueur2) :
        longueur2 += longueur2

    while len(longueur2) > len(longueur1) :
        longueur2 = longueur2[0:-1]

    return longueur2


def congruence(suite1, suite2) :
    """suite1 + suite 2 est congrue à suite3 [alphabet]"""
    suite3 = []
    for k in range(len(suite1)) :
        if suite1[k][1] == "L"  :
            ajout_brut = suite1[k][0] + suite2[k][0]
            while ajout_brut >= 26 :
                ajout_brut = ajout_brut - 26
            suite3.append([ajout_brut, "L"])

        elif suite1[k][1] == "N"  :
            ajout_brut = suite1[k][0] + suite2[k][0]
            while ajout_brut >= 10 :
                ajout_brut = ajout_brut - 10
            suite3.append([ajout_brut, "N"])

        elif suite1[k][1] == "C" :
            suite3.append([suite1[k][0], "C"])
        else :
            suite3.append(["*", "I"])
    return suite3


def congruence_inverse(suite1, suite2) :
    """suite1 - suite2 congrue à suite3 [alphabet]"""
    suite3 = []
    for n in range(len(suite1)) :
        if suite1[n][1] == "L" :
            difference = suite1[n][0] - suite2[n][0]
            while difference < 0 :
                difference += 26
            suite3.append([difference, "L"])
        elif suite1[n][1] == "C" :
            suite3.append([suite1[n][0], "C"])
        elif suite1[n][1] == "N" :
            nomb = suite1[n][0] - suite2[n][0]
            while nomb < 0 :
                nomb += 10
            suite3.append([nomb, "N"])
        else :
            suite3.append(["*", "I"])
    return suite3


def chiffrer(pas_chiffrer, cle) :
    """Chiffre un message avec une clef selon la méthode de Vigenere"""
    a = chaine_to_liste(pas_chiffrer)
    b = chaine_to_liste(cle)
    ajustement = ajuster(a, b)

    x = c_to_n(a)
    y = c_to_n(ajustement)
    z = congruence(x, y)

    mot_chiffrer = n_to_c(z)
    mot_final = liste_to_chaine(mot_chiffrer)
    return mot_final


def dechiffrer(chiffrer, cle) :
    """ Déchiffre un message avec une clef selon la méthode de Vigenere"""
    a = chaine_to_liste(chiffrer)
    b = chaine_to_liste(cle)
    ajustement = ajuster(a, b)

    z = c_to_n(a)
    y = c_to_n(b)
    x = congruence_inverse(z, y)

    mot_dechiffrer = n_to_c(x)
    mot_final = liste_to_chaine(mot_dechiffrer)
    return mot_final


# ----------------- Boucle de Jeu ou programme principal ---------------------#


if __name__ == '__main__' :

    choix = int(input("Tapez 1 si vous voulez chiffrez un message, tapez 2 si vous voulez déchiffrer un message : "))

    if choix == 1 :
        message_a_chiffrer = input("Tapez votre message à chiffrer (tout en majuscule et sans espace ou caractères spéciaux comme des accents) : ")
        cle = input("Tapez la clef de chiffrement (tout en majuscule et sans espace ou caractères spéciaux comme des accents) : ")
        print("Votre message chiffré est : ", chiffrer(message_a_chiffrer, cle))

    elif choix == 2 :
        message_a_dechiffrer = input("Tapez votre message à déchiffrer (tout en majuscule et sans espace ou caractères spéciaux comme des accents) : ")
        cle = input("Tapez la clef de déchiffrement (tout en majuscule et sans espace ou caractères spéciaux comme des accents) : ")
        print("Votre message déchiffré est : ", dechiffrer(message_a_dechiffrer, cle))

    else :
        print("Ne rentrez pas d'autres caractères autre que 1 ou 2 ")