#ifndef HITBOX_H
#define HITBOX_H

#include "head/position.h"

#include <QDebug>
#include <cassert>

/**
 * @class Hitbox
 * @brief Représente une hitbox avec une position, une largeur et une longueur.
 */
class Hitbox
{
public:
    /**
     * @brief Position de la hitbox.
     */
    Position pos;

    /**
     * @brief Constructeur de la classe Hitbox avec position, longueur et largeur.
     * @param p1 Position de la hitbox.
     * @param l Longueur de la hitbox.
     * @param w Largeur de la hitbox.
     */
    Hitbox(Position p1, unsigned int l, unsigned int w);

    /**
     * @brief Constructeur de la classe Hitbox avec position et taille carrée.
     * @param p1 Position de la hitbox.
     * @param s Taille (longueur et largeur égales) de la hitbox.
     */
    Hitbox(Position p1, unsigned int s);

    /**
     * @brief Constructeur par défaut de la classe Hitbox.
     */
    Hitbox();

    /**
     * @brief Destructeur de la classe Hitbox.
     */
    ~Hitbox();

    /**
     * @brief Obtient la longueur de la hitbox.
     * @return La longueur de la hitbox.
     */
    unsigned int l() const;

    /**
     * @brief Obtient la largeur de la hitbox.
     * @return La largeur de la hitbox.
     */
    unsigned int w() const;

    /**
     * @brief Définit la longueur de la hitbox.
     * @param L Nouvelle longueur de la hitbox.
     */
    void setL(unsigned int L);

    /**
     * @brief Définit la largeur de la hitbox.
     * @param W Nouvelle largeur de la hitbox.
     */
    void setW(unsigned int W);

    /**
     * @brief Effectue un test de régression unitaire sur la classe Hitbox.
     */
    static void testRegression();

private:
    /**
     * @brief Longueur de la hitbox.
     */
    unsigned int l_;

    /**
     * @brief Largeur de la hitbox.
     */
    unsigned int w_;
};

#endif // HITBOX_H
