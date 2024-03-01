#include "AdditionalInfoWidget.hpp"

#include "StatusEffectButton.hpp"
#include "UtilsGeneral.hpp"

#include <QApplication>
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
    auto& statusEffects = m_additionalInfoData.statusEffects;
    for (auto i = statusEffects.size() - 1; i >= 0; i--) {
        if (statusEffects.at(i).isPermanent) {
            continue;
        }

        auto& effect = statusEffects[i];

        effect.duration += decrease ? -1 : 1;
        if (effect.duration < 1) {
            statusEffects.erase(statusEffects.begin() + i);
        }
    }
}


void
AdditionalInfoWidget::setStatusEffects(const QVector<AdditionalInfoData::StatusEffect>& effects)
{
    m_additionalInfoData.statusEffects = effects;
    m_statusEffectLabel->setVisible(!effects.empty());

    m_statusEffectsLayoutWidth = 0;
    m_statusEffectsLayoutWidth += Utils::General::getStringWidth(m_statusEffectLabel->text());

    for (auto i = 0; i < m_additionalInfoData.statusEffects.size(); i++) {
        auto* const statusEffectButton = new StatusEffectButton(m_additionalInfoData.statusEffects[i]);
        connect(statusEffectButton, &StatusEffectButton::menuCalled, this, [this] {
            emit widgetCalled();
        });
        connect(statusEffectButton, &StatusEffectButton::effectChanged, this, [this, i] (auto statusEffect) {
            m_additionalInfoData.statusEffects[i] = statusEffect;
            emit additionalInfoEdited();
        });
        connect(statusEffectButton, &StatusEffectButton::removeCalled, this, [this, i] {
            m_additionalInfoData.statusEffects.erase(m_additionalInfoData.statusEffects.begin() + i);
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
    if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        auto lineEditPalette = m_additionalInfoLineEdit->palette();
        auto applicationBaseColor = QApplication::palette().color(QPalette::Button);
        lineEditPalette.setColor(QPalette::Base, applicationBaseColor);
        m_additionalInfoLineEdit->setPalette(lineEditPalette);

        auto thisPalette = palette();
        thisPalette.setColor(QPalette::Button, QApplication::palette().color(QPalette::Button));
        thisPalette.setColor(QPalette::ButtonText, QApplication::palette().color(QPalette::ButtonText));
        setPalette(thisPalette);
    }
    return QWidget::event(event);
}
