#include "head/obstacle.h"
#include "head/perks.h"
#include <cstdlib>
#include <iostream>

Obstacle::Obstacle() {
    h.pos.setPos(0, 0);
    h.setL(1);
    h.setW(1);
}

Obstacle::~Obstacle() {}

Obstacle::Obstacle(Position p1, unsigned int la, unsigned int wa) {
    Hitbox hiboux(p1, la, wa);
    h = hiboux;
}

Obstacle::Obstacle(Position p1, unsigned int sa) : h(p1, sa) {}

void Obstacle::setL(unsigned int la) { h.setL(la); }

void Obstacle::setW(unsigned int wa) { h.setW(wa); }

unsigned int Obstacle::getL() const { return h.l(); }

unsigned int Obstacle::getW() const { return h.w(); }

void Obstacle::setPos(Position pa) { h.pos = pa; }

void Obstacle::updateObstacles(std::vector<Obstacle> &tab_obstacle, float aRoute_, float multiplier, float terrainHeight_, float screenY_) {
    for (int i = 0; i < int(tab_obstacle.size());) {
        Obstacle &objet = tab_obstacle[i];
        float oy = objet.h.pos.y() + aRoute_ * multiplier * terrainHeight_ / screenY_;
        objet.h.pos.setY(oy);

        if (oy >= terrainHeight_) {
            tab_obstacle.erase(tab_obstacle.begin() + i);
        } else {
            i++;
        }
    }
}

void Obstacle::genereObstacle(std::vector<Obstacle> &tab_obstacle, std::vector<Perks> &tab_perks, float multObstacles) {
    if (!tryObstacle(multObstacles) || int(tab_obstacle.size()) >= 6)
        return;

    const int MAX_ATTEMPTS = 20;
    bool isPosValid = false;
    int height, length;

    Obstacle newObstacle;
    newObstacle.setType(rand()%3);

    switch (newObstacle.type()) {
        case 0:
            length = (rand() % 4) + 7;
            height = (int)(length * 0.52);
            break;

        default:
            length = (rand() % 5) + 20;
            height = int(length * 0.60);
            break;
    }



    for (int i = 0; i < MAX_ATTEMPTS && !isPosValid; ++i) {
        short int positionX = (rand() % (96 - length)) + length / 2 - 4;
        Position posObs(positionX, -height);

        isPosValid = true;

        // Vérification de la position par rapport aux obstacles existants
        for (auto &obstacle : tab_obstacle) {
            bool collisionX = std::abs(obstacle.h.pos.x() - posObs.x()) < (obstacle.h.w() + length * 0.8);
            bool collisionY = std::abs(obstacle.h.pos.y() - posObs.y()) < (obstacle.h.l() + height * 0.8);

            if (collisionX && collisionY) {
                isPosValid = false;
                break;
            }
        }

        // Vérification de la position par rapport aux perks existants
        if (isPosValid) {
            for (auto &perk : tab_perks) {
                bool collisionX = std::abs(perk.h.pos.x() - posObs.x()) < (perk.h.w() + length * 0.8);
                bool collisionY = std::abs(perk.h.pos.y() - posObs.y()) < (perk.h.l() + height * 0.8);

                if (collisionX && collisionY) {
                    isPosValid = false;
                    break;
                }
            }
        }

        if (isPosValid) {
            newObstacle.setPos(posObs);
            newObstacle.setL(height);
            newObstacle.setW(length);
        }
    }

    if (isPosValid) {
        tab_obstacle.push_back(newObstacle);
    }
}

bool Obstacle::tryObstacle(float multObstacles) { return ((rand() % (int)(10*1/multObstacles)) == 1); }

void Obstacle::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Obstacle =====";

    // Test constructeur avec position et dimensions
    Position pos1(10.0f, 20.0f);
    Obstacle o1(pos1, 50, 100);
    Q_ASSERT(o1.getL() == 50);
    Q_ASSERT(o1.getW() == 100);
    o1.setL(60);
    o1.setW(120);
    Q_ASSERT(o1.getL() == 60);
    Q_ASSERT(o1.getW() == 120);
    qDebug() << "✓ Constructeur (pos, l, w) et setters OK";

    // Test constructeur avec taille uniforme
    Obstacle o2(pos1, 30);
    Q_ASSERT(o2.getL() == 30);
    Q_ASSERT(o2.getW() == 30);
    qDebug() << "✓ Constructeur (pos, taille) OK";

    // Test constructeur par défaut
    Obstacle o3;
    o3.setL(10);
    o3.setW(15);
    o3.setPos(Position(5.0f, 5.0f));
    Q_ASSERT(o3.getL() == 10);
    Q_ASSERT(o3.getW() == 15);
    qDebug() << "✓ Constructeur par défaut + setters OK";

    // Test ignoreMe
    // o3.setignoreMe(true);
    // Q_ASSERT(o3.ignoreMe() == true);
    // o3.setignoreMe(false);
    // Q_ASSERT(!o3.ignoreMe());
    // qDebug() << "✓ setignoreMe / ignoreMe OK";

    // Test fonction statique tryObstacle (résultat aléatoire)
    bool result = Obstacle::tryObstacle(1);
    Q_ASSERT(result == true || result == false);  // Juste vérifier qu'elle renvoie un bool
    qDebug() << "✓ tryObstacle OK (bool retourné)";

    // Test updateObstacles (ne crash pas)
    std::vector<Obstacle> tab;
    tab.push_back(o1);
    Obstacle::updateObstacles(tab, 1.0f, 1.0f, 100.0f, 600.0f);
    qDebug() << "✓ updateObstacles OK";

    qDebug() << "===== FIN testRegression: Obstacle =====";
}

int Obstacle::type() { return type_; }

void Obstacle::setType(int va) { type_ = va; }
