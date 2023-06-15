#include "AdditionalInfoWidget.hpp"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "StatusEffectButton.hpp"

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
        emit additionalInfoEdited();
    });
}


void
AdditionalInfoWidget::adjustEffectDuration(bool decrease)
{
    for (auto i = m_statusEffects.size() - 1; i >= 0; i--) {
        if (m_statusEffects.at(i).isPermanent) {
            continue;
        }

        auto& effect = m_statusEffects[i];

        effect.duration += decrease ? -1 : 1;
        if (effect.duration < 1) {
            m_statusEffects.erase(m_statusEffects.begin() + i);
        }
    }
}


void
AdditionalInfoWidget::setStatusEffects(QVector<AdditionalInfoData::StatusEffect>& effects)
{
    m_statusEffects = effects;
    m_statusEffectLabel->setVisible(!effects.empty());

    for (int i = 0; i < m_statusEffects.size(); i++) {
        auto* const statusEffectButton = new StatusEffectButton(m_statusEffects[i]);
        connect(statusEffectButton, &StatusEffectButton::menuCalled, this, [this] {
            emit widgetCalled();
        });
        connect(statusEffectButton, &StatusEffectButton::effectChanged, this, [this, i] (auto statusEffect) {
            m_statusEffects[i] = statusEffect;
            emit additionalInfoEdited();
        });
        connect(statusEffectButton, &StatusEffectButton::removeCalled, this, [this, i] {
            m_statusEffects.erase(m_statusEffects.begin() + i);
            emit additionalInfoEdited();
        });

        m_statusEffectsLayout->addWidget(statusEffectButton);
    }

    m_statusEffectsLayout->addStretch();
}


bool
AdditionalInfoWidget::eventFilter(QObject* object, QEvent* event)
{
    if (object == m_additionalInfoLineEdit && event->type() == QEvent::FocusIn) {
        emit widgetCalled();
        return false;
    }
    return false;
}
