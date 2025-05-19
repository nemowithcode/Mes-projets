#include "head/game.h"

using namespace std;

Game::Game(short int dimx, short int dimy) : c(50, 75) {
    saveManager = new SaveManager();
    event_ = new Event();

    srand(time(NULL));

    score_ = 0;
    screenX_ = dimx;
    screenY_ = dimy;

    leftLimWin_ = screenX_ * 0.2;
    rightLimWin_ = screenX_ * 0.8;

    vRoute_ = 1;
    dVoiture_ = 0;
    aRoute_ = 6;

    unlockedDifficulty_ = EASY;
    currentDifficulty_ = EASY;

    mediaPlayer = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile(musicPath));
    audioOutput->setVolume(0.1);

    sfxPlayer = new QMediaPlayer();
    sfxAudioOutput = new QAudioOutput();
    sfxPlayer->setAudioOutput(sfxAudioOutput);
    sfxAudioOutput->setVolume(0.1); //
}

Game::~Game() {
    roadLines_.clear();
    tab_obstacle.clear();
    tab_perks.clear();

    delete saveManager;
    delete event_;

    mediaPlayer->stop();
    sfxPlayer->stop();

    delete mediaPlayer;
    delete audioOutput;
    delete sfxPlayer;
    delete sfxAudioOutput;
}

Game &Game::operator=(const Game &other) {
    if (this != &other) {
        score_ = other.score_;
        bestScore_ = other.bestScore_;

        vRoute_ = other.vRoute_;
        dVoiture_ = other.dVoiture_;
        aRoute_ = other.aRoute_;

        leftLimWin_ = other.leftLimWin_;
        rightLimWin_ = other.rightLimWin_;

        consoleWidth_ = other.consoleWidth_;
        consoleHeight_ = other.consoleHeight_;

        terrainWidth_ = other.terrainWidth_;
        terrainHeight_ = other.terrainHeight_;

        screenX_ = other.screenX_;
        screenY_ = other.screenY_;

        currentDifficulty_ = other.currentDifficulty_;
        unlockedDifficulty_ = other.unlockedDifficulty_;
    }
    return *this;
}

void Game::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Game =====";

    Game g(800, 600);

    assert(g.i.isPlaying());
    if (g.i.isPlaying())
        assert(!g.i.isOnPause());

    g.i.showPause();
    assert(!g.i.isPlaying());
    assert(g.i.isOnPause());

    assert(g.screenX() > 100 && g.screenY() > 100);
    assert(g.screenX() < 10000 && g.screenY() < 10000);

    Car voiture(50, 75);
    assert(voiture.h.pos.x() == 50 && voiture.h.pos.y() == 75);
    voiture.h.pos.setX(20);
    voiture.h.pos.setY(40);
    assert(voiture.h.pos.x() == 20 && voiture.h.pos.y() == 40);

    Position p(50, 50);
    Obstacle o(p, 10);
    assert(o.h.pos.x() == p.x() && o.h.pos.y() == p.y());

    qDebug() << "✓ Game::testRegression terminé avec succès.";
}

void Game::actionTouche(unsigned char t) {
    switch (t) {
    case '<':
        if (i.isPlaying()) {
            move(-1);
            break;
        } else
            break;

    case '>':
        if (i.isPlaying()) {
            move(1);
            break;
        } else
            break;

    case '.':
        if (i.isPlaying()) {
        } else if (i.isOnPause()) {
        }
    default:
        break;
    }
}

void Game::setDifficulty(Difficulty diff) {
    qDebug() << "Game::setDifficulty() appelé avec :" << difficultyToString(diff);
    //if (diff <= unlockedDifficulty_) {
        currentDifficulty_ = diff;

        // Réinitialisation complète des paramètres
        tab_obstacle.clear();
        tab_perks.clear();
        roadLines_.clear();
        score_ = 0;
        vRoute_ = 1.0;
        dVoiture_ = 0;


        // Configuration des paramètres selon la difficulté
        switch (diff) {
        case EASY:
            multObstacles_ = 1.5;
            multPerks_ = 0.5;
            aRoute_ = 10.0;
            break;

        case MEDIUM:
            multObstacles_ = 2.25;
            multPerks_ = 0.3;
            aRoute_ = 12.0;
            break;

        case HARD:
            multObstacles_ = 3;
            multPerks_ = 0.1;
            aRoute_ = 14.0;
            break;
        }

        // Réinitialisation de la voiture
        c.setPos(50, 75);

}

Game::Difficulty Game::currentDifficulty() const {
    return currentDifficulty_;
}

Game::Difficulty Game::unlockedDifficulty() const {
    return unlockedDifficulty_;
}

QString Game::difficultyToString(Difficulty diff) {
    switch (diff) {
    case EASY: return "Facile";
    case MEDIUM: return "Moyen";
    case HARD: return "Difficile";
    default: return "Inconnu";
    }
}

float Game::vRoute() const { return vRoute_; }

float Game::aRoute() const { return aRoute_; }

int Game::dVoiture() const { return dVoiture_; }

void Game::addaRoute(float val) { aRoute_ += val; }

void Game::addvRoute(float val) { vRoute_ += val; }

void Game::adddVoiture(int val) { dVoiture_ += val; }

void Game::setaRoute(float val) { aRoute_ = val; }

void Game::setvRoute(float val) { vRoute_ = val; }

void Game::setdVoiture(int val) { dVoiture_ = val; }

unsigned int Game::score() const { return score_; }

void Game::addScore(unsigned int val) { score_ += val; }

void Game::setScore(unsigned int val) { score_ = val; }

short Game::leftLimWin() const { return leftLimWin_; }

short Game::rightLimWin() const { return rightLimWin_; }

short Game::terrainWidth() const { return terrainWidth_; }

short Game::terrainHeight() const { return terrainHeight_; }

int Game::consoleWidth() const { return consoleWidth_; }

int Game::consoleHeight() const { return consoleHeight_; }

unsigned int Game::screenX() const { return screenX_; }

unsigned int Game::screenY() const { return screenY_; }

bool Game::checkCollision() {
    if (!voidCollisions) {
        for (int i = 0; i < int(tab_obstacle.size()); i++) {
            if ((c.h.pos.x() < tab_obstacle[i].h.pos.x() + tab_obstacle[i].h.w()) &&
                (c.h.pos.x() + c.h.w() > tab_obstacle[i].h.pos.x()) &&
                (c.h.pos.y() < tab_obstacle[i].h.pos.y() + tab_obstacle[i].h.l()) &&
                (c.h.pos.y() + c.h.l() > tab_obstacle[i].h.pos.y())) {

                return true;
            }
        }
    }
    for (int i = 0; i < int(tab_perks.size()); i++) {
        if ((c.h.pos.x() < tab_perks[i].h.pos.x() + tab_perks[i].h.w()) &&
            (c.h.pos.x() + c.h.w() > tab_perks[i].h.pos.x()) &&
            (c.h.pos.y() < tab_perks[i].h.pos.y() + tab_perks[i].h.l()) &&
            (c.h.pos.y() + c.h.l() > tab_perks[i].h.pos.y())) {

            unsigned int perkType = tab_perks[i].type();
            event_->callToEvent(static_cast<int>(perkType), voidCollisions);
            tab_perks.erase(tab_perks.begin() + i);

            QString soundPath = QString("data/sfx/perk%1.mp3").arg(perkType);

            if (QFile::exists(soundPath)) {
                if (sfxPlayer && sfxAudioOutput) {

                    sfxPlayer->stop();
                    sfxPlayer->setSource(QUrl::fromLocalFile(soundPath));

                    sfxPlayer->play();
                } else {
                    qWarning() << "sfxPlayer ou sfxAudioOutput non initialisé !";
                }
            } else {
                qWarning() << "Fichier son non trouvé -" << soundPath;
            }
        }
    }
    return false;
}

void Game::setEffectsVolume(float volume) {
    // Assure que le volume reste dans les limites [0.0, 1.0]
    float clampedVolume = std::max(0.0f, std::min(1.0f, volume));
    if (sfxAudioOutput) {
        sfxAudioOutput->setVolume(clampedVolume);
        qDebug() << "Game::setEffectsVolume - Volume SFX réglé à:" << clampedVolume; // Debug
    }
}

void Game::generateRoadLines() {
    int lineGirth = 20;
    int distTraits = 150;
    int posLigne = (rightLimWin_ - leftLimWin_) / 3;

    roadLines_.clear();

    for (int i = posLigne * 2; i < posLigne * 4; i += posLigne) {
        for (int j = -distTraits; j <= (int)screenY_; j += distTraits) {
            RoadLine line;
            line.x = i;
            line.y = j + vRoute_;
            line.width = lineGirth;
            line.height = 50;
            roadLines_.push_back(line);
        }
    }
}

std::vector<RoadLine> &Game::roadLines() { return roadLines_; }

void Game::environment() {
    if (score_ >= 5000) {
        unlockedDifficulty_ = HARD;
    } else if (score_ >= 1000) {
        unlockedDifficulty_ = MEDIUM;
    }

    if (vRoute_ >= 150)
        vRoute_ -= 150 - aRoute_ * event_->multSpeed();
    else
        vRoute_ += aRoute_ * event_->multSpeed();
    if (Obstacle::tryObstacle(multObstacles_)) {
        Obstacle::genereObstacle(tab_obstacle, tab_perks, multObstacles_);
    }   Obstacle::updateObstacles(tab_obstacle, aRoute_, event_->multSpeed(), terrainHeight_, screenY_);

    if (Perks::tryPerk(multPerks_)) {
        Perks::generePerk(tab_obstacle, tab_perks, multPerks_);
    }   Perks::updatePerks(tab_perks, aRoute_, event_->multSpeed(), terrainHeight_, screenY_);

    generateRoadLines();

    if ((event_->isThereAnEvent()!=0) && (((event_->elapsedEvent.remainingTimeAsDuration()).count()) <= 400))
    {
        event_->elapsedEvent.stop();
        event_->setEventTo(0);
        event_->setMultSpeed(1);
        event_->setMultScore(1);
        voidCollisions = false;
    }
}

void Game::move(short val) {
    short int isPositive = (short int)(val >= 0);
    if (0 >= c.h.pos.x()) {
        if (isPositive)
            c.h.pos.setX(val + c.h.pos.x());

    } else if (c.h.pos.x() >= terrainWidth_-1) {
        if (!isPositive)
            c.h.pos.setX(val + c.h.pos.x());

    } else {
        c.h.pos.setX(val + c.h.pos.x());
    }
}

void Game::autoBest() {
    if (score_ > bestScore_)
        bestScore_ = score_;
}

void Game::setBestScore(unsigned int bs) { bestScore_ = bs; }

QJsonObject Game::toJson() const {
    QJsonObject data;
    data["bestScore"] = static_cast<int>(bestScore_);
    data["leftK"] = static_cast<double>(leftKey);
    data["rightK"] = static_cast<double>(rightKey);
    data["difficulty"] = static_cast<int>(currentDifficulty_);
    return data;
}

void Game::fromJson(const QJsonObject &data) {
    bestScore_ = data["bestScore"].toInt();
    leftKey = data["leftK"].toDouble();
    rightKey = data["rightK"].toDouble();
    if (data.contains("difficulty"))
        currentDifficulty_ = static_cast<Difficulty>(data["difficulty"].toInt());
}

unsigned int Game::bestScore() const { return bestScore_; }

void Game::saveGame(int slot) {
    QJsonObject gameData = toJson();
    if (slot == 0)
        saveManager->autoSave(gameData);
    else
        saveManager->manualSave(slot, gameData);
}

void Game::loadGame(int slot) {
    QJsonObject gameData;

    if (slot == 0) {
        gameData = saveManager->loadLastAutoSave();
    } else {
        gameData = saveManager->loadManualSave(slot);
    }

    if (!gameData.isEmpty()) {
        fromJson(gameData);
    }
}

void Game::toggleMusic() {
    if (i.isPlaying()) {
        if (!musicPlaying) {
            mediaPlayer->play();
            musicPlaying = true;
        }
    } else {
        if (musicPlaying) {
            mediaPlayer->pause();
            musicPlaying = false;
        }
    }
}
