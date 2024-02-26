#include "UtilsTable.hpp"

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "CombatWidget.hpp"
#include "UtilsGeneral.hpp"

#include <QHBoxLayout>
#include <QObject>
#include <QWidget>

namespace Utils::Table
{
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
