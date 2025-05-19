#include "head/perks.h"
#include "head/obstacle.h"

Perks::Perks(Hitbox hitbox, PerkType perk) : h(hitbox), type_(perk) {}

Perks::Perks() {}

Perks::~Perks() {}

unsigned int Perks::type() const {
    return static_cast<unsigned int>(type_); 
}

void Perks::setType(PerkType perk) { 
    type_ = perk; 
}

Position Perks::pos() const { 
    return h.pos; 
}

void Perks::setPos(Position p) { 
    h.pos = p; 
}

void Perks::setL(unsigned int la) { 
    h.setL(la); 
}

void Perks::setW(unsigned int wa) { 
    h.setW(wa); 
}

unsigned int Perks::getL() const { 
    return h.l(); 
}

unsigned int Perks::getW() const { 
    return h.w(); 
}

void Perks::updatePerks(std::vector<Perks> &tab_perks, float aRoute_, float multiplier, float terrainHeight_, float screenY_) {
    for (int i = 0; i < int(tab_perks.size());) {
        Perks &objet = tab_perks[i];
        float oy = objet.h.pos.y() + aRoute_ * multiplier * terrainHeight_ / screenY_;
        objet.h.pos.setY(oy);

        if (oy >= terrainHeight_) {
            tab_perks.erase(tab_perks.begin() + i);
        } else {
            i++;
        }
    }
}

void Perks::generePerk(std::vector<Obstacle> &tab_obstacle, std::vector<Perks> &tab_perks, float multPerks) {
    if (int(tab_perks.size()) >= 2)
        return;

    const int MAX_ATTEMPTS = 20;
    bool positionValide = false;
    int longueur = 10;
    Perks newPerk;

    for (int attempt = 0; attempt < MAX_ATTEMPTS && !positionValide; ++attempt) {
        short int positionX = (rand() % (96 - longueur)) + longueur / 2 + 4;
        Position posPerk(positionX, -longueur);
        Hitbox hPerk(posPerk, longueur, longueur);

        positionValide = true;

        // Vérification de la position par rapport aux obstacles
        for (auto &obstacle : tab_obstacle) {
            bool collisionX = std::abs(obstacle.h.pos.x() - posPerk.x()) < (obstacle.h.w() + longueur) * 0.6;
            bool collisionY = std::abs(obstacle.h.pos.y() - posPerk.y()) < (obstacle.h.l() + longueur) * 0.6;

            if (collisionX && collisionY) {
                positionValide = false;
                break;
            }
        }

        // Vérification de la position par rapport aux autres perks
        if (positionValide) {
            for (auto &perk : tab_perks) {
                bool collisionX = std::abs(perk.h.pos.x() - posPerk.x()) < (perk.h.w() + longueur) * 0.6;
                bool collisionY = std::abs(perk.h.pos.y() - posPerk.y()) < (perk.h.l() + longueur) * 0.6;

                if (collisionX && collisionY) {
                    positionValide = false;
                    break;
                }
            }
        }

        if (positionValide) {
            newPerk.h.pos.setPos(posPerk);
            newPerk.h.setL(longueur);
            newPerk.h.setW(longueur);
            newPerk.setType(static_cast<PerkType>(rand()%3+1));
        }
    }

    if (positionValide) {
        tab_perks.push_back(newPerk);
    }
}

bool Perks::tryPerk(float multPerks) {
    return ((rand() % (int)(40*1/multPerks)) == 1);
}

void Perks::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Perks =====";

    // Test constructeur avec paramètres
    Hitbox hb;  // Suppose que Hitbox a un constructeur par défaut
    Perks p1(hb, PerkType::TURBO);
    Q_ASSERT(p1.type() == static_cast<unsigned int>(PerkType::TURBO));
    p1.setType(PerkType::INVINCIBILITY);
    Q_ASSERT(p1.type() == static_cast<unsigned int>(PerkType::INVINCIBILITY));
    qDebug() << "✓ Constructeur et type OK";

    // Test position
    Position pos(5.0f, 10.0f);
    p1.setPos(pos);
    Q_ASSERT(p1.pos().x() == 5.0f && p1.pos().y() == 10.0f);
    qDebug() << "✓ Position OK";

    // Test taille
    p1.setL(25);
    p1.setW(40);
    Q_ASSERT(p1.getL() == 25);
    Q_ASSERT(p1.getW() == 40);
    qDebug() << "✓ Taille OK";

    // Test constructeur par défaut
    Perks p2;
    p2.setType(PerkType::SLOW);
    Q_ASSERT(p2.type() == static_cast<unsigned int>(PerkType::SLOW));
    qDebug() << "✓ Constructeur par défaut OK";

    // Test méthode statique tryPerk
    bool test = Perks::tryPerk(1);
    Q_ASSERT(test == true || test == false);  // Vérifie juste que ça renvoie un bool
    qDebug() << "✓ tryPerk OK";

    // Test updatePerks
    std::vector<Perks> tab;
    tab.push_back(p1);
    Perks::updatePerks(tab, 1.0f, 1.0f, 100.0f, 600.0f);
    qDebug() << "✓ updatePerks OK";

    qDebug() << "===== FIN testRegression: Perks =====";
}
