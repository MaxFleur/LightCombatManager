#pragma once

#include <QLineEdit>
#include <QPointer>

#include "AdditionalInfoData.hpp"

class QHBoxLayout;
class QLabel;

// This class displays additional information and status effects
class AdditionalInfoWidget : public QWidget {
    Q_OBJECT

public:
    AdditionalInfoWidget();

    void
    adjustRoundNumber(bool decrease);

    void
    setStatusEffects(QVector<AdditionalInfoData::StatusEffect>& effects);

    void
    setMainInfoText(const QString& text)
    {
        m_additionalInfoLineEdit->setText(text);
    }

    const QString
    getMainInfoText()
    {
        return m_additionalInfoLineEdit->text();
    }

    const AdditionalInfoData::AdditionalInformation
    getAdditionalInformation()
    {
        return { m_additionalInfoLineEdit->text(), m_statusEffects };
    }

signals:
    void
    widgetCalled();

    void
    additionalInfoEdited();

private:
    bool
    eventFilter(QObject* object,
                QEvent*  event);

private:
    QPointer<QLineEdit> m_additionalInfoLineEdit;
    QPointer<QLabel> m_statusEffectLabel;
    QPointer<QHBoxLayout> m_statusEffectsLayout;

    QVector<AdditionalInfoData::StatusEffect> m_statusEffects;
};
