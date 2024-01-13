#pragma once

#include "AdditionalInfoData.hpp"
#include "CharacterHandler.hpp"

class QTableWidget;

// General utility functions
namespace Utils::General
{
// Roll a 20 sided dice
[[nodiscard]] int
rollDice();

// Get a QString width in pixels
[[nodiscard]] int
getStringWidth(const QString& str);

// Trim a file's path so only the file name remains
[[nodiscard]] QString
getCSVName(const QString& filePath);

[[nodiscard]] QString
getRulesetName(unsigned int ruleset);

[[nodiscard]] QString
getAutoRollEnabled(bool autoRollEnabled);

// Calculate the luminance value of a certain color, determine if the color is in dark mode
[[nodiscard]] bool
isSystemInDarkMode();
}
