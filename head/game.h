#ifndef GAME_H
#define GAME_H

#include "head/car.h"
#include "head/interface.h"
#include "head/obstacle.h"
#include "head/perks.h"
#include "head/events.h"
#include "head/savemanager.h"

#include "qjsonobject.h"
#include <vector>
#include <cassert>
#include <QKeySequence>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>


/**
 * @struct RoadLine
 * @brief Représente une ligne de la route.
 */
struct RoadLine {
    float x;     ///< Position X de la ligne.
    float y;     ///< Position Y de la ligne.
    int width;   ///< Largeur de la ligne.
    int height;  ///< Hauteur de la ligne.
};

/**
 * @class Game
 * @brief Classe principale du jeu, gérant la logique et les éléments du jeu.
 */
class Game {
public:
    /**
     * @enum Difficulty
     * @brief Enumération des niveaux de difficulté.
     */
    enum Difficulty { EASY, MEDIUM, HARD };

    Interface i;                ///< Interface du jeu.
    Car c;                      ///< Voiture du jeu.
    SaveManager *saveManager;   ///< Gestion des sauvegardes.
    Event *event_;              ///< Gestion des événements.

    int leftKey = Qt::Key_Left;  ///< Touche pour aller à gauche.
    int rightKey = Qt::Key_Right; ///< Touche pour aller à droite.

    std::vector<Obstacle> tab_obstacle; ///< Liste des obstacles.
    std::vector<Perks> tab_perks;       ///< Liste des power-ups.

    /**
     * @brief Définit la difficulté du jeu.
     * @param diff La difficulté à définir.
     */
    void setDifficulty(Difficulty diff);

    /**
     * @brief Retourne la difficulté actuelle.
     * @return La difficulté actuelle.
     */
    Difficulty currentDifficulty() const;

    /**
     * @brief Retourne la difficulté débloquée.
     * @return La difficulté débloquée.
     */
    Difficulty unlockedDifficulty() const;

    /**
     * @brief Convertit une difficulté en chaîne de caractères.
     * @param diff La difficulté à convertir.
     * @return Une chaîne représentant la difficulté.
     */
    static QString difficultyToString(Difficulty diff);

    /**
     * @brief Constructeur de la classe Game.
     * @param x Largeur de l'écran.
     * @param y Hauteur de l'écran.
     */
    Game(short int x, short int y);

    /**
     * @brief Destructeur de la classe Game.
     */
    ~Game();

    /**
     * @brief Opérateur d'affectation pour la classe Game.
     * @param other L'objet Game à copier.
     * @return Une référence à l'objet Game actuel.
     */
    Game& operator=(const Game& other);

    /**
     * @brief Effectue un test de régression unitaire sur la classe Game.
     */
    static void testRegression();

    /**
     * @brief Gère l'action d'une touche pressée.
     * @param t La touche pressée.
     */
    void actionTouche(unsigned char t);

    /**
     * @brief Retourne la vitesse de la route.
     * @return La vitesse de la route.
     */
    float vRoute() const;

    /**
     * @brief Retourne l'accélération de la route.
     * @return L'accélération de la route.
     */
    float aRoute() const;

    /**
     * @brief Retourne la position de la voiture.
     * @return La position de la voiture.
     */
    int dVoiture() const;

    /**
     * @brief Ajoute une valeur à l'accélération de la route.
     * @param val La valeur à ajouter.
     */
    void addaRoute(float val);

    /**
     * @brief Ajoute une valeur à la vitesse de la route.
     * @param val La valeur à ajouter.
     */
    void addvRoute(float val);

    /**
     * @brief Ajoute une valeur à la position de la voiture.
     * @param val La valeur à ajouter.
     */
    void adddVoiture(int val);

    /**
     * @brief Définit l'accélération de la route.
     * @param val La nouvelle valeur de l'accélération.
     */
    void setaRoute(float val);

    /**
     * @brief Définit la vitesse de la route.
     * @param val La nouvelle valeur de la vitesse.
     */
    void setvRoute(float val);

    /**
     * @brief Définit la position de la voiture.
     * @param val La nouvelle position de la voiture.
     */
    void setdVoiture(int val);

    /**
     * @brief Met à jour l'environnement du jeu (route, obstacles, etc.).
     */
    void environment();

    /**
     * @brief Déplace la voiture.
     * @param val La valeur de déplacement.
     */
    void move(short int val);

    /**
     * @brief Vérifie les collisions entre la voiture, les obstacles et les perks.
     * @return True en cas de collision, false sinon.
     */
    bool checkCollision();

    /**
     * @brief Définit le volume des effets sonores.
     * @param volume Le volume (entre 0.0 et 1.0).
     */
    void setEffectsVolume(float volume);

    /**
     * @brief Génère les lignes de la route.
     */
    void generateRoadLines();

    /**
     * @brief Retourne les lignes de la route.
     * @return Un vecteur contenant les lignes de la route.
     */
    std::vector<RoadLine>& roadLines();

    /**
     * @brief Retourne le score actuel.
     * @return Le score actuel.
     */
    unsigned int score() const;

    /**
     * @brief Ajoute une valeur au score.
     * @param val La valeur à ajouter.
     */
    void addScore(unsigned int val);

    /**
     * @brief Définit le score.
     * @param val La nouvelle valeur du score.
     */
    void setScore(unsigned int val);

    /**
     * @brief Accesseur de la bordure gauche.
     * @return L'abscisse de la bordure gauche.
     */
    short int leftLimWin() const;

    /**
     * @brief Accesseur de la bordure droite.
     * @return L'abscisse de la bordure droite.
     */
    short int rightLimWin() const;

    /**
     * @brief Retourne la largeur du terrain.
     * @return La largeur du terrain.
     */
    short int terrainWidth() const;

    /**
     * @brief Retourne la hauteur du terrain.
     * @return La hauteur du terrain.
     */
    short int terrainHeight() const;

    /**
     * @brief Retourne la largeur de la console.
     * @return La largeur de la console.
     */
    int consoleWidth() const;

    /**
     * @brief Retourne la hauteur de la console.
     * @return La hauteur de la console.
     */
    int consoleHeight() const;

    /**
     * @brief Retourne la largeur de l'écran.
     * @return La largeur de l'écran.
     */
    unsigned int screenX() const;

    /**
     * @brief Retourne la hauteur de l'écran.
     * @return La hauteur de l'écran.
     */
    unsigned int screenY() const;

    /**
     * @brief Met à jour le meilleur score.
     */
    void autoBest();

    /**
     * @brief Définit le meilleur score.
     * @param bs Le nouveau meilleur score.
     */
    void setBestScore(unsigned int bs);

    /**
     * @brief Convertit l'objet Game en un QJsonObject.
     * @return Le QJsonObject représentant l'objet Game.
     */
    QJsonObject toJson() const;

    /**
     * @brief Remplit l'objet Game à partir d'un QJsonObject.
     * @param data Le QJsonObject contenant les données du jeu.
     */
    void fromJson(const QJsonObject& data);

    /**
     * @brief Retourne le meilleur score.
     * @return Le meilleur score.
     */
    unsigned int bestScore() const;

    /**
     * @brief Sauvegarde l'état du jeu.
     * @param slot Le slot de sauvegarde (0 pour autosave).
     */
    void saveGame(int slot = 0);

    /**
     * @brief Charge l'état du jeu.
     * @param slot Le slot de sauvegarde (0 pour autosave).
     */
    void loadGame(int slot = 0);

    /**
     * @brief Contrôle la lecture de la musique.
     */
    void toggleMusic();

    QMediaPlayer *mediaPlayer;   ///< Lecteur multimédia pour la musique.
    QAudioOutput *audioOutput;  ///< Sortie audio pour la musique.
    bool musicPlaying = false;  ///< Indique si la musique est en cours de lecture.
    QString musicPath = "data/songs/Breakdown.mp3"; ///< Chemin du fichier musical.

    QMediaPlayer *sfxPlayer;     ///< Lecteur multimédia pour les SFX.
    QAudioOutput *sfxAudioOutput; ///< Sortie audio pour les SFX.

private:
    unsigned int score_ = 0;     ///< Score actuel du joueur.
    unsigned int bestScore_ = 0; ///< Meilleur score enregistré.

    float vRoute_ = 1;    ///< Vitesse de la route.
    int dVoiture_ = 0;    ///< Position de la voiture.
    float aRoute_ = 4;    ///< Accélération de la route.
    bool voidCollisions = false; ///< Pour ignorer les obstacles.

    short int leftLimWin_;  ///< Abscisse de la bordure gauche de la fenêtre graphique.
    short int rightLimWin_; ///< Abscisse de la bordure droite de la fenêtre graphique.

    int consoleWidth_ = 30;  ///< Largeur fixe pour la console.
    int consoleHeight_ = 15; ///< Hauteur fixe pour la console.

    short int terrainWidth_ = 100;  ///< Largeur du terrain.
    short int terrainHeight_ = 100; ///< Hauteur du terrain.

    short int screenX_; ///< Largeur de l'écran.
    short int screenY_; ///< Hauteur de l'écran.

    float multObstacles_;
    float multPerks_;

    std::vector<RoadLine> roadLines_; ///< Conteneur des lignes de la route.

    Difficulty currentDifficulty_ = EASY; ///< Difficulté actuelle.
    Difficulty unlockedDifficulty_ = EASY; ///< Difficulté débloquée.
};

#endif // GAME_H
