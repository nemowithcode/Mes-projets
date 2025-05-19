# models/model_pg.py
import psycopg
from psycopg import sql
from psycopg.rows import dict_row
from logzero import logger

def execute_select_query(connexion, query, params=[]):
    """
    Méthode générique pour exécuter une requête SELECT (qui peut retourner plusieurs instances).
    Utilisée par des fonctions plus spécifiques.
    """
    print("query1 ", query)
    with connexion.cursor() as cursor:
        try:
            cursor.execute(query, params)
            cursor.row_factory = dict_row
            result = cursor.fetchall()
            print("result1 ", result)
            return result 
        except psycopg.Error as e:
            logger.error(e)
    return None

def execute_other_query(connexion, query, params=[]):
    """
    Méthode générique pour exécuter une requête INSERT, UPDATE, DELETE.
    Utilisée par des fonctions plus spécifiques.
    """
    print("query2 ", query)
    with connexion.cursor() as cursor:
        try:
            cursor.execute(query, params)
            result = cursor.rowcount
            return result 
        except psycopg.Error as e:
            logger.error(e)
    return None

def get_instances(connexion, nom_table):
    """
    Retourne les instances de la table nom_table
    String nom_table : nom de la table
    """
    query = sql.SQL('SELECT * FROM {table}').format(table=sql.Identifier(nom_table), )
    return execute_select_query(connexion, query)

def count_instances(connexion, nom_table):
    """
    Retourne le nombre d'instances de la table nom_table
    String nom_table : nom de la table
    """
    query = sql.SQL('SELECT COUNT(*) AS nb FROM {table}').format(table=sql.Identifier(nom_table))
    return execute_select_query(connexion, query)



def get_id_grille(connexion, taille):
    """Retourne un ID de grille aléatoire de la taille spécifiée"""
    # Exemple d'implémentation basique - à adapter à votre BD
    cursor = connexion.cursor()
    cursor.execute("SELECT id_grille FROM grilles WHERE taille = %s ORDER BY RANDOM() LIMIT 1", (taille,))
    return [{"id_grille": cursor.fetchone()[0]}]

def get_tuile(connexion, id_grille):
    """Récupère les tuiles pour une grille donnée"""
    cursor = connexion.cursor()
    cursor.execute("SELECT * FROM tuiles WHERE id_grille = %s", (id_grille,))
    return cursor.fetchall()


def create_new_game(connexion, taille, niveau, id_Joueur):
    """Crée une nouvelle partie en base de données"""
    query = "INSERT INTO Partie (date, horaire_debut_partie, etat, id_Grille, id_Joueur) VALUES (CURRENT_DATE, CURRENT_TIME, 'en cours', (SELECT id_Grille FROM Grille WHERE taille_grille = %s AND niveau_difficulte = %s LIMIT 1), %s) RETURNING id_Partie"
    return execute_select_query(connexion, query, [taille, niveau, id_Joueur])

def get_players_list(connexion):
    """Récupère la liste des joueurs pour la sélection"""
    query = "SELECT id_Joueur, pseudo FROM données_jeu.Joueur ORDER BY pseudo ASC"
    return execute_select_query(connexion, query)
