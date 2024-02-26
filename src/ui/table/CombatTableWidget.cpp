#include "CombatTableWidget.hpp"

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "UtilsGeneral.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QObject>
#include <QWidget>

CombatTableWidget::CombatTableWidget(std::shared_ptr<CharacterHandler> characterHandler,
                                     QWidget *                         parent) :
    QTableWidget(parent),
    m_characterHandler(characterHandler)
{
    connect(this, &QTableWidget::itemSelectionChanged, this, &CombatTableWidget::removeAlphaValueForCellWidgets);
}


void
CombatTableWidget::resynchronizeCharacters()
{
    m_characterHandler->clearCharacters();

    for (auto i = 0; i < rowCount(); i++) {
        auto *const additionalInfoWidget = cellWidget(i, 5)->findChild<AdditionalInfoWidget *>();

        m_characterHandler->storeCharacter(item(i, 0)->text(),
                                           item(i, 1)->text().toInt(),
                                           item(i, 2)->text().toInt(),
                                           item(i, 3)->text().toInt(),
                                           item(i, 4)->checkState() == Qt::Checked,
                                           additionalInfoWidget->getAdditionalInformation());
    }
}


void
CombatTableWidget::setRowAndPlayer(QLabel *roundCounterLabel, QLabel *currentPlayerLabel, int rowEntered)
{
    selectionModel()->clearSelection();

    if (rowCount() == 0) {
        currentPlayerLabel->setText(QObject::tr("Current: None"));
        roundCounterLabel->setText(QObject::tr("Round 0"));
        return;
    }

    // Setting fonts may trigger unwished item setting events, so block the signals
    blockSignals(true);
    // Reset bold text rows to standard font
    for (auto i = 0; i < rowCount(); i++) {
        if (const auto font = item(i, 0)->font(); font.bold()) {
            for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
                item(i, j)->setFont(font.defaultFamily());
            }
            break;
        }
    }

    // Highlight selected row with bold fonts, but ignore the last columns (widgets)
    auto font = item(rowEntered, 0)->font();
    font.setBold(true);
    for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
        item(rowEntered, j)->setFont(font);
    }
    blockSignals(false);

    currentPlayerLabel->setText(QObject::tr("Current: ") + item(rowEntered, 0)->text());
}


void
CombatTableWidget::setTableRowColor(bool resetColor)
{
    blockSignals(true);

    m_rowsUncolored = resetColor;
    const auto color = [this](bool isEnemy, bool isButton) {
        // Weaker alpha value so that the status effect buttons do not stick out too much
        const auto alpha = isButton ? 10 : 60;
        if (m_rowsUncolored) {
            return QApplication::palette().color(QPalette::Base);
        }
        return isEnemy ? QColor(255, 194, 10, alpha) : QColor(12, 123, 220, alpha);
    };

    for (auto i = 0; i < rowCount(); i++) {
        const auto isEnemy = item(i, COL_ENEMY)->checkState() == Qt::Checked;
        const auto noButtonColor = color(isEnemy, false);

        for (auto j = 0; j < 5; j++) {
            item(i, j)->setBackground(noButtonColor);
        }

        QPalette palette;
        palette.setColor(QPalette::Base, noButtonColor);
        palette.setColor(QPalette::Button, m_rowsUncolored ? noButtonColor : color(isEnemy, true));
        cellWidget(i, COL_ADDITIONAL)->setAutoFillBackground(!m_rowsUncolored);
        cellWidget(i, COL_ADDITIONAL)->setPalette(palette);
    }

    blockSignals(false);
}


void
CombatTableWidget::setIniColumnTooltips(bool resetToolTip)
{
    blockSignals(true);

    for (auto i = 0; i < rowCount(); i++) {
        auto toolTipString = QString();
        if (!resetToolTip) {
            const auto rolledValue = item(i, 1)->text().toInt() - item(i, 2)->text().toInt();
            toolTipString += "Calculation: Rolled Value " + QString::number(rolledValue) + ", Modifier " + item(i, 2)->text();
        }

        item(i, 1)->setToolTip(toolTipString);
    }

    blockSignals(false);
}


void
CombatTableWidget::setStatusEffectInWidget(QVector<AdditionalInfoData::StatusEffect> statusEffects, int row)
{
    auto* const additionalInfoWidget = cellWidget(row, COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
    additionalInfoWidget->setStatusEffects(statusEffects);
}


void
CombatTableWidget::adjustStatusEffectRoundCounter(bool decrease)
{
    for (auto i = 0; i < rowCount(); i++) {
        auto* const additionalInfoWidget = cellWidget(i, COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
        additionalInfoWidget->adjustEffectDuration(decrease);
    }
}


QVector<QVector<QVariant> >
CombatTableWidget::tableDataFromWidget()
{
    QVector<QVector<QVariant> > tableData;
    for (auto i = 0; i < rowCount(); i++) {
        QVector<QVariant> rowValues;

        for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
            rowValues.push_back(item(i, j)->text());
        }

        rowValues.push_back(item(i, COL_ENEMY)->checkState() == Qt::Checked);

        QVariant variant;
        variant.setValue(cellWidget(i, COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>()->getAdditionalInformation());
        rowValues.push_back(variant);

        tableData.push_back(rowValues);
    }

    return tableData;
}


QVector<QVector<QVariant> >
CombatTableWidget::tableDataFromCharacterVector()
{
    QVector<QVector<QVariant> > tableData;
    for (const auto& character : m_characterHandler->getCharacters()) {
        QVector<QVariant> charValues;

        QVariant additionalInfoVariant;
        additionalInfoVariant.setValue(character.additionalInfoData);
        charValues << character.name << character.initiative << character.modifier
                   << character.hp << character.isEnemy << additionalInfoVariant;

        tableData.push_back(charValues);
    }

    return tableData;
}


unsigned int
CombatTableWidget::getHeight() const
{
    auto height = 0;
    for (int i = 0; i < rowCount(); i++) {
        height += rowHeight(i);
    }
    return height + HEIGHT_BUFFER;
}


void
CombatTableWidget::keyPressEvent(QKeyEvent *event)
{
    // Ignore arrow keys
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
        event->ignore();
        return;
    }
    QTableWidget::keyPressEvent(event);
}


void
CombatTableWidget::removeAlphaValueForCellWidgets()
{
    // If the table rows are colored and a row is selected, the selection color will mix
    // with the cell widget color because of the non-maximum alpha values.
    // So we set the alpha value for selected rows to 0 to circumvent this problem
    if (m_rowsUncolored) {
        return;
    }
    // Reset all colors because other rows might have been selected before
    setTableRowColor(m_rowsUncolored);

    const auto selectedRows = selectionModel()->selectedRows();

    for (const auto& selectedModel : selectedRows) {
        const auto row = selectedModel.row();

        auto palette = cellWidget(row, COL_ADDITIONAL)->palette();
        auto color = palette.color(QPalette::Base);

        color.setAlpha(0);
        palette.setColor(QPalette::Base, color);
        cellWidget(row, COL_ADDITIONAL)->setPalette(palette);
    }
}
