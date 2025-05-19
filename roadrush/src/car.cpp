#include "head/car.h"


Car::Car(){
    Position pa(50, 75);
    Hitbox ha(pa,5,10);
    h = ha;
    skin_ = static_cast<Couleur>(1);
    canMove_ = true;
}

Car::Car(float px, float py) {
    canMove_ = true;
    skin_ = static_cast<Couleur>(1);
    Position pa(px, py);
    Hitbox ha(pa,5,10);
    h = ha;
}

Car& Car::operator=(const Car& other) {
    if (this != &other) { // Vérifier l'auto-affectation
        h = other.h;
        canMove_ = other.canMove_;
        skin_ = other.skin_;
    }
    return *this;
}

Car::Couleur Car::skin() const { return skin_ ;}

Position Car::getPos() const {
    return h.pos;
}

void Car::setPos(float xa, float ya) {
    h.pos.setPos(xa,ya);
}


Car::~Car(){

}


void Car::move(float val)
{
    bool isPositive = (val>=0);
    val*=2;
    if (265>h.pos.x()) {
        switch (isPositive)
        {
        case 1:
            h.pos.setX(val+getPos().x());
            break;
        }
    } else if (h.pos.x()>1205) {
        switch (isPositive)
        {
        case 0:
            h.pos.setX(val+h.pos.x());
            break;
        }
    } else {
        h.pos.setX(val+h.pos.x());
    }
}

void Car::setSkin(Couleur newSkin) { skin_ = newSkin; }

void Car::setFree(){
    canMove_ = true;
    
}

bool Car::canMove() const {
    return canMove_;
}

void Car::restrain() {
    canMove_ = false;
}

void Car::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Car =====";

    Car c1;
    c1.setPos(10.0f, 20.0f);
    Q_ASSERT(c1.getPos().x() == 10.0f && c1.getPos().y() == 20.0f);
    qDebug() << "✓ Position OK";

    c1.setSkin(Car::BLUE);
    Q_ASSERT(c1.skin() == Car::BLUE);
    qDebug() << "✓ Skin OK";

    c1.restrain();
    Q_ASSERT(!c1.canMove());
    c1.setFree();
    Q_ASSERT(c1.canMove());
    qDebug() << "✓ Déplacement activé/désactivé OK";

    Car c2 = c1;
    Q_ASSERT(c2.getPos().x() == 10.0f);
    qDebug() << "✓ Affectation OK";

    qDebug() << "===== FIN testRegression: Car =====";
}
