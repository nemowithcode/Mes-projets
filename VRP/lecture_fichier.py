def fichier_to_liste(nom_fichier):
    """Prend un fichier et construit une liste de ses lignes."""

    assert type(nom_fichier) == str, "L'argument doit être une chaine de caractère"

    fichier = open(nom_fichier, 'r')
    f_brut = fichier.readlines()
    f_net = []
    for ligne in f_brut:
        if (ligne[-1] == '\n'):
            f_net.append(ligne[:-1])
        else:
            f_net.append(ligne)

    return f_net

def liste_to_dictionnaire(liste):
    """Prend une liste et construit un dictionnaire pour accéder aux données."""

    assert type(liste) == list, "L'argument doit être une liste"

    dictionnaire = {}
    i = trouver_indice(liste, "NB_DEPOTS")
    if i is not None:
        dictionnaire['nb_depots'] = int(liste[i].split(':')[-1].strip())
    else:
        return None  # Retourne None si "NB_DEPOTS" n'est pas trouvé

    dictionnaire['depots'] = {}
    i = trouver_indice(liste, "DATA_DEPOTS")
    if i is not None:
        for j in range(1, int(dictionnaire['nb_depots']) + 1):
            depot_data = liste[i + j].split()
            dictionnaire['depots'][depot_data[0]] = {
                'x': float(depot_data[1]),
                'y': float(depot_data[2]),
                'readyTime': float(depot_data[3]),
                'dueTime': float(depot_data[4])
            }
    else:
        return None  # Retourne None si "DATA_DEPOTS" n'est pas trouvé

    dictionnaire['clients'] = {}
    i = trouver_indice(liste, "NB_CLIENTS")
    if i is not None:
        dictionnaire['nb_clients'] = int(liste[i].split(':')[-1].strip())
    else:
        return None  # Retourne None si "NB_CLIENTS" n'est pas trouvé

    i = trouver_indice(liste, "DATA_CLIENTS")
    if i is not None:
        for j in range(1, int(dictionnaire['nb_clients']) + 1):
            client_data = liste[i + j].split()
            client_id = client_data[0]
            dictionnaire['clients'][client_id] = {
                'x': float(client_data[1]),
                'y': float(client_data[2]),
                'readyTime': float(client_data[3]),
                'dueTime': float(client_data[4]),
                'demand': float(client_data[5]),
                'service': float(client_data[6])
            }
        dictionnaire['client_demands'] = {client_id: client['demand'] for client_id, client in dictionnaire['clients'].items()}  # Stocke les demandes des clients
    else:
        return None  # Retourne None si "DATA_CLIENTS" n'est pas trouvé

    i = trouver_indice(liste, "MAX_QUANTITY")
    if i is not None:
        dictionnaire['max_quantity'] = float(liste[i].split(':')[-1].strip())
    else:
        return None  # Retourne None si "MAX_QUANTITY" n'est pas trouvé

    return dictionnaire


def trouver_indice(liste, debut):
    """Trouve l'indice d'un élément commençant par 'debut' dans une liste."""

    assert type(liste) == list, "L'argument doit être une liste"

    for i, elem in enumerate(liste):
        if elem.startswith(debut):
            return i
    return None  # Retourne None si aucun élément ne commence par 'debut'