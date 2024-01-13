#pragma once

#include "CharacterHandler.hpp"

#include <QDialog>
#include <QPointer>

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
    AddCharacterDialog(QWidget* parent = 0);

signals:
    void
    characterCreated(CharacterHandler::Character character,
                     int                         instanceCount);

private slots:
    void
    randomButtonClicked();

    void
    animateLabel();

    void
    saveButtonClicked();

    void
    resetButtonClicked();

    void
    okButtonClicked();

private:
    QPointer<QLineEdit> m_nameEdit;
    QPointer<QSpinBox> m_iniBox;
    QPointer<QSpinBox> m_iniModifierBox;
    QPointer<QLabel> m_rolledValueLabel;
    QPointer<QSpinBox> m_hpBox;
    QPointer<QCheckBox> m_enemyBox;
    QPointer<QLineEdit> m_addInfoEdit;
    QPointer<QCheckBox> m_multipleEnabledBox;
    QPointer<QSpinBox> m_instanceNumberBox;

    QPointer<QLabel> m_animatedLabel;

    QPointer<QTimer> m_timer;

    int m_iniWithoutModValue = 0;

    bool m_isFirstCharStored{ false };

    static constexpr int LABEL_SHOWN_DURATION = 1000;
    static constexpr int LABEL_FADEOUT = 2000;
};
