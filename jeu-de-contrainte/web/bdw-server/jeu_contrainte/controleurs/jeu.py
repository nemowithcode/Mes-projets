from controleurs.includes import add_activity
from model.model_pg import get_id_grille, get_tuile  # Assurez-vous que ces fonctions existent

add_activity(SESSION['HISTORIQUE'], "Jeu")

if GET:
    # Récupération avec valeurs par défaut sécurisées
    taille = int(GET.get('taille', ['4'])[0])
    niveau = GET.get('niveau', ['facile'])[0]
    
    # Validation
    taille = max(4, min(12, taille))
    if niveau not in ['facile', 'moyen', 'difficile']:
        niveau = 'facile'
    
    try:
        REQUEST_VARS["taille_grille"] = taille
        REQUEST_VARS["niveau"] = niveau
        REQUEST_VARS["current_id"] = get_id_grille(SESSION['CONNEXION'], taille)[0]["id_grille"]
        REQUEST_VARS["tuile"] = get_tuile(SESSION['CONNEXION'], REQUEST_VARS["current_id"])
    except Exception as e:
        print(f"Erreur chargement grille: {str(e)}")
        REQUEST_VARS["taille_grille"] = 4
        REQUEST_VARS["current_id"] = 1  # ID par défaut
        REQUEST_VARS["tuile"] = []