#include "head/savemanager.h"
#include <QCoreApplication>
#include <QKeySequence>

SaveManager::SaveManager(QObject *parent) : QObject(parent) {
    QDir().mkpath("data/saves/autosave");
    QDir().mkpath("data/saves/manual");
}

void SaveManager::autoSave(const QJsonObject& gameData) {
    // Rotation autosaves
    QFile::remove("data/saves/autosave/autosave2.json");
    QFile::rename("data/saves/autosave/autosave1.json", "data/saves/autosave/autosave2.json");

    writeJsonFile("data/saves/autosave/autosave1.json", gameData);
}

void SaveManager::manualSave(int slot, const QJsonObject& gameData) {
    if (slot < 1 || slot > 3) return;
    writeJsonFile(QString("data/saves/manual/slot%1.json").arg(slot), gameData);
}

QJsonObject SaveManager::loadLastAutoSave() const {
    return readJsonFile("data/saves/autosave/autosave1.json");
}

QJsonObject SaveManager::loadManualSave(int slot) const {
    if (slot < 1 || slot > 3) return QJsonObject();
    return readJsonFile(QString("data/saves/manual/slot%1.json").arg(slot));
}

bool SaveManager::writeJsonFile(const QString& path, const QJsonObject& data) {
    QFileInfo fi(path);
    QDir().mkpath(fi.absolutePath()); // Crée le dossier si inexistant

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(QJsonDocument(data).toJson());
    file.close();
    return true;
}

QJsonObject SaveManager::readJsonFile(const QString& path) const {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return QJsonObject();

    return QJsonDocument::fromJson(file.readAll()).object();
}

QString SaveManager::getAutoSavePath() const {
    // Crée un dossier 'saves' à côté de l'exécutable
    QString path = "data/saves/autosave/autosave1.json";
    QDir().mkpath(QFileInfo(path).absolutePath()); // Crée l'arborescence
    return path;
}

QString SaveManager::getManualSavePath(int slot) const {
    return QString("data/saves/manual/slot%1.json").arg(slot);
}

QStringList SaveManager::getManualSaveInfo(int slot) const {
    if (slot < 1 || slot > 3) return QStringList();

    QJsonObject saveData = readJsonFile(QString("data/saves/manual/slot%1.json").arg(slot));
    if (saveData.isEmpty()) {
        return QStringList() << "Slot libre";
    }

    QStringList info;
    info << QString("Meilleur score: %1").arg(saveData["bestScore"].toInt());
    info << QString("Touches: Gauche(%1), Droite(%2)")
                .arg(QKeySequence(saveData["leftKey"].toInt()).toString(),
                     QKeySequence(saveData["rightKey"].toInt()).toString());
    return info;
}

bool SaveManager::manualSaveExists(int slot) const {
    if (slot < 1 || slot > 3) return false;
    return QFile::exists(QString("data/saves/manual/slot%1.json").arg(slot));
}

void SaveManager::deleteManualSave(int slot) {
    if (slot < 1 || slot > 3) return;
    QFile::remove(QString("data/saves/manual/slot%1.json").arg(slot));
}

void SaveManager::cleanOldAutoSaves(int maxFiles) {
    for (int i = maxFiles; i >= 1; --i) {
        QString currentFile = QString("data/saves/autosave/autosave%1.json").arg(i);
        QString nextFile = QString("data/saves/autosave/autosave%1.json").arg(i + 1);

        if (QFile::exists(currentFile)) {
            QFile::remove(nextFile);
            QFile::rename(currentFile, nextFile);
        }
    }
}

void SaveManager::testRegression() {
    qDebug() << "===== DÉBUT testRegression: SaveManager =====";

    // Créer une instance de SaveManager pour appeler les méthodes non statiques
    SaveManager manager;

    QJsonObject dummyData;
    dummyData["score"] = 12345;
    dummyData["player"] = "TestPlayer";
    dummyData["difficulty"] = "HARD";

    manager.autoSave(dummyData);
    QJsonObject loadedAuto = manager.loadLastAutoSave();
    Q_ASSERT(loadedAuto["score"] == 12345);
    Q_ASSERT(loadedAuto["player"] == "TestPlayer");
    Q_ASSERT(loadedAuto["difficulty"] == "HARD");
    qDebug() << "✓ Autosave et lecture OK";

    int testSlot = 1;
    manager.manualSave(testSlot, dummyData);
    Q_ASSERT(manager.manualSaveExists(testSlot));
    QJsonObject loadedManual = manager.loadManualSave(testSlot);
    Q_ASSERT(loadedManual["score"] == 12345);
    Q_ASSERT(loadedManual["player"] == "TestPlayer");
    qDebug() << "✓ Sauvegarde manuelle et lecture OK";

    QStringList info = manager.getManualSaveInfo(testSlot);
    Q_ASSERT(!info.isEmpty());
    qDebug() << "✓ Infos sauvegarde OK : " << info;

    manager.deleteManualSave(testSlot);
    Q_ASSERT(!manager.manualSaveExists(testSlot));
    qDebug() << "✓ Suppression sauvegarde OK";

    manager.cleanOldAutoSaves();
    qDebug() << "✓ Nettoyage auto OK";

    qDebug() << "===== FIN testRegression: SaveManager =====";
}
