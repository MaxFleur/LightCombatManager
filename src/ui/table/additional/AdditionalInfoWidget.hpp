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
    clear();

    void
    adjustRoundNumber(bool decrease);

    void
    setStatusEffects(const QVector<AdditionalInfoData::StatusEffect>& effects);

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
    lineEditFocused();

    void
    mainInfoEdited();

private:
    bool
    eventFilter(QObject* object,
                QEvent*  event);

private:
    QPointer<QLineEdit> m_additionalInfoLineEdit;
    QPointer<QLabel> m_statusEffectLabel;
    QPointer<QHBoxLayout> m_statusEffectsLayout;

    QVector<QPointer<QWidget> > m_statusEffectWidgets;
    QVector<AdditionalInfoData::StatusEffect> m_statusEffects;
};
