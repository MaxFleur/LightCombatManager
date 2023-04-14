#pragma once

#include "CharacterHandler.hpp"

class QColor;
class QFont;
class QTableWidget;

// General utility functions
namespace Utils
{
namespace General
{
// Append a comma to a string, depending on if the string already ends with a comma
[[nodiscard]] QString
appendCommaToString(const QString& inputText);

// CSV files use semicolons to separate the cells, so the name and additional info columns
// are checked for these to prevent additional, user entered semicolons being saved
[[nodiscard]] bool
containsSemicolon(const QTableWidget *tableWidget);

// Roll a 20 sided dice
[[nodiscard]] int
rollDice();

// Get a QString width in pixels
[[nodiscard]] int
getStringWidth(const QString& str,
               const QFont&   font);

// Trim a file's path so only the file name remains
[[nodiscard]] QString
getCSVName(const QString& filePath);

[[nodiscard]] QString
getRulesetName(unsigned int ruleset);

[[nodiscard]] QString
getAutoRollEnabled(unsigned int autoRollEnabled);

// Calculate the luminance value of a certain color, determine if the color is in dark mode
[[nodiscard]] bool
isColorDark(const QColor& color);
}
}
