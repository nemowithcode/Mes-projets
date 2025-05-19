#ifndef MAINTEXTUEL_H
#define MAINTEXTUEL_H

#include <QTextEdit>
#include "head/game.h"
#include "qelapsedtimer.h"
#include <QTimer>
#include <QMap>
#include <QKeySequence>

class MainTextuel : public QTextEdit {
    Q_OBJECT

public slots:
    void tick();
    void seconds();

private slots:
    void handleMenuInput(int input);
    //void handleSettingsInput(int input); // New function for settings menu input

public:
    MainTextuel(QWidget *parent = nullptr, short int valx = 0, short int valy = 0);
    ~MainTextuel();

    void updateDisplay();

    Game g;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event);

private:
    enum GameState {
        MAIN_MENU,
        DIFFICULTY_SELECTION,
        PLAYING,
        PAUSED,
        GAME_OVER,
        SKIN_SELECTION,
        SETTINGS,
        SAVE_MENU
    };

    GameState currentState = MAIN_MENU;
    GameState previousState = MAIN_MENU; // Pour revenir en arrière

    void showMainMenu();
    void showDifficultyMenu();
    void showSkinMenu();
    void showSettingsMenu();
    void showSaveMenu();
    void startGame(Game::Difficulty diff);
    void changeCarSkin(int skinId);
    void saveToSlot(int slot);
    void loadFromSlot(int slot);
    void togglePause();
    void resetGame();
    void applySettings();

    QTimer timer;
    QTimer timerS;
    QElapsedTimer elapsed;
    int elapsedS = 0;

    QTimer leftKeyTimer; ///< Timer pour la touche gauche.
    QTimer rightKeyTimer; ///< Timer pour la touche droite.
    bool leftKeyPressed = false; ///< Indique si la touche gauche est pressée.
    bool rightKeyPressed = false; ///< Indique si la touche droite est pressée.

    static constexpr int CONSOLE_COLS = 80;
    static constexpr int CONSOLE_ROWS = 25; // Increased rows for menus
    static constexpr int ROAD_WIDTH = 30;
    static constexpr float LOGICAL_WIDTH = 100.0f;
    static constexpr float MOVE_STEP = LOGICAL_WIDTH / ROAD_WIDTH; // MOVE_STEP adjusted
    static constexpr float LEFT_MARGIN = 3.0f;
    static constexpr float RIGHT_MARGIN = 6.0f;
    static constexpr float MIN_X = LEFT_MARGIN;
    static constexpr float MAX_X = LOGICAL_WIDTH - RIGHT_MARGIN;
    static constexpr float WARNING_ZONE = 5.0f;

    QPoint convertToConsolePos(float logicalX, float logicalY);
    bool isNearBorder(float x) const;

    // Settings data
    QMap<QString, int> keyBindings; // Use int for keys
    QString currentSettingAction; // "Gauche" or "Droite"
};

#endif // MAINTEXTUEL_H
