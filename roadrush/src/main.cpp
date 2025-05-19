#include "head/mainwindow.h"
#include "head/maintextuel.h"

#include "qboxlayout.h"
#include "qdialog.h"

#include <QScreen>
#include <QApplication>
#include <QTimer>
using namespace std;

/**
 * @brief Point d'entrée de l'application.
 * @param argc Nombre d'arguments en ligne de commande.
 * @param argv Tableau des arguments en ligne de commande.
 * @return Code de retour de l'application.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    bool choixAffichage = true;

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect availableGeometry = screen->availableGeometry();
    short int screenX = availableGeometry.width();
    short int screenY = availableGeometry.height();
    MainWindow w(nullptr,screenX,screenY);

    if (choixAffichage) {


            w.setWindowTitle("ROADRUSH : Version graphique");
            w.setGeometry(0, screenY/2, screenX, screenY/2);
            w.showMaximized();

    } else {

            MainTextuel *t = new MainTextuel(nullptr,screenX,screenY);

            QDialog *consoleWindow = new QDialog();
            consoleWindow->setWindowTitle("ROADRUSH : Version textuelle");
            consoleWindow->setGeometry(screenX/4, screenY/4, screenX/2, screenY/2);

            QVBoxLayout *layout = new QVBoxLayout(consoleWindow);
            layout->addWidget(t);
            consoleWindow->setLayout(layout);

            consoleWindow->show();

    }

    return a.exec();
}
