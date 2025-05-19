#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "head/hitbox.h"
#include "head/position.h"

#include <vector>
#include <QDebug>
#include <cassert>

class Perks;

/**
 * @class Obstacle
 * @brief Classe représentant un obstacle dans le jeu.
 */
class Obstacle {
public:
    /**
     * @brief Constructeur de la classe Obstacle avec position, longueur et largeur.
     * @param p1 Position de l'obstacle.
     * @param l Longueur de l'obstacle.
     * @param w Largeur de l'obstacle.
     */
    Obstacle(Position p1, unsigned int l, unsigned int w);

    /**
     * @brief Constructeur de la classe Obstacle avec position et taille carrée.
     * @param p1 Position de l'obstacle.
     * @param s Taille (longueur et largeur égales) de l'obstacle.
     */
    Obstacle(Position p1, unsigned int s);

    /**
     * @brief Constructeur par défaut de la classe Obstacle.
     */
    Obstacle();

    /**
     * @brief Destructeur de la classe Obstacle.
     */
    ~Obstacle();

    /**
     * @brief Hitbox associée à l'obstacle.
     */
    Hitbox h;

    /**
     * @brief Définit la longueur de l'obstacle.
     * @param s Nouvelle longueur de l'obstacle.
     */
    void setL(unsigned int s);

    /**
     * @brief Définit la largeur de l'obstacle.
     * @param s Nouvelle largeur de l'obstacle.
     */
    void setW(unsigned int s);

    /**
     * @brief Obtient la longueur de l'obstacle.
     * @return La longueur de l'obstacle.
     */
    unsigned int getL() const;

    /**
     * @brief Obtient la largeur de l'obstacle.
     * @return La largeur de l'obstacle.
     */
    unsigned int getW() const;

    /**
     * @brief Définit la position de l'obstacle.
     * @param p Nouvelle position de l'obstacle.
     */
    void setPos(Position p);

    /**
     * @brief Met à jour les obstacles en fonction de la vitesse et de la position.
     * @param tab_obstacle Vecteur contenant les obstacles.
     * @param aRoute_ Accélération de la route.
     * @param multiplier Multiplicateur de vitesse.
     * @param terrainHeight_ Hauteur du terrain.
     * @param screenY_ Hauteur de l'écran.
     */
    static void updateObstacles(std::vector<Obstacle> &tab_obstacle, float aRoute_, float multiplier, float terrainHeight_, float screenY_);

    /**
     * @brief Génère de nouveaux obstacles et perks.
     * @param tab_obstacle Vecteur contenant les obstacles.
     * @param tab_perks Vecteur contenant les perks.
     */
    static void genereObstacle(std::vector<Obstacle> &tab_obstacle, std::vector<Perks> &tab_perks, float multObstacles);

    /**
     * @brief Tente de générer un obstacle.
     * @return True si un obstacle est généré, false sinon.
     */
    static bool tryObstacle(float multObstacles);

    /**
     * @brief Effectue un test de régression unitaire sur la classe Obstacle.
     */
    static void testRegression();

    /**
     * @brief Retourne le type de l'obstacle.
     */
    int type();

    /**
     * @brief Définit le type de l'obstacle.
     */
    void setType(int va);

private:

    int type_; ///< Type de l'obstacle.
};

#endif // OBSTACLE_H
