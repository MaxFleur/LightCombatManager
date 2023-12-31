#include "UtilsGeneral.hpp"

#include "AdditionalInfoWidget.hpp"

#include <QApplication>
#include <QFileInfo>

#include <random>

namespace Utils::General
{
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
getAutoRollEnabled(bool autoRollEnabled)
{
    return autoRollEnabled ? "automatic rolling enabled" : "automatic rolling disabled";
}


bool
isSystemInDarkMode()
{
    const QWidget widget;
    const auto color = widget.palette().color(QPalette::Window);
    const auto luminance = sqrt(0.299 * std::pow(color.redF(), 2) +
                                0.587 * std::pow(color.greenF(), 2) +
                                0.114 * std::pow(color.blueF(), 2));
    return luminance < 0.2;
}
}
