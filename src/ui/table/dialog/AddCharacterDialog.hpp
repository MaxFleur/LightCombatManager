#pragma once

#include <QDialog>
#include <QPointer>

#include "CharacterHandler.hpp"

class QCheckBox;
class QLabel;
class QLineEdit;
class QMessageBox;
class QSpinBox;

class RuleSettings;

// Dialog used to add new characters to an existing Combat
class AddCharacterDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    AddCharacterDialog(const RuleSettings& RuleSettings,
                       QWidget*            parent = 0);

signals:
    void
    characterCreated(CharacterHandler::Character character,
                     int                         instanceCount);

private slots:
    void
    setLabelRolled();

    void
    animateLabel();

    void
    saveButtonClicked();

    void
    resetButtonClicked();

    void
    okButtonClicked();

private:
    void
    keyPressEvent(QKeyEvent *event);

    void
    closeEvent(QCloseEvent *event);

private:
    QPointer<QLineEdit> m_nameEdit;
    QPointer<QSpinBox> m_iniBox;
    QPointer<QSpinBox> m_iniModifierBox;
    QPointer<QLabel> m_labelRolled;
    QPointer<QSpinBox> m_hpBox;
    QPointer<QCheckBox> m_enemyBox;
    QPointer<QLineEdit> m_addInfoEdit;

    QPointer<QCheckBox> m_multipleEnabledBox;
    QPointer<QSpinBox> m_instanceNumberBox;

    QPointer<QLabel> m_animatedLabel;
    QPointer<QTimer> m_timer;

    bool m_somethingStored{ false };
    bool m_isFirstCharStored{ false };

    const RuleSettings& m_ruleSettings;

    static constexpr int LABEL_SHOWN = 1000;
    static constexpr int LABEL_FADEOUT = 2000;
};
