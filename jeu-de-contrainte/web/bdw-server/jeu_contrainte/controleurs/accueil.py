from controleurs.includes import add_activity
from model.model_pg import get_players_list

add_activity(SESSION['HISTORIQUE'], "Accueil")

# Récupération des joueurs
REQUEST_VARS["joueurs"] = get_players_list(SESSION['CONNEXION'])