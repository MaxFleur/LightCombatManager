#pragma once

#include "AdditionalInfoData.hpp"
#include "FocusOutLineEdit.hpp"

#include <QLineEdit>
#include <QPointer>

class QHBoxLayout;
class QLabel;

// This class displays additional information and status effects
class AdditionalInfoWidget : public QWidget {
    Q_OBJECT

public:
    AdditionalInfoWidget();

    void
    adjustEffectDuration(bool decrease);

    void
    setStatusEffects(const QVector<AdditionalInfoData::StatusEffect>& effects);

    void
    setMainInfoText(const QString& text)
    {
        m_additionalInfoData.mainInfoText = text;
        m_additionalInfoLineEdit->setText(text);
    }

    [[nodiscard]] const QString
    getMainInfoText()
    {
        return m_additionalInfoLineEdit->text();
    }

    [[nodiscard]] const AdditionalInfoData
    getAdditionalInformation()
    {
        return m_additionalInfoData;
    }

signals:
    void
    widgetCalled();

    void
    additionalInfoEdited();

    void
    widthAdjusted(int width);

private slots:
    void
    triggerAdditionalInfoEdited();

private:
    void
    calculateWidth();

    bool
    eventFilter(QObject* object,
                QEvent*  event) override;

    bool
    event(QEvent *event) override;

private:
    QPointer<FocusOutLineEdit> m_additionalInfoLineEdit;
    QPointer<QLabel> m_statusEffectLabel;
    QPointer<QHBoxLayout> m_statusEffectsLayout;

    AdditionalInfoData m_additionalInfoData;

    QString m_mainInfoTextCache;

    int m_statusEffectsLayoutWidth = 0;

    static constexpr int LENGTH_BUFFER = 30;
};
