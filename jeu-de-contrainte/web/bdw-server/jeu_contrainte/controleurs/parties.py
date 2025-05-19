from controleurs.includes import add_activity

add_activity(SESSION['HISTORIQUE'], "Jeu")

if GET and 'taille' in GET:
    REQUEST_VARS['taille'] = 5
    print(REQUEST_VARS['taille'])



