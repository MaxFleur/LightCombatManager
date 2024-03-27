#include "TableFileHandler.hpp"

#include "AdditionalInfoData.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

bool
TableFileHandler::writeToFile(
    const QVector<QVector<QVariant> >& tableData,
    const QString&                     fileName,
    unsigned int                       rowEntered,
    unsigned int                       roundCounter,
    const RuleSettings::Ruleset&       ruleset,
    bool                               rollAutomatically) const
{
    // Main combat stats
    QJsonObject lcmFile;
    lcmFile["row_entered"] = (int) rowEntered;
    lcmFile["round_counter"] = (int) roundCounter;
    lcmFile["ruleset"] = (int) ruleset;
    lcmFile["roll_automatically"] = rollAutomatically;

    QJsonObject charactersObject;
    for (auto i = 0; i < tableData.size(); i++) {
        const auto& row = tableData.at(i);
        // Character values
        QJsonObject singleCharacterObject;
        singleCharacterObject["name"] = row.at(0).toString();
        singleCharacterObject["initiative"] = row.at(1).toInt();
        singleCharacterObject["modifier"] = row.at(2).toInt();
        singleCharacterObject["hp"] = row.at(3).toInt();
        singleCharacterObject["is_enemy"] = row.at(4).toBool();

        // Additional info
        QJsonObject additionalInfoObject;
        auto addInfo = row.at(5).value<AdditionalInfoData>();
        additionalInfoObject["main_info"] = addInfo.mainInfoText;

        // Status effects for additional info
        QJsonObject statusEffectsObject;
        for (auto j = 0; j < addInfo.statusEffects.size(); j++) {
            const auto& statusEffect = addInfo.statusEffects.at(j);
            QJsonObject singleEffectObject;
            singleEffectObject["name"] = statusEffect.name;
            singleEffectObject["duration"] = (int) statusEffect.duration;
            singleEffectObject["is_permanent"] = statusEffect.isPermanent;

            statusEffectsObject[QString::number(j)] = singleEffectObject;
        }
        additionalInfoObject["status_effects"] = statusEffectsObject;
        singleCharacterObject["additional_info"] = additionalInfoObject;

        charactersObject[QString::number(i)] = singleCharacterObject;
    }
    lcmFile["characters"] = charactersObject;

    // Write to file
    auto byteArray = QJsonDocument(lcmFile).toJson();
    QFile fileOut(fileName);
    fileOut.open(QIODevice::WriteOnly);
    return fileOut.write(byteArray) != -1;
}
int
TableFileHandler::getStatus(const QString& fileName)
{
    // Try to open
    QFile fileIin(fileName);
    if (!fileIin.open(QIODevice::ReadOnly)) {
        // Read failed
        return 2;
    }

    auto byteArray = fileIin.readAll();
    const auto document = QJsonDocument::fromJson(byteArray);
    m_lcmFile = document.object();
    // Correct or false format
    return !checkFileFormat();
}
bool
TableFileHandler::checkFileFormat() const
{
    auto checker = !m_lcmFile.empty() && m_lcmFile.contains("row_entered") && m_lcmFile.contains("round_counter") &&
                   m_lcmFile.contains("ruleset") && m_lcmFile.contains("roll_automatically") &&
                   m_lcmFile.contains("characters");

    return checker;
}