#include "UtilsGeneral.hpp"

#include "AdditionalInfoWidget.hpp"

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QTableWidget>

#include <random>

namespace Utils::General
{
bool
containsSemicolon(const QTableWidget *tableWidget)
{
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        const auto textNameColumn = tableWidget->item(i, 0)->text();
        const auto textAddInfoColumn = tableWidget->cellWidget(i, 5)->findChild<AdditionalInfoWidget *>()->getAdditionalInformation().mainInfo;

        if (textNameColumn.contains(';') || textAddInfoColumn.contains(';')) {
            return true;
        }
    }
    return false;
}


int
rollDice()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 20);

    return distr(gen);
}


int
getStringWidth(const QString& str)
{
    // Use a bold font for longer columns
    auto font = QApplication::font();
    font.setBold(true);

    const QFontMetrics fontMetrics(font);
    return fontMetrics.boundingRect(str).width();
}


QString
getCSVName(const QString& filePath)
{
    const QFileInfo fileInfo(filePath);
    return fileInfo.fileName();
}


QString
getRulesetName(unsigned int ruleset)
{
    switch (ruleset) {
    case 0:
        return "Pathfinder 1E/D&D 3.5E";
        break;
    case 1:
        return "Pathfinder 2E";
        break;
    case 2:
        return "D&D 5E";
        break;
    case 3:
        return "D&D 3.0E";
        break;
    case 4:
        return "Starfinder";
        break;
    default:
        return "";
        break;
    }
}


QString
getAutoRollEnabled(unsigned int autoRollEnabled)
{
    return (autoRollEnabled == 1) ? "automatic rolling enabled" : "automatic rolling disabled";
}


bool
isColorDark(const QColor& color)
{
    const auto luminance = sqrt(0.299 * std::pow(color.redF(), 2) +
                                0.587 * std::pow(color.greenF(), 2) +
                                0.114 * std::pow(color.blueF(), 2));
    return luminance < 0.2;
}


AdditionalInfoData::AdditionalInformation
convertStringToAdditionalInfoData(const QString& str)
{
    AdditionalInfoData::AdditionalInformation additionalInformation;
    // Main info text to the left of separator
    auto splitIndex = str.lastIndexOf("---");
    additionalInformation.mainInfo = splitIndex == -1 ? str : str.left(splitIndex);
    if (splitIndex == -1) {
        return additionalInformation;
    }

    auto statusEffects = str.mid(splitIndex + 3);
    // Status effects are separated using  '--'
    const auto& statusEffectsSplitted = statusEffects.split("--");

    for (const auto& statusEffectString : statusEffectsSplitted) {
        // Abort for last string which is always empty
        if (statusEffectString.isEmpty()) {
            break;
        }

        AdditionalInfoData::StatusEffect statusEffect;
        // Status effect members are separated using '+'
        const auto& statusEffectSplitted = statusEffectString.split("+");
        // Apply
        statusEffect.name = statusEffectSplitted.at(0);
        statusEffect.isPermanent = statusEffectSplitted.at(1).toInt() == 1;
        statusEffect.duration = statusEffectSplitted.at(2).toInt();

        additionalInformation.statusEffects.push_back(statusEffect);
    }

    return additionalInformation;
}
}
