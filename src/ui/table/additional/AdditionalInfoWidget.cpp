#include "AdditionalInfoWidget.hpp"

#include "StatusEffectButton.hpp"
#include "UtilsGeneral.hpp"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

AdditionalInfoWidget::AdditionalInfoWidget()
{
    m_additionalInfoLineEdit = new FocusOutLineEdit;
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
        // Will trigger a focus out event and thus a possible signal emit
        m_additionalInfoLineEdit->clearFocus();
    });
    connect(m_additionalInfoLineEdit, &FocusOutLineEdit::focusOut, this, &AdditionalInfoWidget::triggerAdditionalInfoEdited);
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
AdditionalInfoWidget::setStatusEffects(const QVector<AdditionalInfoData::StatusEffect>& effects)
{
    m_statusEffects = effects;
    m_statusEffectLabel->setVisible(!effects.empty());

    m_statusEffectsLayoutWidth = 0;
    m_statusEffectsLayoutWidth += Utils::General::getStringWidth(m_statusEffectLabel->text());

    for (auto i = 0; i < m_statusEffects.size(); i++) {
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
        const auto buttonTextStringWidth = Utils::General::getStringWidth(statusEffectButton->text());
        // The normal widget width returns weird values, so as a workaround use the text width with a buffer
        m_statusEffectsLayoutWidth += buttonTextStringWidth + LENGTH_BUFFER;
    }
    m_statusEffectsLayout->addStretch();

    calculateWidth();
}


void
AdditionalInfoWidget::triggerAdditionalInfoEdited()
{
    calculateWidth();

    const auto mainInfoText = getMainInfoText();
    if (m_mainInfoTextCache != mainInfoText) {
        emit additionalInfoEdited();
    }
}


void
AdditionalInfoWidget::calculateWidth()
{
    const auto addInfoNewWidth = Utils::General::getStringWidth(m_additionalInfoLineEdit->text());
    const auto newWidgetWidth = std::max(addInfoNewWidth, m_statusEffectsLayoutWidth);

    emit widthAdjusted(newWidgetWidth);
}


bool
AdditionalInfoWidget::eventFilter(QObject* object, QEvent* event)
{
    if (object == m_additionalInfoLineEdit && event->type() == QEvent::FocusIn) {
        if (!m_isTextCacheLocked) {
            m_mainInfoTextCache = getMainInfoText();
            m_isTextCacheLocked = true;
        }
        emit widgetCalled();
    }
    return false;
}


bool
AdditionalInfoWidget::event(QEvent *event)
{
    // The color alpha for the line edit background seems to be applied differently when
    // the Base background is set in the table utils function. For this reason, we reset it
    // so that the line edit does not stick out to strongly.
    if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        auto palette = m_additionalInfoLineEdit->palette();
        auto color = palette.color(QPalette::Base);
        color.setAlpha(10);
        palette.setColor(QPalette::Base, color);

        m_additionalInfoLineEdit->setPalette(palette);
    }
    return QWidget::event(event);
}
