#include "head/mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent, unsigned int px, unsigned int py)
    : QMainWindow(parent), g(px, py), ui(new Ui::MainWindow)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath() + "../../../../");

    ui->setupUi(this);
    loadKeyBindings();

    pauseMenuWidget = nullptr;
    createPauseMenu();

    // Stacked Widg et
    stackedWidget = new QStackedWidget();
    setCentralWidget(stackedWidget); // IMPORTANT: Le parent est this par défaut

    // Menu
    menuWidget = createMenuWidget();
    gameWidget = new QWidget();

    stackedWidget->addWidget(menuWidget);
    stackedWidget->addWidget(gameWidget);

    connect(&timerT, SIGNAL(timeout()),this, SLOT(tick())); // Connexion du timer TICK
    connect(&timerS, SIGNAL(timeout()),this, SLOT(seconds())); // Connexion du timer SECONDS

    connect(&leftKeyTimer, &QTimer::timeout, [this]() { if (leftKeyPressed && g.i.isPlaying()) g.actionTouche('<'); });
    connect(&rightKeyTimer, &QTimer::timeout, [this]() { if (rightKeyPressed && g.i.isPlaying()) g.actionTouche('>'); });

    int id = QFontDatabase::addApplicationFont("fonts/paciencia.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont paciencia(family);

    int id2 = QFontDatabase::addApplicationFont("fonts/shockwave.ttf");
    if (id2 == -1) {
        qWarning() << "ERREUR : Impossible de charger la police shockwave depuis :/fonts/shockwave.ttf";
    } else {
        QString family2 = QFontDatabase::applicationFontFamilies(id2).at(0);
        qDebug() << "Police shockwave chargée avec succès. Famille :" << family2;
        QFont shockwave(family2);
    }

    g.fromJson(g.saveManager->loadLastAutoSave());

    obstacleDImage.load("data/img/obstacleD.png");
    if (obstacleDImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de l'obstacleD !";
    } else qDebug() << "Image obstacleD chargée avec succès";

    obstacleGImage.load("data/img/obstacleG.png");
    if (obstacleGImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de l'obstacleG !";
    } else qDebug() << "Image obstacleG chargée avec succès";

    obstaclePImage.load("data/img/obstacleP.png");
    if (obstaclePImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de l'obstacleP !";
    } else qDebug() << "Image obstacleP chargée avec succès";

    perk3Image.load("data/img/perk3.png");
    if (perk3Image.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image du turbo !";
    } else qDebug() << "Image turbo chargée avec succès";

    perk2Image.load("data/img/perk2.png");
    if (perk2Image.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image du bouclier !";
    } else qDebug() << "Image bouclier chargée avec succès";

    perk1Image.load("data/img/perk1.png");
    if (perk1Image.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image du ralentisseur !";
    } else qDebug() << "Image ralentisseur chargée avec succès";

}

MainWindow::~MainWindow()
{
    delete stackedWidget;
    delete pauseMenuWidget;
    delete ui;
}


void MainWindow::openSettings()
{
    settingsDialog = new QDialog(this);
    settingsDialog->setWindowTitle("Configuration des touches");
    settingsDialog->setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(settingsDialog);
    QGroupBox *keyGroup = new QGroupBox("Assignation des touches", settingsDialog);
    QFormLayout *formLayout = new QFormLayout(keyGroup);

    keyBindEdits["Gauche"] = new QKeySequenceEdit(QKeySequence(g.leftKey));
    keyBindEdits["Droite"] = new QKeySequenceEdit(QKeySequence(g.rightKey));

    formLayout->addRow("Tourner à gauche:", keyBindEdits["Gauche"]);
    formLayout->addRow("Tourner à droite:", keyBindEdits["Droite"]);

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        settingsDialog);

    // indicateur pour doublons
    auto updateKeyConflictWarning = [this]() {
        bool hasConflict = (keyBindEdits["Gauche"]->keySequence()[0] ==
                            keyBindEdits["Droite"]->keySequence()[0]);

        QString conflictStyle = "background-color: #FFCCCC;";
        QString normalStyle = "";

        keyBindEdits["Gauche"]->setStyleSheet(hasConflict ? conflictStyle : normalStyle);
        keyBindEdits["Droite"]->setStyleSheet(hasConflict ? conflictStyle : normalStyle);

        return hasConflict;
    };

    // 2. vérification tmps réel
    connect(keyBindEdits["Gauche"], &QKeySequenceEdit::keySequenceChanged,
            [/*this, */updateKeyConflictWarning]() {
                updateKeyConflictWarning();
            });

    connect(keyBindEdits["Droite"], &QKeySequenceEdit::keySequenceChanged,
            [/*this, */updateKeyConflictWarning]() {
                updateKeyConflictWarning();
            });

    // 3. modif de la connexion pr vérifier avant sauvegarde
    disconnect(buttons, &QDialogButtonBox::accepted, this, &MainWindow::saveKeyBindings);
    connect(buttons, &QDialogButtonBox::accepted, [this, updateKeyConflictWarning]() {
        if (updateKeyConflictWarning()) {
            QMessageBox::warning(this, "Conflit de touches",
                                 "Les touches gauche et droite doivent être différentes !");
            return;
        }
        saveKeyBindings();
    });

    connect(buttons, &QDialogButtonBox::rejected, settingsDialog, &QDialog::reject);

    layout->addWidget(keyGroup);
    layout->addWidget(buttons);

    // Ajout des sliders de volume
    QGroupBox *volumeGroup = new QGroupBox("Volume", settingsDialog);
    QFormLayout *volumeLayout = new QFormLayout(volumeGroup);

    musicVolumeSlider = new QSlider(Qt::Horizontal, settingsDialog);
    musicVolumeSlider->setMinimum(0);
    musicVolumeSlider->setMaximum(100);
    musicVolumeSlider->setValue(static_cast<int>(g.audioOutput->volume() * 100)); // Récupérer le volume actuel

    effectsVolumeSlider = new QSlider(Qt::Horizontal, settingsDialog);
    effectsVolumeSlider->setMinimum(0);
    effectsVolumeSlider->setMaximum(100);
    effectsVolumeSlider->setValue(static_cast<int>(g.sfxAudioOutput->volume() * 100)); // Valeur par défaut pour les effets

    musicVolumeLabel = new QLabel("Musique", settingsDialog);
    effectsVolumeLabel = new QLabel("Effets", settingsDialog);

    volumeLayout->addRow(musicVolumeLabel, musicVolumeSlider);
    volumeLayout->addRow(effectsVolumeLabel, effectsVolumeSlider);

    layout->addWidget(volumeGroup);

    // Connexions des sliders
    connect(musicVolumeSlider, &QSlider::valueChanged, this, &MainWindow::setMusicVolume);
    connect(effectsVolumeSlider, &QSlider::valueChanged, this, &MainWindow::setEffectsVolume);

    settingsDialog->exec();
}

void MainWindow::startGame() {
    g.i.showGame();
    stackedWidget->setCurrentWidget(gameWidget);
    gameWidget->update();
    update();
    timerT.start(20);
    timerS.start(1000);
    leftKeyTimer.setInterval(20);
    rightKeyTimer.setInterval(20);
    elapsed.start();
}

void MainWindow::saveKeyBindings()
{
    g.leftKey = keyBindEdits["Gauche"]->keySequence()[0];
    g.rightKey = keyBindEdits["Droite"]->keySequence()[0];
    // Sauvegardez d'autres touches...

    settingsDialog->accept();
    qDebug() << "Touches configurées - Gauche:" << QKeySequence(g.leftKey).toString()
             << "Droite:" << QKeySequence(g.rightKey).toString();
}

void MainWindow::loadKeyBindings()
{
    // on pourra charger ça depuis une save
    if (g.leftKey == 0) g.leftKey = Qt::Key_Left;
    if (g.rightKey == 0) g.rightKey = Qt::Key_Right;
}





QWidget* MainWindow::createMenuWidget()
{
    QLabel* titleLabel = new QLabel("ROAD RUSH");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("paciencia", 70, QFont::Bold));
    titleLabel->setStyleSheet("font-size: 96px; font-weight: bold; color: #FFFFFF;");


    QWidget* menuWidget = new QWidget();
    menuWidget->setObjectName("menuWidget"); // Pour le style CSS

    QVBoxLayout* mainLayout = new QVBoxLayout(menuWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Crucial !
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget* buttonContainer = new QWidget();
    QVBoxLayout* buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setSpacing(25);
    buttonLayout->setContentsMargins(20, 20, 20, 20);

    auto createButton = [](const QString& text, const QString& color) {
        QPushButton* btn = new QPushButton(text);
        btn->setFixedSize(220, 70);
        btn->setStyleSheet(QString(
                               "QPushButton {"
                               "   background-color: %1;"
                               "   color: white;"
                               "   font-size: 22px;"
                               "   border-radius: 15px;"
                               "   border: 2px solid #FFF;"
                               "}"
                               "QPushButton:hover {"
                               "   background-color: %2;"
                               "   border: 2px solid gold;"
                               "}"
                               ).arg(color, QColor(color).darker(120).name()));
        return btn;
    };

    QPushButton* startButton = createButton("Start", "#4CAF50");
    QPushButton* skinButton = createButton("Changer Skin", "#9C27B0");
    QPushButton* settingsButton = createButton("Settings", "#008CBA");
    QPushButton* saveButton = createButton("Sauvegarde", "#FF9800");
    QPushButton *quitBtn     = createButton("Quitter le jeu", "#F44336");
    // QPushButton* difficultyButton = createButton("Difficulté", "#9C27B0");


    // 5. Assemblage
    buttonLayout->addStretch();
    buttonLayout->addWidget(startButton, 0, Qt::AlignHCenter);
    buttonLayout->addWidget(skinButton, 0, Qt::AlignHCenter);
    buttonLayout->addWidget(settingsButton, 0, Qt::AlignHCenter);
    buttonLayout->addWidget(saveButton, 0, Qt::AlignHCenter);
    buttonLayout->addWidget(quitBtn, 0, Qt::AlignHCenter);
    // buttonLayout->addWidget(difficultyButton, 0, Qt::AlignHCenter);
    buttonLayout->addStretch();

    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonContainer, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // 6. Connexions
    connect(startButton, &QPushButton::clicked, this, &MainWindow::showDifficultySelection);
    connect(skinButton, &QPushButton::clicked, this, &MainWindow::openSkinSelection);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
    connect(saveButton, &QPushButton::clicked, this, &::MainWindow::openSaveMenu);
    connect(quitBtn, &QPushButton::clicked, this, &MainWindow::close);
    // connect(difficultyButton, &QPushButton::clicked, this, &MainWindow::openDifficultyMenu);

    return menuWidget;
}

void MainWindow::changeCarSkin(int skinId)
{
    // Ici vous implémenterez le changement de skin
    // Par exemple :
    switch(skinId) {
    case 0: // Rouge Classique
        g.c.setSkin(Car::RED);
        break;
    case 1: // Bleu Sport
        g.c.setSkin(Car::BLUE);
        break;
    case 2: // Blanc mat
        g.c.setSkin(Car::WHITE);
        break;
    case 3: // Vert Neon
        g.c.setSkin(Car::GREEN);
        break;
    }
    update(); // Rafraîchir l'affichage
}

void MainWindow::openSkinSelection()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Choisir un skin");
    dialog.setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("SELECTION DU SKIN");
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Liste des skins disponibles
    QStringList skins = {"Rouge Classique", "Bleu Sport", "Blanc mat", "Vert Neon"};

    QButtonGroup *skinGroup = new QButtonGroup(&dialog);

    for (int i = 0; i < skins.size(); ++i) {
        QPushButton *skinBtn = new QPushButton(skins[i]);
        skinBtn->setCheckable(true);
        skinBtn->setStyleSheet(
            "QPushButton {"
            "   padding: 10px;"
            "   font-size: 16px;"
            "   border-radius: 5px;"
            "   margin: 5px;"
            "}"
            "QPushButton:checked {"
            "   background-color: #4CAF50;"
            "   color: white;"
            "}"
            );
        skinGroup->addButton(skinBtn, i);
        layout->addWidget(skinBtn);
    }

    // Bouton Valider
    QPushButton *validateBtn = new QPushButton("Valider");
    validateBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   font-size: 18px;"
        "   padding: 10px;"
        "}"
        );

    connect(validateBtn, &QPushButton::clicked, [&]() {
        int selectedId = skinGroup->checkedId();
        if (selectedId >= 0) {
            changeCarSkin(selectedId);
            dialog.accept();
        }
    });

    layout->addWidget(validateBtn);
    dialog.exec();
}

void MainWindow::cleanupGame()
{
    timerT.stop();
    timerS.stop();

    g.event_->setEventTo(0);
    g.event_->setMultScore(1);
    g.event_->setMultSpeed(1);

    g.mediaPlayer->stop();
    g.setScore(0);
    g.setaRoute(1);
    g.i.showMain();
    update();
}

void MainWindow::resetTimers(){
    timerS.stop();
    timerT.stop();
    elapsed.restart();
    // timerS.start(1000);
    // timerT.start(20);
}

QPushButton* MainWindow::createDifficultyButton(const QString& text, Game::Difficulty diff, bool unlocked, QDialog* parentDialog) {
    QPushButton *btn = new QPushButton(text);
    btn->setFixedSize(300, 80);
    btn->setEnabled(unlocked);

    QString style = QString(
                        "QPushButton {"
                        "   font-size: 22px;"
                        "   font-weight: bold;"
                        "   border-radius: 15px;"
                        "   padding: 10px;"
                        "   background-color: %1;"
                        "   color: white;"
                        "   border: 3px solid %2;"
                        "}"
                        "QPushButton:hover { background-color: %3; }"
                        "QPushButton:disabled { background-color: #555; color: #888; }"
                        ).arg(
                            unlocked ? "#4CAF50" : "#555",
                            unlocked ? "#2E7D32" : "#333",
                            unlocked ? "#388E3C" : "#555"
                            );

    btn->setStyleSheet(style);

    connect(btn, &QPushButton::clicked, [this, diff, parentDialog]() {
        startGameWithDifficulty(diff);
        parentDialog->accept();  // Fermer le dialogue parent
    });

    return btn;
}

void MainWindow::showDifficultySelection() {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Choix de la difficulté");
    dialog->setFixedSize(400, 350);
    dialog->setStyleSheet("background-color: #222; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *title = new QLabel("CHOISISSEZ VOTRE DIFFICULTÉ");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #FFFFFF;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Espacement haut
    layout->addSpacing(20);

    bool mediumUnlocked = g.bestScore() >= 1000;
    bool hardUnlocked = g.bestScore() >= 3000;

    auto createButton = [this, dialog](const QString& text, Game::Difficulty diff, bool unlocked) {
        QPushButton *btn = new QPushButton(text);
        btn->setFixedSize(200, 60);
        btn->setEnabled(unlocked);

        QString style;
        if (text == "MOYEN") {
            style = QString(
                        "QPushButton {"
                        "   font-size: 18px;"
                        "   font-weight: bold;"
                        "   border-radius: 10px;"
                        "   background-color: %1;"
                        "   color: white;"
                        "   border: 2px solid %2;"
                        "}"
                        "QPushButton:hover { background-color: %3; }"
                        "QPushButton:disabled { background-color: #555; color: #888; }"
                        ).arg(
                            unlocked ? "#FF9800" : "#555",
                            unlocked ? "#F57C00" : "#333",
                            unlocked ? "#FF5722" : "#555"
                            );
        }
        else if (text == "DIFFICILE") {
            style = QString(
                        "QPushButton {"
                        "   font-size: 18px;"
                        "   font-weight: bold;"
                        "   border-radius: 10px;"
                        "   background-color: %1;"
                        "   color: white;"
                        "   border: 2px solid %2;"
                        "}"
                        "QPushButton:hover { background-color: %3; }"
                        "QPushButton:disabled { background-color: #555; color: #888; }"
                        ).arg(
                            unlocked ? "#F44336" : "#555",  // Rouge pour difficile
                            unlocked ? "#D32F2F" : "#333",
                            unlocked ? "#C62828" : "#555"
                            );
        } else {
            style = QString(
                        "QPushButton {"
                        "   font-size: 18px;"
                        "   font-weight: bold;"
                        "   border-radius: 10px;"
                        "   background-color: %1;"
                        "   color: white;"
                        "   border: 2px solid %2;"
                        "}"
                        "QPushButton:hover { background-color: %3; }"
                        "QPushButton:disabled { background-color: #555; color: #888; }"
                        ).arg(
                            unlocked ? "#4CAF50" : "#555",  // Vert pour facile
                            unlocked ? "#2E7D32" : "#333",
                            unlocked ? "#388E3C" : "#555"
                            );
        }
        btn->setStyleSheet(style);
        connect(btn, &QPushButton::clicked, [this, diff, dialog]() {
            startGameWithDifficulty(diff);
            dialog->accept();
        });

        return btn;
    };

    layout->addWidget(createButton("FACILE", Game::EASY, true), 0, Qt::AlignHCenter);
    layout->addSpacing(10);

    QPushButton *mediumBtn = createButton("MOYEN", Game::MEDIUM, mediumUnlocked);
    if (!mediumUnlocked) mediumBtn->setToolTip("Débloqué à 1000 points");
    layout->addWidget(mediumBtn, 0, Qt::AlignHCenter);
    layout->addSpacing(10);

    QPushButton *hardBtn = createButton("DIFFICILE", Game::HARD, hardUnlocked);
    if (!hardUnlocked) hardBtn->setToolTip("Débloqué à 3000 points");
    layout->addWidget(hardBtn, 0, Qt::AlignHCenter);
    layout->addSpacing(20);

    // Bouton Annuler
    QPushButton *cancelBtn = new QPushButton("ANNULER");
    cancelBtn->setFixedSize(150, 50);
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "   font-size: 16px;"
        "   border-radius: 8px;"
        "   background-color: #F44336;"
        "   color: white;"
        "   border: 2px solid #B71C1C;"
        "}"
        "QPushButton:hover { background-color: #D32F2F; }"
        );
    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::reject);
    layout->addWidget(cancelBtn, 0, Qt::AlignHCenter);

    dialog->exec();
    delete dialog;
}


void MainWindow::startGameWithDifficulty(Game::Difficulty diff) {
    g.setDifficulty(diff);
    startGame();
}

void MainWindow::createPauseMenu() {
    pauseMenuWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(pauseMenuWidget);
    pauseMenuWidget->setStyleSheet("background-color: rgba(0, 0, 0, 200); border-radius: 10px;");

    pauseScoreLabel = new QLabel("Score: 0");
    pauseScoreLabel->setStyleSheet("color: white; font-size: 28px;");
    mainLayout->addWidget(pauseScoreLabel, 0, Qt::AlignCenter);

    // Conteneur pour les boutons (comme dans le menu principal)
    QWidget *buttonContainer = new QWidget();
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setSpacing(25); // Espacement entre boutons
    buttonLayout->setContentsMargins(20, 20, 20, 20);

    auto createPauseButton = [](const QString &text, const QString &color) {
        QPushButton *btn = new QPushButton(text);
        btn->setFixedSize(220, 70);
        btn->setStyleSheet(QString(
                               "QPushButton {"
                               "   background-color: %1;"
                               "   color: white;"
                               "   font-size: 22px;"
                               "   border-radius: 15px;"
                               "   border: 2px solid #FFF;"
                               "}"
                               "QPushButton:hover {"
                               "   background-color: %2;"
                               "   border: 2px solid gold;"
                               "}"
                               ).arg(color, QColor(color).darker(120).name()));
        return btn;
    };

    QPushButton *settingsBtn = createPauseButton("Paramètres", "#008CBA");
    QPushButton *returnBtn   = createPauseButton("Retour au jeu", "#4CAF50");
    QPushButton *menuBtn = createPauseButton("Menu principal", "#000FFF");
    QPushButton *quitBtn     = createPauseButton("Quitter le jeu", "#F44336");

    connect(settingsBtn, &QPushButton::clicked, [this]() {
        g.i.hidePauseMenu();
        pauseMenuWidget->hide();
        openSettings();
    });

    connect(returnBtn, &QPushButton::clicked, this, &MainWindow::togglePauseMenu);
    connect(menuBtn, &QPushButton::clicked, [this]() {
        g.i.hidePauseMenu();
        pauseMenuWidget->hide();
        cleanupGame();
        g.i.showMain();
        stackedWidget->setCurrentWidget(menuWidget);
    });
    connect(quitBtn, &QPushButton::clicked, this, &MainWindow::close);

    buttonLayout->addWidget(settingsBtn, 0, Qt::AlignHCenter);
    buttonLayout->addWidget(returnBtn, 0, Qt::AlignHCenter);
    buttonLayout->addWidget(menuBtn, 0, Qt::AlignHCenter);
    buttonLayout->addWidget(quitBtn, 0, Qt::AlignHCenter);

    mainLayout->addStretch();
    mainLayout->addWidget(buttonContainer, 0, Qt::AlignCenter);
    mainLayout->addStretch();
}

void MainWindow::togglePauseMenu() {
    if (g.i.isPauseMenuShown()) {
        g.i.hidePauseMenu();
        pauseMenuWidget->hide();
        timerT.start(20);
        timerS.start(1000);

        g.sfxPlayer->pause();
        g.mediaPlayer->play();
        g.event_->elapsedEvent.start(timerP);

    } else {
        g.i.showPauseMenu();
        g.mediaPlayer->pause();
        g.sfxPlayer->pause();

        timerP = g.event_->elapsedEvent.remainingTime();

        updatePauseMenu();
        pauseMenuWidget->setParent(gameWidget);
        pauseMenuWidget->setGeometry(0, 0, gameWidget->width(), gameWidget->height());
        pauseMenuWidget->show();
        timerT.stop();
        timerS.stop();
    }
}

void MainWindow::updatePauseMenu() {
    pauseScoreLabel->setText(
        QString("SCORE: %1\nMEILLEUR SCORE: %2")
            .arg(g.score())
            .arg(g.bestScore())
        );
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (g.i.isPlaying()) {

        painter.drawText(10, 20, "Score: " + QString::number(g.score()));
        painter.drawText(10, 40, "Vitesse: " + QString::number(round(g.aRoute()-5.0)));
        painter.drawText(10, 60, "Difficulté: " + Game::difficultyToString(g.currentDifficulty()));


        // Dessiner les bordures de la route
        painter.fillRect(g.leftLimWin()-20, 0, 20, 1000, Qt::gray); // Bordure gauche
        painter.fillRect(g.rightLimWin()+40, 0, 20, 1000, Qt::gray); // Bordure droite
        int dRL = (g.rightLimWin() - g.leftLimWin());

        // Dessiner les lignes de la route
        std::vector<RoadLine>& roadLines = g.roadLines();
        for (unsigned int i = 0; i < roadLines.size(); i++) {
            RoadLine& line = roadLines[i];
            painter.fillRect(line.x, line.y, line.width, line.height, Qt::gray);
        }

        // Dessiner les obstacles
        for(int i=0; i<int(g.tab_obstacle.size()); i++) {
            Position pos = g.tab_obstacle[i].h.pos;
            int obstacleX = g.leftLimWin() + (pos.x() * dRL) / g.terrainWidth();
            int obstacleY = (pos.y() * g.screenY()) / g.terrainHeight();
            int obstacleW = (g.tab_obstacle[i].getW()*dRL)/g.terrainWidth();
            int obstacleH = (g.tab_obstacle[i].getL()*g.screenY())/g.terrainHeight();
            
            switch(g.tab_obstacle[i].type()) {
            case 1:
                painter.drawImage(QRect(obstacleX, obstacleY, obstacleW, obstacleH), obstacleDImage.scaled(obstacleW, obstacleH));
                break;
            case 2:
                painter.drawImage(QRect(obstacleX, obstacleY, obstacleW, obstacleH), obstacleGImage.scaled(obstacleW, obstacleH));
                break;
            default:
                painter.drawImage(QRect(obstacleX, obstacleY, obstacleW, obstacleH), obstaclePImage.scaled(obstacleW, obstacleH));
                break;
            }


        }
        for(int i=0; i<int(g.tab_perks.size()); i++) {
            Position pos = g.tab_perks[i].h.pos;
            int perkX = g.leftLimWin() + (pos.x() * dRL) / g.terrainWidth();
            int perkY = (pos.y() * g.screenY()) / g.terrainHeight();
            int perkW = (g.tab_perks[i].getW()*dRL)/g.terrainWidth();
            int perkH = (g.tab_perks[i].getL()*g.screenY())/g.terrainHeight();

            switch(g.tab_perks[i].type()) {
                case 1:
                    painter.drawImage(QRect(perkX, perkY, perkW, perkH), perk1Image.scaled(perkW, perkH));
                    break;
                case 2:
                    painter.drawImage(QRect(perkX, perkY, perkW, perkH), perk2Image.scaled(perkW, perkH));
                    break;
                case 3:
                    painter.drawImage(QRect(perkX, perkY, perkW, perkH), perk3Image.scaled(perkW, perkH));
                    break;
                default:
                    break;
            }


        }

        // Dessiner la voiture
        int carX = g.c.h.pos.x(); // Position X de la voiture dans le système de coordonnées du jeu (0 à 100)
        int carY = g.c.h.pos.y(); // Position Y de la voiture dans le système de coordonnées du jeu (0 à 100)

        // Convertir les coordonnées du jeu en coordonnées d'écran
        int screenCarX = g.leftLimWin() + (carX * (g.rightLimWin()/*-60*/ - g.leftLimWin())) / g.terrainWidth();
        int screenCarY = (carY * g.screenY()) / g.terrainHeight();
        int screenCarL = g.c.h.l() * g.screenY() / g.terrainHeight();
        int screenCarW = g.c.h.w() * dRL / g.terrainWidth();

        QColor carColor;
        switch(g.c.skin()) {
            case Car::BLUE:
                carColor = Qt::blue; break;

            case Car::WHITE:
                carColor = Qt::white; break;

            case Car::GREEN:
                carColor = Qt::green; break;

            default:
                carColor = Qt::red; // Rouge par défaut
        }

        painter.fillRect(screenCarX, screenCarY, screenCarW, screenCarL, carColor); // Dessin de la voiture

        short int chrono = 1+(std::chrono::duration_cast<std::chrono::milliseconds>(g.event_->elapsedEvent.remainingTimeAsDuration()).count() / 1000);
        short int chronomax = (std::chrono::duration_cast<std::chrono::milliseconds>(g.event_->elapsedEvent.intervalAsDuration()).count() / 1000);

        QRect originalRect = rect();
        QRect adjustedRect = originalRect.adjusted(0, 0, 0, 400);

        switch(g.event_->isThereAnEvent()) {

            case 3:
                painter.setFont(QFont("shockwave", 120, QFont::Bold));
                painter.setPen(Qt::red);
                painter.drawText(adjustedRect, Qt::AlignHCenter | Qt::AlignVCenter, QString::number(chrono));

                painter.setFont(QFont("shockwave", 140-4*(chronomax-chrono), QFont::Bold));
                painter.setPen(QColor(255,255,0,20*chrono));
                painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, "TURBO");
                break;

            case 2:
                painter.setFont(QFont("shockwave", 120, QFont::Bold));
                painter.setPen(Qt::blue);
                painter.drawText(adjustedRect, Qt::AlignHCenter | Qt::AlignVCenter, QString::number(chrono));

                painter.setFont(QFont("shockwave", 140-4*(chronomax-chrono), QFont::Bold));
                painter.setPen(QColor(255,0,255,20*chrono));
                painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, "SHIELD");
                break;

            case 1:
                painter.setFont(QFont("shockwave", 120, QFont::Bold));
                painter.setPen(Qt::blue);
                painter.drawText(adjustedRect, Qt::AlignHCenter | Qt::AlignVCenter, QString::number(chrono));

                painter.setFont(QFont("shockwave", 140-4*(chronomax-chrono), QFont::Bold));
                painter.setPen(QColor(0,255,255,20*chrono));
                painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, "SLOW-MO");

                break;

            default:
                break;
        }

    } else if(g.i.hasLost()) {
        painter.setFont(QFont("paciencia", 70, QFont::Bold));
        painter.setPen(Qt::red);
        painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, "YOU DIED");
        painter.setFont(QFont("Arial", 19));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, "\n\n\n\n" "Appuyez sur [Espace] pour recommencer.");
        painter.setFont(QFont("paciencia", 20));
        painter.setPen(Qt::green);
        painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, "\n\n\n\n\n\n\n\n\n\n" "Score : " + QString::number(g.score()));
    }
}

void MainWindow::openSaveMenu() {
    QDialog saveDialog(this);
    saveDialog.setWindowTitle("Gestion des sauvegardes");
    saveDialog.setFixedSize(500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(&saveDialog);

    // Création des slots
    for (int slot = 1; slot <= 3; slot++) {
        QGroupBox *slotGroup = new QGroupBox(QString("Slot %1").arg(slot), &saveDialog);
        QVBoxLayout *slotLayout = new QVBoxLayout(slotGroup);

        // Info du slot
        QStringList saveInfo = g.saveManager->getManualSaveInfo(slot);
        QLabel *infoLabel = new QLabel(saveInfo.join("\n"), &saveDialog);
        slotLayout->addWidget(infoLabel);

        // Boutons
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *saveBtn = new QPushButton("Sauvegarder", &saveDialog);
        QPushButton *loadBtn = new QPushButton("Charger", &saveDialog);

        connect(saveBtn, &QPushButton::clicked, [this, slot, &saveDialog]() {
            saveToSlot(slot);
            saveDialog.accept();
            QMessageBox::information(this, "Succès", "Sauvegarde effectuée!");
        });

        connect(loadBtn, &QPushButton::clicked, [this, slot, &saveDialog]() {
            loadFromSlot(slot);
            saveDialog.accept();
            QMessageBox::information(this, "Succès", "Chargement effectué!");
        });

        buttonLayout->addWidget(saveBtn);
        buttonLayout->addWidget(loadBtn);
        slotLayout->addLayout(buttonLayout);

        mainLayout->addWidget(slotGroup);
    }

    QPushButton *closeBtn = new QPushButton("Fermer", &saveDialog);
    connect(closeBtn, &QPushButton::clicked, &saveDialog, &QDialog::accept);
    mainLayout->addWidget(closeBtn);

    saveDialog.exec();
}

void MainWindow::saveToSlot(int slot) {
    QJsonObject gameData = g.toJson();
    gameData["leftKey"] = g.leftKey;
    gameData["rightKey"] = g.rightKey;
    gameData["sfx"] = g.sfxAudioOutput->volume();
    gameData["song"] = g.audioOutput->volume();
    gameData["difficulty"] = static_cast<int>(g.currentDifficulty());

    g.saveManager->manualSave(slot, gameData);
}

void MainWindow::loadFromSlot(int slot) {
    QJsonObject gameData = g.saveManager->loadManualSave(slot);
    if (!gameData.isEmpty()) {
        g.fromJson(gameData);
        // Mettre à jour les touches
        g.leftKey = gameData["leftKey"].toInt();
        g.rightKey = gameData["rightKey"].toInt();
        g.sfxAudioOutput->setVolume(static_cast<float>(gameData["sfx"].toDouble()));
        g.audioOutput->setVolume(static_cast<float>(gameData["song"].toDouble()));
        loadKeyBindings();
        update();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;

    const int keyPressed = event->key();

    if (g.i.isOnMain()) {
        if (keyPressed == Qt::Key_Space) {
            startGame();
        }
    }
    else if (g.i.isPlaying()) {
        if (keyPressed == g.rightKey) {
            rightKeyPressed = true;
            rightKeyTimer.start();
            g.actionTouche('>');
        }
        else if (keyPressed == g.leftKey) {
            leftKeyPressed = true;
            leftKeyTimer.start();
            g.actionTouche('<');
        }
        else if (keyPressed == Qt::Key_Escape) {
            togglePauseMenu();
        }
    }
    else if (g.i.isOnPause()) {
        if (keyPressed == Qt::Key_Escape) {
            togglePauseMenu();
        }
    }
    else if (g.i.hasLost()) {
        if (keyPressed == Qt::Key_Space) {
            cleanupGame();

            g.i.showMain(); // <- ajoute cette ligne pour bien forcer le retour au menu

            if (stackedWidget->indexOf(menuWidget) == -1) {
                menuWidget = createMenuWidget();
                stackedWidget->insertWidget(0, menuWidget);
            }
            stackedWidget->setCurrentWidget(menuWidget);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    if (event->key() == g.rightKey){
        rightKeyPressed = false;
        rightKeyTimer.stop();
    }
    else if (event->key() == g.leftKey){
        leftKeyPressed = false;
        leftKeyTimer.stop();
    }
}

void MainWindow::tick() {

    if(g.i.isPlaying()) {
        g.environment();
        if (g.checkCollision()) {

            QString soundPath = QString("data/sfx/crash.mp3");

            if (QFile::exists(soundPath)) {
                if (g.sfxPlayer && g.sfxAudioOutput) {

                    g.sfxPlayer->stop();
                    g.sfxPlayer->setSource(QUrl::fromLocalFile(soundPath));

                    g.sfxPlayer->play();
                } else {
                    qWarning() << "sfxPlayer ou sfxAudioOutput non initialisé !";
                }
            } else {
                qWarning() << "Fichier son non trouvé -" << soundPath;
            }

            g.tab_obstacle.clear();
            g.i.showLoss();

            g.autoBest();
            g.saveGame(0);

            Game newGame(g.screenX(), g.screenY());
            newGame.setScore(g.score());
            newGame.setBestScore(g.bestScore());
            newGame.c.setPos(50,75);
            g = newGame;

            resetTimers();

        }
    }
    g.toggleMusic();
    update();
}

void MainWindow::seconds()
{
    elapsedS = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed.durationElapsed()).count() / 1000);
    if(g.i.isPlaying()) {
        switch(g.currentDifficulty()) {
        case Game::EASY:
            g.setScore(5 * elapsedS * g.event_->multSpeed());
            break;
        case Game::MEDIUM:
            g.setScore(10 * elapsedS * g.event_->multSpeed());
            break;
        case Game::HARD:
            g.setScore(15 * elapsedS * g.event_->multSpeed());
            break;
        }
        if (elapsedS % 5 == 1) g.addaRoute(0.1*g.event_->multSpeed());
    }

    if (elapsedS % 10 == 5) {
        g.saveGame(0);
    }


}

void MainWindow::setMusicVolume(int volume) {
    if (g.mediaPlayer) {
        g.audioOutput->setVolume(volume / 100.0f);
    }
}

void MainWindow::setEffectsVolume(int volume) {
    if (g.sfxPlayer) {
        g.sfxAudioOutput->setVolume(volume / 100.0f);
    }

}

void MainWindow::testRegression() {
    qDebug() << "===== DÉBUT testRegression: MainWindow =====";

    Q_ASSERT(g.screenX() >= 10);
    Q_ASSERT(g.screenY() >= 10);
    qDebug() << "✓ Accès au Game OK";

    obstacleDImage = QImage();
    Q_ASSERT(obstacleDImage.isNull());
    qDebug() << "✓ Image D obstacle vide OK";

    qDebug() << "===== FIN testRegression: MainWindow =====";
}
