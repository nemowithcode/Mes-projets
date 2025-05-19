#include "head/interface.h"

Interface::Interface()
{
    hideAll();
    showMain();
    // showGame();
}

bool Interface::isOnMain() const
{
    return isOnMain_;
}

bool Interface::isOnSettings() const
{
    return isOnSettings_;
}

bool Interface::isOnCustom() const
{
    return isOnCustom_;
}

bool Interface::isOnPause() const
{
    return isOnPause_;
}

bool Interface::isOnModes() const
{
    return isOnModes_;
}

bool Interface::isPlaying() const
{
    return isPlaying_;
}

bool Interface::hasLost() const
{
    return hasLost_;
}

bool Interface::isPauseMenuShown() const
{
    return isPauseMenuShown_;
}

void Interface::showMain()
{
    hideAll();
    isOnMain_ = true;
}

void Interface::showSettings()
{
    hideAll();
    isOnSettings_ = true;
}

void Interface::showCustoms()
{
    hideAll();
    isOnCustom_ = true;
}

void Interface::showPause()
{
    hideAll();
    isOnPause_ = true;
}

void Interface::showModes()
{
    hideAll();
    isOnModes_ = true;
}

void Interface::showGame()
{
    hideAll();
    isPlaying_ = true;
}

void Interface::showLoss()
{
    hideAll();
    hasLost_ = true;
}

void Interface::showPauseMenu()
{
    isPauseMenuShown_ = true;
    isOnPause_ = true;
    isPlaying_ = false;
}

void Interface::hidePauseMenu()
{
    isPauseMenuShown_ = false;
    isOnPause_ = false;
    isPlaying_ = true;
}

void Interface::hideAll()
{
    isPlaying_ = false;
    isOnMain_ = false;
    isOnSettings_ = false;
    isOnCustom_ = false;
    isOnPause_ = false;
    isOnModes_ = false;
    isPauseMenuShown_ = false;
    hasLost_ = false;
}

void Interface::testRegression() {
    qDebug() << "===== DÉBUT testRegression: Interface =====";

    Interface ui;
    ui.showGame();
    Q_ASSERT(ui.isPlaying());

    ui.showLoss();
    Q_ASSERT(ui.hasLost());

    ui.showPauseMenu();
    Q_ASSERT(ui.isPauseMenuShown());

    ui.hidePauseMenu();
    Q_ASSERT(!ui.isPauseMenuShown());

    ui.showMain();
    Q_ASSERT(ui.isOnMain());

    ui.showSettings();
    Q_ASSERT(ui.isOnSettings());

    qDebug() << "✓ États d'interface OK";

    qDebug() << "===== FIN testRegression: Interface =====";
}
