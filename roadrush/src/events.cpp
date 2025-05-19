#include "head/events.h"

Event::Event() {}

Event::~Event() {}

short int Event::isThereAnEvent() const {
    return isThereAnEvent_;
}

void Event::callToEvent(short int eventType, bool &voidCollision_) {
    int chronomax = elapsedEvent.interval();
    int chronorest = elapsedEvent.remainingTime();
    switch (eventType) {
    case 1:

        if(isThereAnEvent_ == eventType) {
            elapsedEvent.start(chronorest + 6000);
            multSpeed_ *= 0.75;
            multScore_ *= 0.75;
        } else {
            elapsedEvent.start(6000);
            multSpeed_ = 0.75;
            multScore_ = 0.75;
        }

        voidCollision_ = false;
        isThereAnEvent_ = eventType;
        break;

    case 2:

        if(isThereAnEvent_ == eventType) {
            elapsedEvent.start(chronorest + 5000);
        } else elapsedEvent.start(5000);

        voidCollision_ = true;
        multSpeed_ = 1;
        multScore_ = 1;

        isThereAnEvent_ = eventType;
        break;

    case 3:

        if(isThereAnEvent_ == eventType) {
            elapsedEvent.start(chronorest + 8000);
            multSpeed_ *= 1.25;
            multScore_ *= 1.25;
        } else {
            elapsedEvent.start(8000);
            multSpeed_ = 1.25;
            multScore_ = 1.25;
        }

        voidCollision_ = false;
        isThereAnEvent_ = eventType;
        break;

    default:
        break;
    }
}

float Event::multSpeed() const {
    return multSpeed_;
}

float Event::multScore() const {
    return multScore_;
}

void Event::setMultSpeed(float va) {
    multSpeed_ = va;
}

void Event::setMultScore(float va) {
    multScore_ = va;
}

void Event::setEventTo(short va) {
    isThereAnEvent_ = va;
}

void Event::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Event =====";

    Event e;
    Q_ASSERT(e.isThereAnEvent() == 0);
    bool boole = true;
    e.callToEvent(1, boole);
    Q_ASSERT(e.isThereAnEvent() == 1);
    Q_ASSERT(boole == false);
    e.setMultSpeed(2.0f);
    e.setMultScore(3.0f);
    Q_ASSERT(e.multSpeed() == 2.0f);
    Q_ASSERT(e.multScore() == 3.0f);
    e.setEventTo(0);
    Q_ASSERT(e.isThereAnEvent() == 0);
    qDebug() << "✓ Événements et multiplicateurs OK";

    qDebug() << "===== FIN testRegression: Event =====";
}
