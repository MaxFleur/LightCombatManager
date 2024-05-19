#pragma once

#include "CharacterHandler.hpp"

#include <QDialog>
#include <QPointer>
#include <QGridLayout>

class QCheckBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QSpinBox;

class RuleSettings;
class TemplatesWidget;

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
    storeTemplatesButtonClicked();

    void
    manageTemplatesVisibility(bool isVisible);

    void
    saveButtonClicked();

    void
    resetButtonClicked();

    void
    okButtonClicked();

    void
    applyLoadedCharacterToUI(const CharacterHandler::Character& character);

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
    QPointer<QPushButton> m_storeTemplatesButton;

    QPointer<TemplatesWidget> m_templatesWidget;
    QPointer<QWidget> m_rightSideWidget;

    QPointer<QLabel> m_animatedLabel;

    QPointer<QTimer> m_timer;

    int m_iniWithoutModValue = 0;

    bool m_isFirstCharStored{ false };

    static constexpr int LABEL_SHOWN_DURATION = 1000;
    static constexpr int LABEL_FADEOUT = 2000;
    static constexpr int MIN_ROW_HEIGHT = 12;
};
