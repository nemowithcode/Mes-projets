import random

alphabet = ['a', 'b', 'c', 'd',
            'e', 'f', 'g', 'h',
            'i', 'j', 'k', 'l',
            'm', 'n', 'o', 'p',
            'q', 'r', 's', 't',
            'u', 'v', 'w', 'x',
            'y', 'z',]

voyelle = ["a", "e", "i", "o", "u"]

def mot_aleatoire(long) :
    bazard = []
    for i in range(long) :
        indice_lettre = random.randint(0, 25)
        if len(bazard) == 0 :
            bazard.append(alphabet[indice_lettre])
        else :
            if bazard[i-1] == "q" :
                bazard.append("u")
            else :
                choix = random.randint(0,2)
                if bazard[i-1] == "a" and choix == 2:
                    bazard.append("u")
                else :
                    if (alphabet[indice_lettre] in voyelle and not(bazard[i-1] in voyelle)) or (not(alphabet[indice_lettre] in voyelle) and bazard[i-1] in voyelle) :
                        bazard.append(alphabet[indice_lettre])
                    elif alphabet[indice_lettre] in voyelle :
                        a = "a"
                        while a in voyelle :
                            a = alphabet[random.randint(0, 25)]
                        bazard.append(a)
                    else :
                        a = "b"
                        while not(a in voyelle) :
                            a = alphabet[random.randint(0, 25)]
                        bazard.append(a)
    return convertir(bazard)

def convertir(lis) :
    """Converti une liste en chaine de caractère"""
    listenbourg = lis[0]
    for nombre in range(len(lis)-1) :
        listenbourg += lis[nombre+1]
    return listenbourg






for x in range(1000) :
    longueur = random.randint(4, 12)
    a = mot_aleatoire(longueur)
    print(a)


