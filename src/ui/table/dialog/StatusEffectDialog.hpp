#pragma once

#include <QDialog>
#include <QPointer>

#include "AdditionalInfoData.hpp"

class QCheckBox;
class QLineEdit;
class QListWidget;
class QSpinBox;

class RuleSettings;

// Dialog used to add certain status effects to characters
class StatusEffectDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    StatusEffectDialog(const RuleSettings& RuleSettings,
                       QWidget *           parent = 0);

    [[nodiscard]] QVector<AdditionalInfoData::StatusEffect>&
    getEffects()
    {
        return m_effects;
    }

private slots:
    void
    okButtonClicked();

private:
    void
    createEffect(const QString& effectName);

    void
    findEffect(const QString& filter);

private:
    QPointer<QListWidget> m_listWidget;
    QPointer<QLineEdit> m_lineEdit;
    QPointer<QCheckBox> m_checkBox;
    QPointer<QSpinBox> m_spinBox;

    QVector<AdditionalInfoData::StatusEffect> m_effects;

    const RuleSettings& m_ruleSettings;
};
