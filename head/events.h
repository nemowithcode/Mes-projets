#ifndef EVENTS_H
#define EVENTS_H

#include "qelapsedtimer.h"
#include "qtimer.h"

#include <QDebug>
#include <cassert>

/**
 * @enum class EventType
 * @brief Énumération représentant les différents types d'événements possibles dans le jeu.
 */
enum class EventType {
    ACCIDENT = 1,      ///< Événement d'accident.
    NUIT = 2,          ///< Événement de changement de luminosité (nuit).
    TURBO = 3          ///< Événement d'obtention de perk.
};

/**
 * @class Event
 * @brief Classe gérant les événements du jeu.
 */
class Event {
public:
    /**
     * @brief Constructeur de la classe Event.
     */
    Event();

    /**
     * @brief Destructeur de la classe Event.
     */
    ~Event();

    /**
     * @brief Vérifie s'il y a un événement en cours et retourne son type.
     * @return 0 si aucun événement, 1-n si un événement est en cours.
     */
    short int isThereAnEvent() const;

    /**
     * @brief Appelle l'événement correspondant au type donné.
     * @param eventType Le type de l'événement à appeler (short int).
     */
    void callToEvent(short int eventType, bool &voidCollision);

    /**
     * @brief Obtient le multiplicateur de vitesse actuel.
     * @return Le multiplicateur de vitesse.
     */
    float multSpeed() const;

    /**
     * @brief Obtient le multiplicateur de score actuel.
     * @return Le multiplicateur de score.
     */
    float multScore() const;

    /**
     * @brief Définit un nouveau multiplicateur de vitesse.
     * @param va La nouvelle valeur du multiplicateur de vitesse.
     */
    void setMultSpeed(float va);

    /**
     * @brief Définit un nouveau multiplicateur de score.
     * @param va La nouvelle valeur du multiplicateur de score.
     */
    void setMultScore(float va);

    /**
     * @brief Définit manuellement le type d'événement en cours.
     * @param va La valeur du type d'événement (short int).
     */
    void setEventTo(short int va);

    /**
     * @brief Chronomètre pour mesurer la durée de l'événement.
     */
    QTimer elapsedEvent;

    /**
     * @brief Effectue un test de régression unitaire sur la classe Events.
     */
    static void testRegression();

private:
    /**
     * @brief Type de l'événement actuel.
     */
    EventType typeE;

    /**
     * @brief Indique si un événement est en cours (0 = aucun, 1-n = événement actif).
     */
    short int isThereAnEvent_ = 0;

    /**
     * @brief Multiplicateur de vitesse appliqué pendant l'événement.
     */
    float multSpeed_ = 1;

    /**
     * @brief Multiplicateur de score appliqué pendant l'événement.
     */
    float multScore_ = 1;
};

#endif // EVENTS_H
