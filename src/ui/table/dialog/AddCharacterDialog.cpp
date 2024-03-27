#include "AddCharacterDialog.hpp"

#include "UtilsGeneral.hpp"
#include "TemplatesWidget.hpp"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QGraphicsEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QTimer>

class TemplatesListWidget;

AddCharacterDialog::AddCharacterDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Add new Character(s)"));

    auto *const nameLabel = new QLabel(tr("Name:"));
    m_nameEdit = new QLineEdit;
    m_nameEdit->setToolTip(tr("Set the name of the Character.\n"
                              "A Character can't be stored without a name."));

    auto *const iniLabel = new QLabel(tr("Initiative:"));
    m_iniBox = new QSpinBox;
    m_iniBox->setMinimum(-20);
    m_iniBox->setToolTip(tr("Set the initiative, including all modifiers."));

    auto *const randomIniButton = new QPushButton(tr("Random Initiative Value"));
    randomIniButton->setToolTip(tr("Roll a 20-sided dice to set the initiative."));

    auto *const iniModifierLabel = new QLabel(tr("Modifier:"));
    m_iniModifierBox = new QSpinBox;
    m_iniModifierBox->setMinimum(-10);
    m_iniModifierBox->setToolTip(tr("Set the initiative modifier."));

    m_rolledValueLabel = new QLabel;

    auto *const hpLabel = new QLabel(tr("Health Points:"));
    m_hpBox = new QSpinBox;
    m_hpBox->setRange(-10000, 10000);
    m_hpBox->setToolTip(tr("Set the Character's HP."));

    auto *const enemyLabel = new QLabel(tr("Is Enemy:"));
    m_enemyBox = new QCheckBox;
    m_enemyBox->setToolTip(tr("Set if the Character is an enemy."));

    auto *const addInfoLabel = new QLabel(tr("Additional Info:"));
    m_addInfoEdit = new QLineEdit;
    m_addInfoEdit->setToolTip(tr("Set additional information. \n"
                                 "Status Effects are added in the main table widget."));

    m_instanceNumberBox = new QSpinBox;
    m_instanceNumberBox->setRange(2, 10);
    m_instanceNumberBox->setEnabled(false);

    m_multipleEnabledBox = new QCheckBox(tr("Add Character multiple Times:"));
    m_multipleEnabledBox->setTristate(false);
    m_multipleEnabledBox->setCheckState(Qt::Unchecked);
    m_multipleEnabledBox->setToolTip(tr("If this is selected and 'Save' is pressed,\n"
                                        "the Character is added multiple times to the Table."));

    m_storeTemplatesButton = new QPushButton(tr("Store in Templates"));
    m_storeTemplatesButton->setVisible(false);
    auto *const resetButton = new QPushButton(tr("Reset all entered Values"));

    m_animatedLabel = new QLabel;

    auto *const openTemplatesButton = new QPushButton(tr("Templates >>"));

    auto *const buttonBox = new QDialogButtonBox;
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    auto *const saveButton = buttonBox->addButton(QDialogButtonBox::Save);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    okButton->setShortcut(Qt::Key_Return);
    saveButton->setShortcut(QKeySequence::Save);
    const auto saveShortcutText = "Save this Character (" + QKeySequence(QKeySequence::Save).toString() + ").";
    saveButton->setToolTip(tr(saveShortcutText.toLocal8Bit().constData()));

    auto *const gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(m_nameEdit, 0, 1, 1, 3);

    gridLayout->setRowMinimumHeight(1, MIN_ROW_HEIGHT);

    gridLayout->addWidget(iniLabel, 2, 0);
    gridLayout->addWidget(m_iniBox, 2, 1);
    gridLayout->addWidget(iniModifierLabel, 2, 2);
    gridLayout->addWidget(m_iniModifierBox, 2, 3);

    gridLayout->addWidget(randomIniButton, 3, 0, 1, 2);
    gridLayout->addWidget(m_rolledValueLabel, 3, 3);

    gridLayout->setRowMinimumHeight(4, MIN_ROW_HEIGHT);

    gridLayout->addWidget(hpLabel, 5, 0);
    gridLayout->addWidget(m_hpBox, 5, 1);
    gridLayout->addWidget(enemyLabel, 5, 2);
    gridLayout->addWidget(m_enemyBox, 5, 3);

    gridLayout->addWidget(addInfoLabel, 6, 0);
    gridLayout->addWidget(m_addInfoEdit, 6, 1, 1, 3);

    gridLayout->setRowMinimumHeight(7, MIN_ROW_HEIGHT);

    gridLayout->addWidget(m_multipleEnabledBox, 8, 0, 1, 3);
    gridLayout->addWidget(m_instanceNumberBox, 8, 3, 1, 1);

    gridLayout->setRowMinimumHeight(9, MIN_ROW_HEIGHT);

    gridLayout->addWidget(m_animatedLabel, 10, 0, 1, 2);

    gridLayout->addWidget(resetButton, 11, 2, 1, 2);
    gridLayout->addWidget(m_storeTemplatesButton, 11, 0, 1, 2);

    gridLayout->setRowMinimumHeight(12, MIN_ROW_HEIGHT);

    gridLayout->addWidget(openTemplatesButton, 13, 0, 1, 1);
    gridLayout->addWidget(buttonBox, 13, 1, 1, 3);

    m_templatesWidget = new TemplatesWidget;
    m_templatesWidget->setVisible(false);

    auto* const mainLayout = new QHBoxLayout;
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(m_templatesWidget);
    setLayout(mainLayout);

    m_nameEdit->setFocus(Qt::TabFocusReason);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);

    connect(randomIniButton, &QPushButton::clicked, this, &AddCharacterDialog::randomButtonClicked);
    connect(m_iniBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] {
        m_iniWithoutModValue = m_iniBox->value() - m_iniModifierBox->value();
    });
    connect(m_iniModifierBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] {
        m_iniBox->setValue(m_iniWithoutModValue + m_iniModifierBox->value());
    });
    connect(m_multipleEnabledBox, &QCheckBox::stateChanged, this, [this] {
        m_instanceNumberBox->setEnabled(m_multipleEnabledBox->checkState() == Qt::Checked);
    });

    connect(m_storeTemplatesButton, &QPushButton::clicked, this, &AddCharacterDialog::storeTemplatesButtonClicked);

    connect(m_templatesWidget, &TemplatesWidget::characterLoaded, this, &AddCharacterDialog::applyLoadedCharacterToUI);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(openTemplatesButton, &QPushButton::clicked, this, [this, openTemplatesButton] {
        const auto isVisible = m_storeTemplatesButton->isVisible();
        openTemplatesButton->setText(isVisible ? "Templates >>" : "Templates <<");
        manageTemplatesVisibility(!isVisible);
    });
    connect(saveButton, &QPushButton::clicked, this, &AddCharacterDialog::saveButtonClicked);
    connect(resetButton, &QPushButton::clicked, this, &AddCharacterDialog::resetButtonClicked);
    connect(okButton, &QPushButton::clicked, this, &AddCharacterDialog::okButtonClicked);

    connect(m_timer, &QTimer::timeout, this, &AddCharacterDialog::animateLabel);
}


void
AddCharacterDialog::randomButtonClicked()
{
    const auto rand = Utils::General::rollDice();
    m_iniBox->setValue(rand + m_iniModifierBox->value());
    m_rolledValueLabel->setText(tr("Rolled a <b>") + QString::number(rand) + "</b>.");
}


void
AddCharacterDialog::storeTemplatesButtonClicked()
{
    if (m_nameEdit->text().isEmpty()) {
        Utils::General::displayWarningMessageBox(this, tr("Creation not possible!"),
                                                 tr("No name has been set. Please set at least a name before caching the Character!"));
        return;
    }

    const auto character = CharacterHandler::Character(m_nameEdit->text(), m_iniBox->value() - m_iniModifierBox->value(), m_iniModifierBox->value(),
                                                       m_hpBox->value(), m_enemyBox->isChecked(), AdditionalInfoData{ {}, m_addInfoEdit->text() });
    m_templatesWidget->addCharacter(character);
}


void
AddCharacterDialog::manageTemplatesVisibility(bool isVisible)
{
    m_storeTemplatesButton->setVisible(isVisible);
    m_templatesWidget->setVisible(isVisible);
    if (isVisible && m_templatesWidget->getTemplatesCount() == 0) {
        m_templatesWidget->loadTemplates();
    }

    QDialog::adjustSize();
}


void
AddCharacterDialog::saveButtonClicked()
{
    if (m_nameEdit->text().isEmpty()) {
        Utils::General::displayWarningMessageBox(this, tr("Creation not possible!"),
                                                 tr("No name has been set. Please set at least a name before storing the Character!"));
        return;
    }
    const auto numberOfInstances = m_multipleEnabledBox->checkState() == Qt::Checked ? m_instanceNumberBox->value() : 1;
    AdditionalInfoData additionalInfoData{ {}, m_addInfoEdit->text() };

    CharacterHandler::Character character(m_nameEdit->text(), m_iniBox->value(), m_iniModifierBox->value(), m_hpBox->value(),
                                          m_enemyBox->isChecked(), additionalInfoData);
    emit characterCreated(character, numberOfInstances);
    resetButtonClicked();
    m_nameEdit->setFocus(Qt::TabFocusReason);

    // Only set the label text after the first stored character,
    // otherwise it will be displayed constantly until something is stored
    if (!m_isFirstCharStored) {
        m_isFirstCharStored = true;
        m_animatedLabel->setText(tr("Character saved!"));
    }
    // Reset the graphics effect and kickoff the animation
    m_animatedLabel->setGraphicsEffect(0);
    m_timer->start(LABEL_SHOWN_DURATION);
}


void
AddCharacterDialog::resetButtonClicked()
{
    // Reset all displayed values
    m_nameEdit->clear();
    m_iniBox->setValue(0);
    m_iniWithoutModValue = 0;
    m_iniModifierBox->setValue(0);
    m_rolledValueLabel->setText("");
    m_hpBox->setValue(0);
    m_enemyBox->setChecked(false);
    m_addInfoEdit->clear();

    m_multipleEnabledBox->setCheckState(Qt::Unchecked);
    m_instanceNumberBox->setValue(2);
}


void
AddCharacterDialog::okButtonClicked()
{
    if (!m_nameEdit->text().isEmpty()) {
        saveButtonClicked();
    }
    QDialog::accept();
}


void
AddCharacterDialog::applyLoadedCharacterToUI(const CharacterHandler::Character& character)
{
    m_nameEdit->setText(character.name);
    m_iniBox->setValue(character.initiative);
    m_iniModifierBox->setValue(character.modifier);
    m_hpBox->setValue(character.hp);
    m_enemyBox->setChecked(character.isEnemy);
    m_addInfoEdit->setText(character.additionalInfoData.mainInfoText);
}


void
AddCharacterDialog::animateLabel()
{
    // Create a small fading text if a character has been stored
    auto *const effect = new QGraphicsOpacityEffect;
    m_animatedLabel->setGraphicsEffect(effect);

    auto *const animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(LABEL_FADEOUT);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
