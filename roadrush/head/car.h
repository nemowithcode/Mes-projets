#ifndef CAR_H
#define CAR_H

#include "head/hitbox.h"

#include <QDebug>
#include <cassert>

/**
 * @class Car
 * @brief Représente une voiture avec une position, une couleur et une capacité de mouvement.
 */
class Car
{
public:
    /**
     * @brief Hitbox associée à la voiture.
     */
    Hitbox h;

    /**
     * @enum Couleur
     * @brief Enumération des couleurs possibles pour la voiture.
     */
    enum Couleur { 
        RED,   /**< Couleur rouge. */
        BLUE,  /**< Couleur bleue. */
        WHITE, /**< Couleur blanche. */
        GREEN  /**< Couleur verte. */
    };

    /**
     * @brief Constructeur par défaut de la classe Car.
     */
    Car();

    /**
     * @brief Constructeur avec paramètres pour initialiser la position de la voiture.
     * @param x Position en X.
     * @param y Position en Y.
     */
    Car(float x, float y);

    /**
     * @brief Destructeur de la classe Car.
     */
    ~Car();

    /**
     * @brief Opérateur d'affectation.
     * @param other Objet Car à copier.
     * @return Référence vers l'objet Car assigné.
     */
    Car& operator=(const Car& other);

    /**
     * @brief Obtient la position actuelle de la voiture.
     * @return Position de la voiture.
     */
    Position getPos() const;

    /**
     * @brief Définit la position de la voiture.
     * @param xa Nouvelle position en X.
     * @param ya Nouvelle position en Y.
     */
    void setPos(float xa, float ya);

    /**
     * @brief Déplace la voiture horizontalement.
     * @param x_ Distance à déplacer en X.
     */
    void move(float x_);

    /**
     * @brief Obtient la couleur actuelle de la voiture.
     * @return Couleur de la voiture.
     */
    Couleur skin() const;

    /**
     * @brief Définit une nouvelle couleur pour la voiture.
     * @param newSkin Nouvelle couleur.
     */
    void setSkin(Couleur newSkin);

    /**
     * @brief Vérifie si la voiture peut se déplacer.
     * @return true si la voiture peut se déplacer, false sinon.
     */
    bool canMove() const;

    /**
     * @brief Rend la voiture disponible pour se déplacer.
     */
    void setFree();

    /**
     * @brief Rend la voiture indisponible pour se déplacer.
     */
    void restrain();

    /**
     * @brief Effectue un test de régression unitaire sur la classe Car.
     */
    static void testRegression();

private:
    /**
     * @brief Indique si la voiture peut se déplacer.
     */
    bool canMove_; 

    /**
     * @brief Couleur actuelle de la voiture.
     */
    Couleur skin_ = RED;
};

#endif // CAR_H
