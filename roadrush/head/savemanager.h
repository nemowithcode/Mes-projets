#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QKeySequence>
#include <QDebug>

/**
 * @class SaveManager
 * @brief Classe gérant les sauvegardes automatiques et manuelles du jeu.
 */
class SaveManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructeur de la classe SaveManager.
     * @param parent Objet parent.
     */
    explicit SaveManager(QObject *parent = nullptr);

    /**
     * @brief Sauvegarde automatique des données du jeu.
     * @param gameData Données du jeu à sauvegarder.
     */
    void autoSave(const QJsonObject& gameData);

    /**
     * @brief Charge la dernière sauvegarde automatique.
     * @return Les données du jeu sous forme de QJsonObject.
     */
    QJsonObject loadLastAutoSave() const;

    /**
     * @brief Sauvegarde manuelle des données du jeu dans un slot donné.
     * @param slot Numéro du slot de sauvegarde.
     * @param gameData Données du jeu à sauvegarder.
     */
    void manualSave(int slot, const QJsonObject& gameData);

    /**
     * @brief Charge une sauvegarde manuelle depuis un slot donné.
     * @param slot Numéro du slot de sauvegarde.
     * @return Les données du jeu sous forme de QJsonObject.
     */
    QJsonObject loadManualSave(int slot) const;

    /**
     * @brief Supprime une sauvegarde manuelle dans un slot donné.
     * @param slot Numéro du slot de sauvegarde.
     */
    void deleteManualSave(int slot);

    /**
     * @brief Obtient les informations sur une sauvegarde manuelle.
     * @param slot Numéro du slot de sauvegarde.
     * @return Une liste de chaînes contenant les informations sur la sauvegarde.
     */
    QStringList getManualSaveInfo(int slot) const;

    /**
     * @brief Vérifie si une sauvegarde manuelle existe dans un slot donné.
     * @param slot Numéro du slot de sauvegarde.
     * @return True si la sauvegarde existe, false sinon.
     */
    bool manualSaveExists(int slot) const;

    /**
     * @brief Nettoie les anciennes sauvegardes automatiques, en gardant un nombre maximum de fichiers.
     * @param maxFiles Nombre maximum de fichiers à conserver.
     */
    void cleanOldAutoSaves(int maxFiles = 2);

private:
    /**
     * @brief Obtient le chemin du fichier de sauvegarde automatique.
     * @return Le chemin du fichier de sauvegarde automatique.
     */
    QString getAutoSavePath() const;

    /**
     * @brief Obtient le chemin du fichier de sauvegarde manuelle pour un slot donné.
     * @param slot Numéro du slot de sauvegarde.
     * @return Le chemin du fichier de sauvegarde manuelle.
     */
    QString getManualSavePath(int slot) const;

    /**
     * @brief Écrit un QJsonObject dans un fichier JSON.
     * @param path Chemin du fichier.
     * @param data Données à écrire.
     * @return True si l'écriture a réussi, false sinon.
     */
    bool writeJsonFile(const QString& path, const QJsonObject& data);

    /**
     * @brief Lit un fichier JSON et retourne son contenu sous forme de QJsonObject.
     * @param path Chemin du fichier.
     * @return Les données du fichier sous forme de QJsonObject.
     */
    QJsonObject readJsonFile(const QString& path) const;

    /**
     * @brief Effectue un test de régression unitaire sur la classe SaveManager.
     */
    static void testRegression();
};

#endif // SAVEMANAGER_H
