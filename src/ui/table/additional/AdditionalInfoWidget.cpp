#include "AdditionalInfoWidget.hpp"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

AdditionalInfoWidget::AdditionalInfoWidget()
{
    m_additionalInfoLineEdit = new QLineEdit;
    m_additionalInfoLineEdit->installEventFilter(this);

    m_statusEffectLabel = new QLabel(tr("Status Effects:"));
    m_statusEffectLabel->setVisible(false);

    m_statusEffectsLayout = new QHBoxLayout;
    m_statusEffectsLayout->addWidget(m_statusEffectLabel);

    auto* const mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_additionalInfoLineEdit);
    mainLayout->addLayout(m_statusEffectsLayout);

    setLayout(mainLayout);

    connect(m_additionalInfoLineEdit, &QLineEdit::returnPressed, this, [this] {
        m_additionalInfoLineEdit->clearFocus();
        emit mainInfoEdited();
    });
}


void
AdditionalInfoWidget::clear()
{
    m_additionalInfoLineEdit->setText("");
    m_statusEffectWidgets.clear();
}


void
AdditionalInfoWidget::adjustRoundNumber(bool decrease)
{
    const auto valueModifier = decrease ? -1 : 1;

    for (auto i = m_statusEffects.size() - 1; i >= 0; i--) {
        auto& effect = m_statusEffects[i];

        if (effect.isPermanent) {
            continue;
        }
        effect.duration += valueModifier;
        if (effect.duration < 1) {
            m_statusEffects.erase(m_statusEffects.begin() + i);
        }
    }
}


void
AdditionalInfoWidget::setStatusEffects(const QVector<AdditionalInfoData::StatusEffect>& effects)
{
    m_statusEffects = effects;
    m_statusEffectLabel->setVisible(!effects.empty());

    for (const auto& effect : effects) {
        auto labelText = effect.name;
        if (!effect.isPermanent) {
            labelText.append(" (" + QString::number(effect.duration) + ")");
        }

        auto* const label = new QLabel(labelText);
        m_statusEffectsLayout->addWidget(label);
        m_statusEffectWidgets.push_back(label);
    }

    m_statusEffectsLayout->addStretch();
}


bool
AdditionalInfoWidget::eventFilter(QObject* object, QEvent* event)
{
    if (object == m_additionalInfoLineEdit && event->type() == QEvent::FocusIn) {
        emit lineEditFocused();
        return false;
    }
    return false;
}
