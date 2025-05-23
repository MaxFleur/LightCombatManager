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

    m_additionalInfoLineEditLayout = new QVBoxLayout;
    m_additionalInfoLineEditLayout->setContentsMargins(BORDER_VALUES, BORDER_VALUES, BORDER_VALUES, BORDER_VALUES);
    m_additionalInfoLineEditLayout->addWidget(m_additionalInfoLineEdit);

    m_statusEffectLabel = new QLabel(tr("Status Effects:"));
    m_statusEffectLabel->setVisible(false);

    m_statusEffectsLayout = new QHBoxLayout;
    m_statusEffectsLayout->addWidget(m_statusEffectLabel);
    m_statusEffectsLayout->addSpacing(SPACING);

    auto* const mainLayout = new QVBoxLayout;
    mainLayout->addLayout(m_additionalInfoLineEditLayout);
    mainLayout->addLayout(m_statusEffectsLayout);
    mainLayout->setSpacing(BORDER_VALUES);
    mainLayout->setContentsMargins(BORDER_VALUES, BORDER_VALUES, BORDER_VALUES, BORDER_VALUES);

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
    m_additionalInfoLineEditLayout->setContentsMargins(BORDER_VALUES, BORDER_VALUES, BORDER_VALUES,
                                                       m_additionalInfoData.statusEffects.empty() ?
                                                       BORDER_VALUES : LINE_EDIT_BOTTOM_SPACING);
    calculateWidth();
}


void
AdditionalInfoWidget::triggerAdditionalInfoEdited()
{
    calculateWidth();

    const auto mainInfoText = getMainInfoText();
    if (m_mainInfoTextCache != mainInfoText) {
        m_additionalInfoData.mainInfoText = mainInfoText;
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
        static auto isTextCacheLocked = false;

        if (!isTextCacheLocked) {
            m_mainInfoTextCache = getMainInfoText();
            isTextCacheLocked = true;
        }
        emit widgetCalled();
    }
    return false;
}


bool
AdditionalInfoWidget::event(QEvent *event)
{
    [[unlikely]] if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        auto lineEditPalette = m_additionalInfoLineEdit->palette();
        lineEditPalette.setColor(QPalette::Base, QApplication::palette().color(QPalette::Button));
        lineEditPalette.setColor(QPalette::Text, QApplication::palette().color(QPalette::Text));
        m_additionalInfoLineEdit->setPalette(lineEditPalette);

        auto thisPalette = palette();
        thisPalette.setColor(QPalette::Button, QApplication::palette().color(QPalette::Button));
        thisPalette.setColor(QPalette::ButtonText, QApplication::palette().color(QPalette::ButtonText));
        setPalette(thisPalette);
    }
    return QWidget::event(event);
}
