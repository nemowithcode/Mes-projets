#include "head/maintextuel.h"
#include <QKeyEvent>
#include <QDebug>
#include <cmath>
#include <QApplication>
#include <QKeySequence>

using namespace std;

void MainTextuel::tick() {
    if (currentState == PLAYING) {
        updateDisplay();
        g.environment();
        if (g.checkCollision()) { // Check for game over
            currentState = GAME_OVER;
            timer.stop();
            timerS.stop();
            updateDisplay();
            resetGame();
        }
    }
}

void MainTextuel::seconds() {
    if (currentState == PLAYING) {
        elapsedS = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed.durationElapsed()).count() / 1000);
        g.addScore(10);
        if (elapsedS % 5 == 1) g.addaRoute(0.1);
    }
}

MainTextuel::MainTextuel(QWidget *parent, short int valx, short int valy)
    : QTextEdit(parent), g(valx, valy) {

    QDir::setCurrent(QCoreApplication::applicationDirPath() + "../../../../");

    g.leftKey = Qt::Key_Left;
    g.rightKey = Qt::Key_Right;

    QFont font("Courier New", 12, QFont::Monospace);
    font.setStyleHint(QFont::Monospace);
    this->setFont(font);

    this->setReadOnly(true);
    this->setTextInteractionFlags(Qt::NoTextInteraction);
    this->setFocusPolicy(Qt::StrongFocus);

    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer.start(10);
    connect(&timerS, SIGNAL(timeout()), this, SLOT(seconds()));
    timerS.start(1000);
    elapsed.start();

    // Initialize key bindings
    keyBindings["Gauche"] = g.leftKey;
    keyBindings["Droite"] = g.rightKey;

    showMainMenu(); // Show main menu on start

    leftKeyTimer.setInterval(16); // Fréquence de déplacement (en ms)
    rightKeyTimer.setInterval(16);

    connect(&leftKeyTimer, &QTimer::timeout, [this]() {
        if (leftKeyPressed && currentState == PLAYING) {
            g.actionTouche('<');
            updateDisplay();
        }
    });

    connect(&rightKeyTimer, &QTimer::timeout, [this]() {
        if (rightKeyPressed && currentState == PLAYING) {
            g.actionTouche('>');
            updateDisplay();
        }
    });
}

MainTextuel::~MainTextuel() {}

QPoint MainTextuel::convertToConsolePos(float logicalX, float logicalY) {
    const int roadStart = (CONSOLE_COLS - ROAD_WIDTH) / 2;

    // Ensure logicalX is within valid bounds
    logicalX = std::max(0.0f, std::min(100.0f, logicalX));

    // Calculate the console position based on the ratio
    int consoleX = roadStart + static_cast<int>(std::round((logicalX / 100.0f) * (ROAD_WIDTH - 1)));
    consoleX = std::max(roadStart, std::min(roadStart + ROAD_WIDTH - 1, consoleX));

    int consoleY = static_cast<int>(std::round((logicalY / LOGICAL_WIDTH) * CONSOLE_ROWS));
    consoleY = std::min(CONSOLE_ROWS - 1, std::max(0, consoleY));

    return QPoint(consoleX, consoleY);
}

void MainTextuel::updateDisplay() {
    this->clear();
    QString display;

    if (currentState == PLAYING || currentState == PAUSED || currentState == GAME_OVER) {
        const int roadStart = (CONSOLE_COLS - ROAD_WIDTH) / 2;
        QPoint carPos = convertToConsolePos(g.c.getPos().x(), g.c.getPos().y());

        std::vector<QPoint> obstaclePositions;
        for (const auto& obs : g.tab_obstacle) {
            obstaclePositions.push_back(convertToConsolePos(obs.h.pos.x(), obs.h.pos.y()));
        }

        for (int y = 0; y < CONSOLE_ROWS; y++) {
            for (int x = 0; x < CONSOLE_COLS; x++) {
                bool isObstacle = false;
                for (const auto& obsPos : obstaclePositions) {
                    if (x == obsPos.x() && y == obsPos.y()) {
                        display += "■";
                        isObstacle = true;
                        break;
                    }
                }

                if (isObstacle) continue;

                if (x == carPos.x() && y == carPos.y()) {
                    display += "█";
                } else if (x == roadStart || x == roadStart + ROAD_WIDTH - 1) {
                    display += "|";
                } else if (x > roadStart && x < roadStart + ROAD_WIDTH - 1) {
                    display += ".";
                } else {
                    display += " ";
                }
            }
            display += "\n";
        }

        // Avertissement si proche des bords
        Position pos = g.c.getPos();
        if (pos.x() < MIN_X + WARNING_ZONE || pos.x() > MAX_X - WARNING_ZONE) {
            display += "\nATTENTION : Approche du bord !";
        }

        display += QString("\nScore: %1  Speed: %2").arg(g.score()).arg(g.aRoute());

        if (currentState == GAME_OVER) {
            display = "GAME OVER\n\nAppuyez sur [Espace] pour recommencer.\n\n" + display;
        } else if (currentState == PAUSED) {
            display = "PAUSE\n\nAppuyez sur [Espace] pour continuer.\nAppuyez sur [M] pour le menu principal.\n\n" + display;
        }

    } else if (currentState == MAIN_MENU) {
        display = "ROAD RUSH\n\n"
                  "1. Jouer\n"
                  "2. Skins\n"
                  "3. Paramètres\n"
                  "4. Sauvegarder\n"
                  "5. Quitter\n\n"
                  "Entrez votre choix: (Retour pour annuler)";
    } else if (currentState == DIFFICULTY_SELECTION) {
        display = "Choisir la difficulté:\n\n"
                  "1. Facile\n"
                  "2. Moyen (Score minimum : 1000)\n"
                  "3. Difficile (Score minimum : 3000)\n\n"
                  "Entrez votre choix: (Retour pour annuler)";
    } else if (currentState == SKIN_SELECTION) {
        display = "Choisir un skin:\n\n"
                  "1. Vous avez pas le choix, c'est blanc ici. c: \n"
                  "Entrez votre choix: (Retour pour annuler)";
    } else if (currentState == SETTINGS) {
        display = "Paramètres:\n\n"
                  "1. Changer les contrôles\n"
                  "2. Retour au menu principal\n\n"
                  "Entrez votre choix: (Retour pour annuler)";
    } else if (currentState == SETTINGS && currentSettingAction.length() > 0) {
        display = QString("Paramètres : Changer les contrôles\n\n"
                          "Appuyez sur la nouvelle touche pour : %1\n"
                          "(Retour pour annuler)").arg(currentSettingAction);
    } else if (currentState == SAVE_MENU) {
        display = "Sauvegarder/Charger:\n\n"
                  "1. Slot 1\n"
                  "2. Slot 2\n"
                  "3. Slot 3\n"
                  "4. Retour au menu principal\n\n"
                  "Entrez votre choix (S pour Sauvegarder, C pour Charger): (Retour pour annuler)";
    } else if (currentState == SETTINGS && currentSettingAction.length() > 0) {
        display = QString("Paramètres : Changer les contrôles\n\n"
                          "Appuyez sur la nouvelle touche pour : %1\n"
                          "(Retour pour annuler)").arg(currentSettingAction);
    }

    this->setText(display);
}

void MainTextuel::showMainMenu() {
    previousState = currentState;
    currentState = MAIN_MENU;
    updateDisplay();
}

void MainTextuel::showDifficultyMenu() {
    previousState = currentState;
    currentState = DIFFICULTY_SELECTION;
    updateDisplay();
}

void MainTextuel::showSkinMenu() {
    previousState = currentState;
    currentState = SKIN_SELECTION;
    updateDisplay();
}

void MainTextuel::showSettingsMenu() {
    previousState = currentState;
    currentState = SETTINGS;
    updateDisplay();
}

void MainTextuel::showSaveMenu() {
    previousState = currentState;
    currentState = SAVE_MENU;
    updateDisplay();
}

void MainTextuel::startGame(Game::Difficulty diff) {
    g.setDifficulty(diff);
    currentState = PLAYING;
    resetGame();
    timer.start(10);
    timerS.start(1000);
    elapsed.start();
    updateDisplay();
}

void MainTextuel::changeCarSkin(int skinId) {
    switch (skinId) {
    case 1:
        g.c.setSkin(Car::RED);
        break;
    case 2:
        g.c.setSkin(Car::BLUE);
        break;
    case 3:
        g.c.setSkin(Car::WHITE);
        break;
    case 4:
        g.c.setSkin(Car::GREEN);
        break;
    }
    updateDisplay();
}

void MainTextuel::saveToSlot(int slot) {
    QJsonObject gameData = g.toJson();
    gameData["leftKey"] = g.leftKey;
    gameData["rightKey"] = g.rightKey;
    gameData["difficulty"] = static_cast<int>(g.currentDifficulty());
    g.saveManager->manualSave(slot, gameData);
    updateDisplay();
}

void MainTextuel::loadFromSlot(int slot) {
    QJsonObject gameData = g.saveManager->loadManualSave(slot);
    if (!gameData.isEmpty()) {
        g.fromJson(gameData);
        g.leftKey = gameData["leftKey"].toInt();
        g.rightKey = gameData["rightKey"].toInt();
        updateDisplay();
    }
}

void MainTextuel::togglePause() {
    if (currentState == PLAYING) {
        currentState = PAUSED;
        timer.stop();
        timerS.stop();
    } else if (currentState == PAUSED) {
        currentState = PLAYING;
        timer.start();
        timerS.start();
    }
    updateDisplay();
}

void MainTextuel::resetGame() {
    timer.stop();
    timerS.stop();
    elapsed.restart();
    g.setScore(0);
    g.i.showGame(); // Ensure game state is set
    g.event_->setEventTo(0);
    g.tab_obstacle.clear();
    g.c.setPos(50, 75);
}

void MainTextuel::applySettings() {
    g.leftKey = keyBindings["Gauche"];
    g.rightKey = keyBindings["Droite"];
}

void MainTextuel::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;

    if (currentState == MAIN_MENU || currentState == DIFFICULTY_SELECTION ||
        currentState == SKIN_SELECTION || currentState == SETTINGS || currentState == SAVE_MENU) {
        if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_5) {
            handleMenuInput(event->key() - Qt::Key_0);
        } else if (currentState == SAVE_MENU && (event->key() == Qt::Key_S || event->key() == Qt::Key_C)) {
            handleMenuInput(event->key());
        } else if (event->key() == Qt::Key_Backspace) {
            handleMenuInput(0); // 0 will be our "back" signal
        }
    } else if (currentState == PLAYING) {
        switch (event->key()) {
        case Qt::Key_Left:
            if (!leftKeyPressed) {
                leftKeyPressed = true;
                leftKeyTimer.start();
            }
            break;
        case Qt::Key_Right:
            if (!rightKeyPressed) {
                rightKeyPressed = true;
                rightKeyTimer.start();
            }
            break;
        case Qt::Key_Escape:
            togglePause();
            break;
        }
    } else if (currentState == PAUSED) {
        if (event->key() == Qt::Key_Escape) {
            togglePause();
        } else if (event->key() == Qt::Key_M) {
            showMainMenu();
        }
    } else if (currentState == GAME_OVER) {
        if (event->key() == Qt::Key_Space) {
            showMainMenu();
        }
    } else if (currentState == SETTINGS && currentSettingAction.length() > 0) {
        // Handle key assignment
        keyBindings[currentSettingAction] = event->key();
        currentSettingAction = (currentSettingAction == "Gauche") ? "Droite" : ""; // Cycle to "Droite" or reset
        applySettings();
        if (currentSettingAction.isEmpty()) {
            showSettingsMenu(); // Go back to settings menu after setting both keys
        } else {
            updateDisplay(); // Ask for the other key
        }
    }
    updateDisplay();
}

void MainTextuel::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    if (event->key() == g.rightKey) {
        rightKeyPressed = false;
        rightKeyTimer.stop();
    } else if (event->key() == g.leftKey) {
        leftKeyPressed = false;
        leftKeyTimer.stop();
    }
}

void MainTextuel::handleMenuInput(int input) {
    switch (currentState) {
    case MAIN_MENU:
        switch (input) {
        case 1:
            showDifficultyMenu();
            break;
        case 2:
            showSkinMenu();
            break;
        case 3:
            showSettingsMenu(); // Correction ici !
            break;
        case 4:
            showSaveMenu();
            break;
        case 5:
            QApplication::quit();
            break;
        case 0:
            // No previous state to go back to in main menu
            break;
        }
        break;
    case DIFFICULTY_SELECTION:
        switch (input) {
        case 1:
            startGame(Game::EASY);
            break;
        case 2:
            if (g.bestScore() >= 1000)
                startGame(Game::MEDIUM);
            else
                this->setText(this->toPlainText() + "\nScore insuffisant pour débloquer cette difficulté");
            break;
        case 3:
            if (g.bestScore() >= 3000)
                startGame(Game::HARD);
            else
                this->setText(this->toPlainText() + "\nScore insuffisant pour débloquer cette difficulté");
            break;
        case 0:
            showMainMenu();
            break;
        }
        break;
    case SKIN_SELECTION:
        if (input >= 1 && input <= 4) {
            changeCarSkin(input);
            showMainMenu();
        } else if (input == 0) {
            showMainMenu();
        }
        break;
    case SETTINGS:
        switch (input) {
        case 1:
            //  Changer les contrôles (à implémenter)
            currentSettingAction = "Gauche";
            updateDisplay();
            break;
        case 2:
            showMainMenu();
            break;
        case 0:
            showMainMenu();
            break;
        }
        break;
    case SAVE_MENU:
        if (input >= 1 && input <= 3) {
            if (this->toPlainText().endsWith("S pour Sauvegarder, C pour Charger): (Retour pour annuler)"))
                this->setText(this->toPlainText().remove(this->toPlainText().length() - 50, 50));
            this->setText(this->toPlainText() + QString("\nSauvegarder ou Charger dans le slot %1 ? (S/C): (Retour pour annuler)").arg(input));
        } else if (input == Qt::Key_S || input == Qt::Key_C) {
            int slot = this->toPlainText().right(2).toInt();
            if (input == Qt::Key_S) {
                saveToSlot(slot);
                this->setText(this->toPlainText() + "\nSauvegarde effectuée");
            } else if (input == Qt::Key_C) {
                loadFromSlot(slot);
                this->setText(this->toPlainText() + "\nChargement effectuée");
            }
            showMainMenu();
        } else if (input == 4 || input == 0) {
            showMainMenu();
        }
        break;
    default:
        break;
    }
}
