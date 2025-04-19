#pragma once

#include "AdditionalInfoData.hpp"
#include "EffectFileHandler.hpp"

#include <QDialog>
#include <QPointer>
#include <QTreeWidgetItem>

class QCheckBox;
class QLineEdit;
class QTreeWidget;
class QToolButton;
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
    setRemoveButtonEnabling();

    void
    addEffectButtonClicked();

    void
    removeEffectButtonClicked();

    void
    okButtonClicked();

private:
    void
    createEffect(const QString& effectName);

    void
    findEffect(const QString& filter);

    void
    setButtonIcons();

    bool
    event(QEvent* event);

private:
    QPointer<QLineEdit> m_lineEdit;
    QPointer<QTreeWidget> m_treeWidget;
    QPointer<QToolButton> m_removeEffectButton;
    QPointer<QToolButton> m_addEffectButton;
    QPointer<QCheckBox> m_checkBox;
    QPointer<QSpinBox> m_spinBox;
    QTreeWidgetItem* m_customHeaderItem;

    std::unique_ptr<EffectFileHandler> m_effectFileHandler;

    QVector<AdditionalInfoData::StatusEffect> m_effects;

    const RuleSettings& m_ruleSettings;

    static constexpr int COL_NAME = 0;
};
