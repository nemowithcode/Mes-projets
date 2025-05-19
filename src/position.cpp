#include "head/position.h"

Position::Position(float xa, float ya) {
    setX(xa);
    setY(ya);
}

Position::Position(const Position& other) {
    x_ = other.x_;
    y_ = other.y_;
}

Position& Position::operator=(const Position& other) {
    if (this != &other) { // Vérifier l'auto-affectation
        x_ = other.x_;
        y_ = other.y_;
    }
    return *this;
}

Position::Position() {}

Position::~Position() {}

float Position::x() const {
    return x_;
}

float Position::y() const {
    return y_;
}

void Position::setX(float xa) {
    x_ = xa;
}

void Position::setY(float ya) {
    y_ = ya;
}

void Position::setPos(float xa, float ya) {
    setX(xa);
    setY(ya);
}

void Position::setPos(Position p) {
    setX(p.x());
    setY(p.y());
}

void Position::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Position =====";

    // Test constructeur avec paramètres
    Position p1(10.0f, 20.0f);
    Q_ASSERT(p1.x() == 10.0f);
    Q_ASSERT(p1.y() == 20.0f);
    qDebug() << "✓ Constructeur avec paramètres OK";

    // Test constructeur par défaut
    Position p2;
    p2.setPos(5.5f, 7.7f);
    Q_ASSERT(p2.x() == 5.5f);
    Q_ASSERT(p2.y() == 7.7f);
    qDebug() << "✓ Constructeur par défaut et setPos OK";

    // Test setX / setY
    p2.setX(1.0f);
    p2.setY(2.0f);
    Q_ASSERT(p2.x() == 1.0f);
    Q_ASSERT(p2.y() == 2.0f);
    qDebug() << "✓ setX / setY OK";

    // Test setPos avec Position
    p1.setPos(p2);
    Q_ASSERT(p1.x() == 1.0f && p1.y() == 2.0f);
    qDebug() << "✓ setPos(Position) OK";

    // Test opérateur d'affectation
    Position p3;
    p3 = p1;
    Q_ASSERT(p3.x() == 1.0f && p3.y() == 2.0f);
    qDebug() << "✓ Opérateur d'affectation OK";

    // Test constructeur de copie
    Position p4(p3);
    Q_ASSERT(p4.x() == 1.0f && p4.y() == 2.0f);
    qDebug() << "✓ Constructeur de copie OK";

    qDebug() << "===== FIN testRegression: Position =====";
}
