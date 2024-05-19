#pragma once

#include "AdditionalInfoData.hpp"
#include "CharacterHandler.hpp"

class QLabel;
class QTableWidget;

// General utility functions
namespace Utils::General
{
void
displayWarningMessageBox(QWidget*       widget,
                         const QString& title,
                         const QString& text);

// Roll a 20 sided dice
[[nodiscard]] int
rollDice();

// Get a QString width in pixels
[[nodiscard]] int
getStringWidth(const QString& str);

// Trim a file's path so only the file name remains
[[nodiscard]] QString
getLCMName(const QString& filePath);

[[nodiscard]] QString
getRulesetName(unsigned int ruleset);

[[nodiscard]] QString
getAutoRollEnabled(bool autoRollEnabled);

void
animateLabel(QPointer<QLabel> label);

// Calculate the luminance value of a certain color, determine if the color is in dark mode
[[nodiscard]] bool
isSystemInDarkMode();
}
