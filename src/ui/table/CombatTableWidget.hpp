#pragma once

#include "CharacterHandler.hpp"

#include <QTableWidget>

class QLabel;

// Helper class, disabling arrow key input
class CombatTableWidget : public QTableWidget {
    Q_OBJECT

public:
    CombatTableWidget(std::shared_ptr<CharacterHandler> characterHandler,
                      QWidget *                         parent = 0);

    // Resynchronize the characters stored in the char handler vector
    // with the data in the table widget
    void
    resynchronizeCharacters();

    // Set the labels displaying the current player and round number
    void
    setRowAndPlayer(QLabel* roundCounterLabel,
                    QLabel* currentPlayerLabel,
                    int     rowEntered);

    void
    setTableRowColor(bool resetColor);

    void
    setIniColumnTooltips(bool resetToolTip);

    void
    setStatusEffectInWidget(QVector<AdditionalInfoData::StatusEffect> statusEffects,
                            int                                       row);

    void
    adjustStatusEffectRoundCounter(bool decrease);

    // Store the table cell values in a vector
    [[nodiscard]] QVector<QVector<QVariant> >
    tableDataFromWidget();

    // Reformat the character vector in a QVariant vector
    [[nodiscard]] QVector<QVector<QVariant> >
    tableDataFromCharacterVector();

    [[nodiscard]] unsigned int
    getHeight() const;

protected:
    void
    keyPressEvent(QKeyEvent *event) override;

private:
    void
    adjustAdditionalInfoWidgetPalette();

private:
    std::shared_ptr<CharacterHandler> m_characterHandler;

    bool m_rowsUncolored;

    static constexpr int COL_ENEMY = 4;
    static constexpr int COL_ADDITIONAL = 5;
    static constexpr int FIRST_FOUR_COLUMNS = 4;

    static constexpr int HEIGHT_BUFFER = 140;
};
