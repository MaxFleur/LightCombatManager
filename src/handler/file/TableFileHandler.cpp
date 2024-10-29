#include "TableFileHandler.hpp"

#include "AdditionalInfoData.hpp"

#include <QFile>
#include <QJsonDocument>

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
    for (auto i = 0; const auto& row : tableData) {
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
        for (auto j = 0; const auto& statusEffect : addInfo.statusEffects) {
            QJsonObject singleEffectObject;
            singleEffectObject["name"] = statusEffect.name;
            singleEffectObject["duration"] = (int) statusEffect.duration;
            singleEffectObject["is_permanent"] = statusEffect.isPermanent;

            statusEffectsObject[QString::number(j++)] = singleEffectObject;
        }
        additionalInfoObject["status_effects"] = statusEffectsObject;
        singleCharacterObject["additional_info"] = additionalInfoObject;

        charactersObject[QString::number(i++)] = singleCharacterObject;
    }
    lcmFile["characters"] = charactersObject;

    // Write to file
    auto byteArray = QJsonDocument(lcmFile).toJson();
    QFile fileOut(fileName);
    fileOut.open(QIODevice::WriteOnly);
    return fileOut.write(byteArray) != -1;
}


bool
TableFileHandler::checkFileFormat() const
{
    auto checker = !m_fileData.empty() && m_fileData.contains("row_entered") && m_fileData.contains("round_counter") &&
                   m_fileData.contains("ruleset") && m_fileData.contains("roll_automatically") &&
                   m_fileData.contains("characters");

    return checker;
}
