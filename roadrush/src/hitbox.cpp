#include "head/hitbox.h"

Hitbox::Hitbox(Position p1, unsigned int wa, unsigned int la)
{
    pos = p1;
    w_ = wa;
    l_ = la;
}

Hitbox::Hitbox(Position p1, unsigned int s)
{
    pos = p1;
    w_ = s;
    l_ = s;
}

Hitbox::Hitbox()
{
}

Hitbox::~Hitbox()
{
}

unsigned int Hitbox::w() const
{
    return w_;
}

unsigned int Hitbox::l() const
{
    return l_;
}

void Hitbox::setL(unsigned int la)
{
    l_ = la;
}

void Hitbox::setW(unsigned int wa)
{
    w_ = wa;
}

void Hitbox::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Hitbox =====";

    Position p(5.0f, 10.0f);
    Hitbox h1(p, 50, 100);
    Q_ASSERT(h1.l() == 50);
    Q_ASSERT(h1.w() == 100);
    qDebug() << "✓ Constructeur avec l et w OK";

    h1.setL(70);
    h1.setW(120);
    Q_ASSERT(h1.l() == 70);
    Q_ASSERT(h1.w() == 120);
    qDebug() << "✓ Setters OK";

    Hitbox h2(p, 40);
    Q_ASSERT(h2.l() == 40 && h2.w() == 40);
    qDebug() << "✓ Constructeur carré OK";

    qDebug() << "===== FIN testRegression: Hitbox =====";
}
