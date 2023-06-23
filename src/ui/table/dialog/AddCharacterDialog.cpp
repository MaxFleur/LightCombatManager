#include "AddCharacterDialog.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QGraphicsEffect>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QTimer>

#include "AdditionalInfoWidget.hpp"
#include "StatusEffectDialog.hpp"
#include "UtilsGeneral.hpp"

AddCharacterDialog::AddCharacterDialog(QWidget *parent)
{
    setWindowTitle(tr("Add new Character(s)"));

    auto *const nameLabel = new QLabel(tr("Name:"));
    m_nameEdit = new QLineEdit;
    m_nameEdit->setToolTip(tr("Set the name of the Character.\n"
                              "A Character can't be stored without a name."));

    auto *const iniLabel = new QLabel(tr("Initiative:"));
    m_iniBox = new QSpinBox;
    m_iniBox->setMinimum(-20);
    m_iniBox->setToolTip(tr("Set the initiative, including all modifiers. Optional."));

    auto *const iniModifierLabel = new QLabel(tr("Modifier:"));
    m_iniModifierBox = new QSpinBox;
    m_iniModifierBox->setMinimum(-10);
    m_iniModifierBox->setToolTip(tr("Set the initiative modifier. Optional."));

    m_labelRolled = new QLabel;
    auto *const rollButton = new QPushButton(tr("Random"));
    rollButton->setToolTip(tr("Roll a 20 sided dice.\n"
                              "The modifier is added to the rolled value."));

    auto *const hpLabel = new QLabel(tr("HP:"));
    m_hpBox = new QSpinBox;
    m_hpBox->setRange(-10000, 10000);
    m_hpBox->setToolTip(tr("Set the HP of this Character. Optional."));

    auto *const enemyLabel = new QLabel(tr("Is Enemy:"));
    m_enemyBox = new QCheckBox;
    m_enemyBox->setToolTip(tr("Set if the Character is an enemy. Optional."));

    auto *const addInfoLabel = new QLabel(tr("Additional:"));
    m_addInfoEdit = new QLineEdit;
    m_addInfoEdit->setToolTip(tr("Set additional information. \n"
                                 "Status Effects are added in the main table widget."));

    m_instanceNumberBox = new QSpinBox;
    m_instanceNumberBox->setRange(2, 10);
    m_instanceNumberBox->setEnabled(false);

    m_multipleEnabledBox = new QCheckBox(tr("Add Character multiple times:"));
    m_multipleEnabledBox->setTristate(false);
    m_multipleEnabledBox->setCheckState(Qt::Unchecked);
    m_multipleEnabledBox->setToolTip(tr("If this is selected and 'Save' is pressed,\n"
                                        "the Character is added multiple times to the Table."));

    auto *const buttonBox = new QDialogButtonBox;
    auto *const saveButton = buttonBox->addButton(QDialogButtonBox::Save);
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    auto *const resetButton = new QPushButton(tr("Reset"));

    saveButton->setShortcut(QKeySequence::Save);
    const auto saveShortcutText = "Save this Character (" + QKeySequence(QKeySequence::Save).toString() + ").";
    saveButton->setToolTip(tr(saveShortcutText.toLocal8Bit().constData()));

    m_animatedLabel = new QLabel;
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);

    auto *const layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(m_nameEdit, 0, 1, 1, 3);

    layout->addWidget(iniLabel, 1, 0);
    layout->addWidget(m_iniBox, 1, 1);
    layout->addWidget(iniModifierLabel, 1, 2);
    layout->addWidget(m_iniModifierBox, 1, 3);

    layout->addWidget(m_labelRolled, 2, 0, 1, 2);
    layout->addWidget(rollButton, 2, 3, 1, 1);

    layout->addWidget(hpLabel, 4, 0);
    layout->addWidget(m_hpBox, 4, 1);
    layout->addWidget(enemyLabel, 4, 2);
    layout->addWidget(m_enemyBox, 4, 3);

    layout->addWidget(addInfoLabel, 5, 0);
    layout->addWidget(m_addInfoEdit, 5, 1, 1, 3);

    layout->addWidget(m_multipleEnabledBox, 6, 0, 1, 3);
    layout->addWidget(m_instanceNumberBox, 6, 3, 1, 1);

    layout->addWidget(m_animatedLabel, 7, 0, 1, 2);
    layout->addWidget(resetButton, 7, 3, 1, 1);

    // Keep a little space to the button box
    layout->setRowMinimumHeight(8, 20);

    layout->addWidget(buttonBox, 9, 1, 1, 3);

    setLayout(layout);
    m_nameEdit->setFocus(Qt::TabFocusReason);

    connect(rollButton, &QPushButton::clicked, this, &AddCharacterDialog::setLabelRolled);
    connect(saveButton, &QPushButton::clicked, this, &AddCharacterDialog::saveButtonClicked);
    connect(resetButton, &QPushButton::clicked, this, &AddCharacterDialog::resetButtonClicked);
    connect(okButton, &QPushButton::clicked, this, &AddCharacterDialog::okButtonClicked);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(m_timer, &QTimer::timeout, this, &AddCharacterDialog::animateLabel);
    connect(m_multipleEnabledBox, &QCheckBox::stateChanged, this, [this] {
        m_instanceNumberBox->setEnabled(m_multipleEnabledBox->checkState() == Qt::Checked);
    });
}


void
AddCharacterDialog::setLabelRolled()
{
    const auto rand = Utils::General::rollDice();
    m_iniBox->setValue(rand + m_iniModifierBox->value());
    m_labelRolled->setText(tr("Rolled number: ") + QString::number(rand));
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


void
AddCharacterDialog::saveButtonClicked()
{
    if (m_nameEdit->text().isEmpty()) {
        auto const reply = QMessageBox::warning(this, tr("Creation not possible!"),
                                                tr("No name has been set. Please set at least a name before storing the Character!"));
        return;
    }
    m_somethingStored = true;

    const auto numberOfInstances = m_multipleEnabledBox->checkState() == Qt::Checked ? m_instanceNumberBox->value() : 1;
    AdditionalInfoData::AdditionalInformation additionalInformation { m_addInfoEdit->text(), {} };

    CharacterHandler::Character character { m_nameEdit->text(), m_iniBox->value(), m_iniModifierBox->value(), m_hpBox->value(),
                                            m_enemyBox->isChecked(), additionalInformation };
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
    m_iniModifierBox->setValue(0);
    m_labelRolled->setText("");
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


// Normally, pressing the Enter key closes a QDialog, calling reject but we do not want that
// The user has to press Escape, the Return key or the closing "X" button
void
AddCharacterDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(event);
}


void
AddCharacterDialog::closeEvent(QCloseEvent *event)
{
    // Accept means that Characters have been stored, whereas Reject
    // just returns without new Characters stored
    m_somethingStored ? QDialog::accept() : QDialog::reject();
}
