#ifndef POSITION_H
#define POSITION_H

#include <QDebug>
#include <cassert>

/**
 * @class Position
 * @brief Classe représentant une position en 2D avec des coordonnées X et Y.
 */
class Position {
public:
    /**
     * @brief Constructeur de la classe Position avec des coordonnées X et Y.
     * @param x Coordonnée X.
     * @param y Coordonnée Y.
     */
    Position(float x, float y);

    /**
     * @brief Constructeur de copie de la classe Position.
     * @param other Objet Position à copier.
     */
    Position(const Position& other);

    /**
     * @brief Constructeur par défaut de la classe Position.
     */
    Position();

    /**
     * @brief Destructeur de la classe Position.
     */
    ~Position();

    /**
     * @brief Opérateur d'affectation.
     * @param other Objet Position à copier.
     * @return Référence vers l'objet Position assigné.
     */
    Position& operator=(const Position& other);

    /**
     * @brief Obtient la coordonnée X.
     * @return La coordonnée X.
     */
    float x() const;

    /**
     * @brief Obtient la coordonnée Y.
     * @return La coordonnée Y.
     */
    float y() const;

    /**
     * @brief Définit la coordonnée X.
     * @param x Nouvelle coordonnée X.
     */
    void setX(float x);

    /**
     * @brief Définit la coordonnée Y.
     * @param y Nouvelle coordonnée Y.
     */
    void setY(float y);

    /**
     * @brief Définit les coordonnées X et Y.
     * @param x Nouvelle coordonnée X.
     * @param y Nouvelle coordonnée Y.
     */
    void setPos(float x, float y);

    /**
     * @brief Définit la position à partir d'un autre objet Position.
     * @param p Objet Position contenant les nouvelles coordonnées.
     */
    void setPos(Position p);

    /**
     * @brief Effectue un test de régression unitaire sur la classe Position.
     */
    static void testRegression();

private:
    float x_; ///< Coordonnée X.
    float y_; ///< Coordonnée Y.
};

#endif // POSITION_H
