#ifndef INTERFACE_H
#define INTERFACE_H

#include <QDebug>
#include <cassert>

/**
 * @class Interface
 * @brief Classe gérant les différents états de l'interface utilisateur du jeu.
 */
class Interface {
private:
    bool isOnMain_;           ///< Indique si l'interface est sur le menu principal.
    bool isOnSettings_;       ///< Indique si l'interface est sur le menu des paramètres.
    bool isOnCustom_;         ///< Indique si l'interface est sur le menu de personnalisation.
    bool isOnPause_;          ///< Indique si l'interface est sur le menu de pause.
    bool isOnModes_;          ///< Indique si l'interface est sur le menu des modes de jeu.
    bool isPlaying_;          ///< Indique si le jeu est en cours.
    bool hasLost_;            ///< Indique si le joueur a perdu.
    bool isPauseMenuShown_;   ///< Indique si le menu de pause est affiché.

public:
    /**
     * @brief Constructeur de la classe Interface.
     */
    Interface();

    /**
     * @brief Vérifie si l'interface est sur le menu principal.
     * @return True si l'interface est sur le menu principal, false sinon.
     */
    bool isOnMain() const;

    /**
     * @brief Vérifie si l'interface est sur le menu des paramètres.
     * @return True si l'interface est sur le menu des paramètres, false sinon.
     */
    bool isOnSettings() const;

    /**
     * @brief Vérifie si l'interface est sur le menu de personnalisation.
     * @return True si l'interface est sur le menu de personnalisation, false sinon.
     */
    bool isOnCustom() const;

    /**
     * @brief Vérifie si l'interface est sur le menu de pause.
     * @return True si l'interface est sur le menu de pause, false sinon.
     */
    bool isOnPause() const;

    /**
     * @brief Vérifie si l'interface est sur le menu des modes de jeu.
     * @return True si l'interface est sur le menu des modes de jeu, false sinon.
     */
    bool isOnModes() const;

    /**
     * @brief Vérifie si le jeu est en cours.
     * @return True si le jeu est en cours, false sinon.
     */
    bool isPlaying() const;

    /**
     * @brief Vérifie si le joueur a perdu.
     * @return True si le joueur a perdu, false sinon.
     */
    bool hasLost() const;

    /**
     * @brief Vérifie si le menu de pause est affiché.
     * @return True si le menu de pause est affiché, false sinon.
     */
    bool isPauseMenuShown() const;

    /**
     * @brief Affiche le menu principal.
     */
    void showMain();

    /**
     * @brief Affiche le menu des paramètres.
     */
    void showSettings();

    /**
     * @brief Affiche le menu de personnalisation.
     */
    void showCustoms();

    /**
     * @brief Affiche le menu de pause.
     */
    void showPause();

    /**
     * @brief Affiche le menu des modes de jeu.
     */
    void showModes();

    /**
     * @brief Affiche l'écran de jeu.
     */
    void showGame();

    /**
     * @brief Affiche l'écran de défaite.
     */
    void showLoss();

    /**
     * @brief Affiche le menu de pause.
     */
    void showPauseMenu();

    /**
     * @brief Masque le menu de pause.
     */
    void hidePauseMenu();

    /**
     * @brief Masque tous les menus.
     */
    void hideAll();

    /**
     * @brief Effectue un test de régression unitaire sur la classe Interface.
     */
    static void testRegression();
};

#endif // INTERFACE_H
