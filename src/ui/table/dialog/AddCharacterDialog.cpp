#include "AddCharacterDialog.hpp"

#include "AdditionalInfoWidget.hpp"
#include "StatusEffectDialog.hpp"
#include "UtilsGeneral.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QTimer>

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

    auto *const buttonBox = new QDialogButtonBox;
    auto *const saveButton = buttonBox->addButton(QDialogButtonBox::Save);
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    auto *const resetButton = new QPushButton(tr("Reset all entered Values"));

    okButton->setShortcut(Qt::Key_Return);

    saveButton->setShortcut(QKeySequence::Save);
    const auto saveShortcutText = "Save this Character (" + QKeySequence(QKeySequence::Save).toString() + ").";
    saveButton->setToolTip(tr(saveShortcutText.toLocal8Bit().constData()));

    m_animatedLabel = new QLabel;
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);

    auto *const layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(m_nameEdit, 0, 1, 1, 3);

    layout->setRowMinimumHeight(1, 12);

    layout->addWidget(iniLabel, 2, 0);
    layout->addWidget(m_iniBox, 2, 1);
    layout->addWidget(iniModifierLabel, 2, 2);
    layout->addWidget(m_iniModifierBox, 2, 3);

    layout->addWidget(randomIniButton, 3, 0, 1, 2);
    layout->addWidget(m_rolledValueLabel, 3, 3);

    layout->setRowMinimumHeight(4, 12);

    layout->addWidget(hpLabel, 5, 0);
    layout->addWidget(m_hpBox, 5, 1);
    layout->addWidget(enemyLabel, 5, 2);
    layout->addWidget(m_enemyBox, 5, 3);

    layout->addWidget(addInfoLabel, 6, 0);
    layout->addWidget(m_addInfoEdit, 6, 1, 1, 3);

    layout->setRowMinimumHeight(7, 12);

    layout->addWidget(m_multipleEnabledBox, 8, 0, 1, 3);
    layout->addWidget(m_instanceNumberBox, 8, 3, 1, 1);

    layout->setRowMinimumHeight(9, 12);

    layout->addWidget(m_animatedLabel, 10, 0, 1, 2);
    layout->addWidget(resetButton, 10, 2, 1, 2);

    layout->setRowMinimumHeight(11, 10);
    layout->addWidget(buttonBox, 12, 1, 1, 3);

    setLayout(layout);
    m_nameEdit->setFocus(Qt::TabFocusReason);

    connect(randomIniButton, &QPushButton::clicked, this, &AddCharacterDialog::randomButtonClicked);
    connect(m_iniBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] {
        m_iniWithoutModValue = m_iniBox->value() - m_iniModifierBox->value();
    });
    connect(m_iniModifierBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] {
        m_iniBox->setValue(m_iniWithoutModValue + m_iniModifierBox->value());
    });

    connect(saveButton, &QPushButton::clicked, this, &AddCharacterDialog::saveButtonClicked);
    connect(resetButton, &QPushButton::clicked, this, &AddCharacterDialog::resetButtonClicked);
    connect(okButton, &QPushButton::clicked, this, &AddCharacterDialog::okButtonClicked);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(m_timer, &QTimer::timeout, this, [this] {
        Utils::General::animateLabel(m_animatedLabel);
    });
    connect(m_multipleEnabledBox, &QCheckBox::stateChanged, this, [this] {
        m_instanceNumberBox->setEnabled(m_multipleEnabledBox->checkState() == Qt::Checked);
    });
}


void
AddCharacterDialog::randomButtonClicked()
{
    const auto rand = Utils::General::rollDice();
    m_iniBox->setValue(rand + m_iniModifierBox->value());
    m_rolledValueLabel->setText(tr("Rolled: <b>") + QString::number(rand) + "</b>.");
}


void
AddCharacterDialog::saveButtonClicked()
{
    if (m_nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Creation not possible!"),
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
