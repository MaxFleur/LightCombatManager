#include "UtilsTable.hpp"

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "CombatWidget.hpp"
#include "UtilsGeneral.hpp"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QObject>
#include <QWidget>

namespace Utils::Table
{
void
setTableCheckBox(CombatWidget *combatWidget, unsigned int row, bool checked)
{
    auto *const combatTableWidget = combatWidget->getCombatTableWidget();

    auto *const checkBox = new QCheckBox;
    checkBox->setChecked(checked);
    QObject::connect(checkBox, &QCheckBox::stateChanged, combatTableWidget, [combatWidget, combatTableWidget, checkBox] {
        combatTableWidget->blockSignals(true);
        // We need to store the old checkbox state, so we will reset the state for a short time
        // Then, after saving, reset to the new value and set the correct undo command
        checkBox->blockSignals(true);
        checkBox->setCheckState(checkBox->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        combatWidget->saveOldState();
        checkBox->setCheckState(checkBox->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        combatWidget->pushOnUndoStack(true);
        checkBox->blockSignals(false);

        emit combatWidget->changeOccured();
        combatTableWidget->blockSignals(false);
    });

    // Center the checkboxes
    auto *const widget = new QWidget;
    auto *layout = new QHBoxLayout(widget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    widget->setLayout(layout);

    combatTableWidget->setCellWidget(row, COL_ENEMY, widget);
}


void
setTableAdditionalInfoWidget(CombatWidget* combatWidget, unsigned int row, const QVariant& additionalInfo)
{
    auto *const combatTableWidget = combatWidget->getCombatTableWidget();

    auto* const additionalInfoWidget = new AdditionalInfoWidget;
    QObject::connect(additionalInfoWidget, &AdditionalInfoWidget::widgetCalled, combatWidget, [combatWidget] {
        combatWidget->saveOldState();
    });
    QObject::connect(additionalInfoWidget, &AdditionalInfoWidget::additionalInfoEdited, combatWidget, [combatWidget] {
        combatWidget->pushOnUndoStack(true);
    });
    QObject::connect(additionalInfoWidget, &AdditionalInfoWidget::widthAdjusted, combatWidget,
                     [combatWidget, combatTableWidget, row] (int additionalInfoWidth) {
        const auto nameWidth = Utils::General::getStringWidth(combatTableWidget->item(row, 0)->text());
        combatWidget->resetNameAndInfoWidth(nameWidth, additionalInfoWidth);
    });

    auto *const widget = new QWidget;
    auto *layout = new QHBoxLayout(widget);
    layout->addWidget(additionalInfoWidget);
    layout->setAlignment(Qt::AlignLeft);
    widget->setLayout(layout);

    const auto converted = additionalInfo.value<AdditionalInfoData>();
    additionalInfoWidget->setMainInfoText(converted.mainInfoText);
    additionalInfoWidget->setStatusEffects(converted.statusEffects);
    combatTableWidget->setCellWidget(row, COL_ADDITIONAL, widget);
}
}
