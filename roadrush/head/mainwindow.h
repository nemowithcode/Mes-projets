#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "head/game.h"
#include "qelapsedtimer.h"

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QStackedWidget>
#include <QDialog>
#include <QKeySequenceEdit>
#include <QMap>
#include <QLabel>
#include <QPainter>
#include <QFontDatabase>

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QButtonGroup>

#include <QGroupBox>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDebug>
#include <cassert>

#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Classe principale de l'application, gérant l'interface graphique et la logique du jeu.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public slots:
    /**
     * @brief Méthode appelée à chaque tick du timer.
     */
    void tick();

    /**
     * @brief Méthode appelée chaque seconde.
     */
    void seconds();

    /**
     * @brief Affiche le menu de sélection de la difficulté.
     */
    void showDifficultySelection();

    /**
     * @brief Démarre le jeu avec une difficulté donnée.
     * @param diff La difficulté sélectionnée.
     */
    void startGameWithDifficulty(Game::Difficulty diff);

    /**
     * @brief Ouvre le menu de sélection des skins.
     */
    void openSkinSelection();

    /**
     * @brief Change le skin de la voiture.
     * @param skinId L'identifiant du skin sélectionné.
     */
    void changeCarSkin(int skinId);

public:
    /**
     * @brief Constructeur de la classe MainWindow.
     * @param parent Widget parent.
     * @param x Largeur de la fenêtre.
     * @param y Hauteur de la fenêtre.
     */
    MainWindow(QWidget *parent = nullptr, unsigned int x = 0, unsigned int y = 0);

    /**
     * @brief Destructeur de la classe MainWindow.
     */
    ~MainWindow();

    /**
     * @brief Objet Game gérant la logique du jeu.
     */
    Game g;

    /**
     * @brief Ouvre le menu des paramètres.
     */
    void openSettings();

    /**
     * @brief Démarre le jeu.
     */
    void startGame();

    /**
     * @brief Réinitialise les timers.
     */
    void resetTimers();

    /**
     * @brief Capture les touches pressées par l'utilisateur.
     * @param event Événement de touche pressée.
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * @brief Capture les touches relâchées par l'utilisateur.
     * @param event Événement de touche relâchée.
     */
    void keyReleaseEvent(QKeyEvent *event);

    /**
     * @brief Affiche ou masque le menu de pause.
     */
    void togglePauseMenu();

    /**
     * @brief Met à jour le menu de pause.
     */
    void updatePauseMenu();

    /**
     * @brief Initialise le menu de pause.
     */
    void initPauseMenu();

    /**
     * @brief Crée le menu de pause.
     */
    void createPauseMenu();

    /**
     * @brief Crée un bouton pour sélectionner une difficulté.
     * @param text Texte du bouton.
     * @param diff Difficulté associée au bouton.
     * @param unlocked Indique si la difficulté est débloquée.
     * @param parentDialog Fenêtre parente.
     * @return Un pointeur vers le bouton créé.
     */
    QPushButton* createDifficultyButton(const QString& text, Game::Difficulty diff, bool unlocked, QDialog* parentDialog);

    /**
     * @brief Ouvre le menu de sélection de la difficulté.
     */
    void openDifficultyMenu();

    /**
     * @brief Gère l'événement de peinture de la fenêtre.
     * @param event Événement de peinture.
     */
    void paintEvent(QPaintEvent *event);

    /**
     * @brief Ouvre le menu de sauvegarde.
     */
    void openSaveMenu();

    /**
     * @brief Sauvegarde l'état du jeu dans un slot donné.
     * @param slot Le numéro du slot de sauvegarde.
     */
    void saveToSlot(int slot);

    /**
     * @brief Charge l'état du jeu depuis un slot donné.
     * @param slot Le numéro du slot de sauvegarde.
     */
    void loadFromSlot(int slot);

    /**
     * @brief Image représentant un obstacle (pneu).
     */
    QImage obstaclePImage;

    /**
     * @brief Image représentant un obstacle (voiture orientée à gauche).
     */
    QImage obstacleGImage;

    /**
     * @brief Image représentant un obstacle (voiture orientée à droite).
     */
    QImage obstacleDImage;

    /**
     * @brief Image représentant une limitation 30 (SLOW-MO 1).
     */
    QImage perk1Image;

    /**
     * @brief Image représentant un bouclier (SHIELD 2).
     */
    QImage perk2Image;

    /**
     * @brief Image représentant un éclair (TURBO 3).
     */
    QImage perk3Image;



    /**
     * @brief Effectue un test de régression unitaire sur la classe MainWindow.
     */
    void testRegression();

private:
    Ui::MainWindow *ui; ///< Interface utilisateur générée.

    QTimer timerT; ///< Timer pour les ticks du jeu.
    QTimer timerS; ///< Timer pour les secondes écoulées.

    unsigned int timerP; ///< Sauvegarde du temps restant du POWERUP actif avant de démarrer le menu pause.

    QElapsedTimer elapsed; ///< Chronomètre pour mesurer le temps écoulé.
    int elapsedS = 0; ///< Temps écoulé en secondes.

    /**
     * @brief Crée le widget du menu principal.
     * @return Un pointeur vers le widget créé.
     */
    QWidget *createMenuWidget();

    /**
     * @brief Nettoie les ressources du jeu.
     */
    void cleanupGame();

    /**
     * @brief Sauvegarde les raccourcis clavier.
     */
    void saveKeyBindings();

    /**
     * @brief Charge les raccourcis clavier.
     */
    void loadKeyBindings();

    QSlider *musicVolumeSlider;
    QSlider *effectsVolumeSlider;
    QLabel *musicVolumeLabel;
    QLabel *effectsVolumeLabel;

    QTimer leftKeyTimer; ///< Timer pour la touche gauche.
    QTimer rightKeyTimer; ///< Timer pour la touche droite.
    bool leftKeyPressed = false; ///< Indique si la touche gauche est pressée.
    bool rightKeyPressed = false; ///< Indique si la touche droite est pressée.

    QStackedWidget *stackedWidget; ///< Widget empilé pour gérer les différentes interfaces.
    QDialog *settingsDialog; ///< Fenêtre de paramètres.
    QMap<QString, QKeySequenceEdit*> keyBindEdits; ///< Map des raccourcis clavier.
    QWidget *gameWidget; ///< Widget de l'écran de jeu.
    QWidget *menuWidget; ///< Widget du menu principal.

    QWidget *pauseMenuWidget; ///< Widget du menu de pause.
    QLabel *pauseScoreLabel; ///< Label affichant le score dans le menu de pause.

    const int PAUSE_MENU_WIDTH = 400; ///< Largeur du menu de pause.
    const int PAUSE_MENU_HEIGHT = 300; ///< Hauteur du menu de pause.
    const int PAUSE_BUTTON_WIDTH = 200; ///< Largeur des boutons du menu de pause.
    const int PAUSE_BUTTON_HEIGHT = 60; ///< Hauteur des boutons du menu de pause.

private slots:
    void setMusicVolume(int volume);
    void setEffectsVolume(int volume);
};

#endif // MAINWINDOW_H
