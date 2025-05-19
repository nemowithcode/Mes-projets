#ifndef PERKS_H
#define PERKS_H

#include "head/hitbox.h"
#include "head/position.h"

#include <vector>
#include <cstdlib>
#include <QDebug>
#include <cassert>

/**
 * @enum PerkType
 * @brief Enumération des types de perks disponibles dans le jeu.
 */
enum class PerkType {
    SLOW = 1,           ///< Ralentit la vitesse.
    INVINCIBILITY = 2,  ///< Rend le joueur invincible.
    TURBO = 3           ///< Augmente la vitesse.
};

class Obstacle;

/**
 * @class Perks
 * @brief Classe représentant un perk dans le jeu.
 */
class Perks {
public:
    /**
     * @brief Constructeur de la classe Perks avec une hitbox et un type de perk.
     * @param hitbox Hitbox associée au perk.
     * @param perk Type du perk.
     */
    Perks(Hitbox hitbox, PerkType perk);

    /**
     * @brief Constructeur par défaut de la classe Perks.
     */
    Perks();

    /**
     * @brief Destructeur de la classe Perks.
     */
    ~Perks();

    /**
     * @brief Obtient le type du perk.
     * @return Le type du perk.
     */
    unsigned int type() const;

    /**
     * @brief Définit le type du perk.
     * @param perk Le nouveau type du perk.
     */
    void setType(PerkType perk);

    /**
     * @brief Hitbox associée au perk.
     */
    Hitbox h;

    /**
     * @brief Obtient la position du perk.
     * @return La position du perk.
     */
    Position pos() const;

    /**
     * @brief Définit la position du perk.
     * @param p La nouvelle position du perk.
     */
    void setPos(Position p);

    /**
     * @brief Définit la longueur de la hitbox du perk.
     * @param la Nouvelle longueur.
     */
    void setL(unsigned int la);

    /**
     * @brief Définit la largeur de la hitbox du perk.
     * @param wa Nouvelle largeur.
     */
    void setW(unsigned int wa);

    /**
     * @brief Obtient la longueur de la hitbox du perk.
     * @return La longueur de la hitbox.
     */
    unsigned int getL() const;

    /**
     * @brief Obtient la largeur de la hitbox du perk.
     * @return La largeur de la hitbox.
     */
    unsigned int getW() const;

    /**
     * @brief Met à jour les perks en fonction de la vitesse et de la position.
     * @param tab_perks Vecteur contenant les perks.
     * @param aRoute_ Accélération de la route.
     * @param multiplier Multiplicateur de vitesse.
     * @param terrainHeight_ Hauteur du terrain.
     * @param screenY_ Hauteur de l'écran.
     */
    static void updatePerks(std::vector<Perks> &tab_perks, float aRoute_, float multiplier, float terrainHeight_, float screenY_);

    /**
     * @brief Génère un nouveau perk en fonction des obstacles.
     * @param tab_obstacle Vecteur contenant les obstacles.
     * @param tab_perks Vecteur contenant les perks.
     */
    static void generePerk(std::vector<Obstacle> &tab_obstacle, std::vector<Perks> &tab_perks, float multPerks);

    /**
     * @brief Tente de générer un perk.
     * @return True si un perk est généré, false sinon.
     */
    static bool tryPerk(float multPerks);

    /**
     * @brief Effectue un test de régression unitaire sur la classe Perks.
     */
    static void testRegression();

private:
    /**
     * @brief Type du perk.
     */
    PerkType type_;
};

#endif // PERKS_H
