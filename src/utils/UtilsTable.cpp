#include "UtilsTable.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "CombatWidget.hpp"
#include "UtilsGeneral.hpp"

namespace Utils::Table
{
void
resynchronizeCharacters(const QTableWidget *tableWidget,
                        CharacterHandlerRef characterHandler)
{
    characterHandler->clearCharacters();

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        // Cell widget is a checkbox within another widget, so find the child
        auto *const checkBox = tableWidget->cellWidget(i, 4)->findChild<QCheckBox *>();
        auto *const additionalInfoWidget = tableWidget->cellWidget(i, 5)->findChild<AdditionalInfoWidget *>();

        characterHandler->storeCharacter(
            tableWidget->item(i, 0)->text(),
            tableWidget->item(i, 1)->text().toInt(),
            tableWidget->item(i, 2)->text().toInt(),
            tableWidget->item(i, 3)->text().toInt(),
            checkBox->isChecked(),
            additionalInfoWidget->getAdditionalInformation());
    }
}


void
setTableCheckBox(CombatWidget *combatWidget, unsigned int row, bool checked)
{
    auto *const tableWidget = combatWidget->getTableWidget();

    auto *const checkBox = new QCheckBox;
    checkBox->setChecked(checked);
    QObject::connect(checkBox, &QCheckBox::stateChanged, tableWidget, [combatWidget, tableWidget, checkBox] {
            tableWidget->blockSignals(true);
            // We need to store the old checkbox state, so we will reset the state for a short time
            // Then, after saving, reset to the new value and set the correct undo command
            checkBox->blockSignals(true);
            checkBox->setChecked(checkBox->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
            combatWidget->saveOldState();
            checkBox->setChecked(checkBox->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
            combatWidget->pushOnUndoStack(true);
            checkBox->blockSignals(false);

            emit combatWidget->changeOccured();
            tableWidget->blockSignals(false);
        });

    // Center the checkboxes
    auto *const widget = new QWidget;
    auto *layout = new QHBoxLayout(widget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    widget->setLayout(layout);

    tableWidget->setCellWidget(row, COL_ENEMY, widget);
}


void
setTableAdditionalInfoWidget(CombatWidget* combatWidget, unsigned int row, const QVariant& additionalInfo)
{
    auto *const tableWidget = combatWidget->getTableWidget();

    auto* const additionalInfoWidget = new AdditionalInfoWidget;
    QObject::connect(additionalInfoWidget, &AdditionalInfoWidget::widgetCalled, combatWidget, [combatWidget] {
            combatWidget->saveOldState();
        });
    QObject::connect(additionalInfoWidget, &AdditionalInfoWidget::additionalInfoEdited, combatWidget, [combatWidget] {
            combatWidget->pushOnUndoStack(true);
        });
    QObject::connect(additionalInfoWidget, &AdditionalInfoWidget::widthAdjusted, combatWidget,
                     [combatWidget, tableWidget, row] (int additionalInfoWidth) {
            const auto nameWidth = Utils::General::getStringWidth(tableWidget->item(row, 0)->text());
            combatWidget->resetNameAndInfoWidth(nameWidth, additionalInfoWidth);
        });

    auto *const widget = new QWidget;
    auto *layout = new QHBoxLayout(widget);
    layout->addWidget(additionalInfoWidget);
    layout->setAlignment(Qt::AlignLeft);
    widget->setLayout(layout);

    const auto converted = additionalInfo.value<AdditionalInfoData::AdditionalInformation>();
    additionalInfoWidget->setMainInfoText(converted.mainInfo);
    additionalInfoWidget->setStatusEffects(converted.statusEffects);
    tableWidget->setCellWidget(row, COL_ADDITIONAL, widget);
}


void
setRowAndPlayer(QTableWidget *tableWidget, QLabel *roundCounterLabel, QLabel *currentPlayerLabel, int rowEntered)
{
    tableWidget->selectionModel()->clearSelection();
    // Setting fonts may trigger unwished item setting events, so block the signals
    tableWidget->blockSignals(true);

    if (tableWidget->rowCount() != 0) {
        // Reset bold text rows to standard font
        for (int i = 0; i < tableWidget->rowCount(); i++) {
            if (tableWidget->item(i, 0)->font().bold()) {
                const auto font = tableWidget->item(i, 0)->font();

                for (int j = 0; j < tableWidget->columnCount(); j++) {
                    if (j < FIRST_FOUR_COLUMNS) {
                        tableWidget->item(i, j)->setFont(font.defaultFamily());
                    }
                }
            }
        }

        // Highlight selected row with bold fonts, but ignore the last columns (widgets)
        for (int j = 0; j < FIRST_FOUR_COLUMNS; j++) {
            auto font = tableWidget->item(rowEntered, 0)->font();
            font.setBold(true);
            tableWidget->item(rowEntered, j)->setFont(font);
        }
    }
    tableWidget->blockSignals(false);

    if (tableWidget->rowCount() == 0) {
        currentPlayerLabel->setText(QObject::tr("Current: None"));
        roundCounterLabel->setText(QObject::tr("Round 0"));
        return;
    }
    currentPlayerLabel->setText(QObject::tr("Current: ") + tableWidget->item(rowEntered, 0)->text());
}


void
setTableRowColor(QTableWidget *tableWidget, bool resetColor)
{
    tableWidget->blockSignals(true);

    const auto color = [](bool resetColor, bool isEnemy, bool isButton) {
                           // Weaker alpha value so that the status effect buttons do not stick out too much
                           const auto alpha = isButton ? 10 : 60;
                           if (resetColor) {
                               return QApplication::palette().color(QPalette::Base);
                           }
                           return isEnemy ? QColor(255, 194, 10, alpha) : QColor(12, 123, 220, alpha);
                       };

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        const auto isEnemy = tableWidget->cellWidget(i, COL_ENEMY)->findChild<QCheckBox *>()->isChecked();
        for (int j = 0; j < tableWidget->columnCount(); j++) {
            if (j < FIRST_FOUR_COLUMNS) {
                tableWidget->item(i, j)->setBackground(color(resetColor, isEnemy, false));
            } else {
                QPalette palette;
                palette.setColor(QPalette::Base, color(resetColor, isEnemy, false));
                palette.setColor(QPalette::Button, color(resetColor, isEnemy, true));
                tableWidget->cellWidget(i, j)->setAutoFillBackground(!resetColor);
                tableWidget->cellWidget(i, j)->setPalette(palette);
            }
        }
    }

    tableWidget->blockSignals(false);
}


void
setIniColumnTooltips(QTableWidget *tableWidget, bool resetToolTip)
{
    tableWidget->blockSignals(true);

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        if (resetToolTip) {
            tableWidget->item(i, 1)->setToolTip(QString());
        } else {
            QString toolTipString = "Calculation: Rolled Value ";
            const auto rolledValue = tableWidget->item(i, 1)->text().toInt() - tableWidget->item(i, 2)->text().toInt();
            toolTipString += QString::number(rolledValue) + ", Modifier " + tableWidget->item(i, 2)->text();

            tableWidget->item(i, 1)->setToolTip(toolTipString);
        }
    }

    tableWidget->blockSignals(false);
}


void
setStatusEffectInWidget(const QTableWidget *tableWidget, QVector<AdditionalInfoData::StatusEffect> statusEffects, int row)
{
    auto* const additionalInfoWidget = tableWidget->cellWidget(row, COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
    additionalInfoWidget->setStatusEffects(statusEffects);
}


void
adjustStatusEffectRoundCounter(const QTableWidget *tableWidget, bool decrease)
{
    for (auto i = 0; i < tableWidget->rowCount(); i++) {
        auto* const additionalInfoWidget = tableWidget->cellWidget(i, COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
        additionalInfoWidget->adjustEffectDuration(decrease);
    }
}


QVector<QVector<QVariant> >
tableDataFromWidget(const QTableWidget *tableWidget)
{
    QVector<QVector<QVariant> > tableData;
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        QVector<QVariant> rowValues;

        for (int j = 0; j < tableWidget->columnCount(); j++) {
            switch (j) {
            case COL_ENEMY:
                rowValues.push_back(tableWidget->cellWidget(i, j)->findChild<QCheckBox *>()->isChecked());
                break;
            case COL_ADDITIONAL:
            {
                const auto variant = AdditionalInfoData::getAdditionalInformationVariant(
                    tableWidget->cellWidget(i, j)->findChild<AdditionalInfoWidget *>()->getAdditionalInformation()
                    );
                rowValues.push_back(variant);
                break;
            }
            default:
                rowValues.push_back(tableWidget->item(i, j)->text());
                break;
            }
        }
        tableData.push_back(rowValues);
    }

    return tableData;
}


QVector<QVector<QVariant> >
tableDataFromCharacterVector(CharacterHandlerRef characterHandler)
{
    const auto& characters = characterHandler->getCharacters();

    QVector<QVector<QVariant> > tableData;
    for (const auto& character : characters) {
        QVector<QVariant> charValues;

        const auto additionalInformationVariant = AdditionalInfoData::getAdditionalInformationVariant(character.additionalInformation);
        charValues << character.name << character.initiative << character.modifier
                   << character.hp << character.isEnemy << additionalInformationVariant;

        tableData.push_back(charValues);
    }

    return tableData;
}
}
