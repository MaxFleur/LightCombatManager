#include "AddCharacterDialog.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QGraphicsEffect>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QTimer>

#include "StatusEffectDialog.hpp"
#include "Utils.hpp"

AddCharacterDialog::AddCharacterDialog(QWidget *parent)
{
	setWindowTitle(tr("Add new Characters"));

	m_nameEdit = new QLineEdit;
	m_iniBox = new QSpinBox;
	m_iniBox->setMinimum(-20);
	m_iniModifierBox = new QSpinBox;
	m_iniModifierBox->setMinimum(-10);
	m_labelRolled = new QLabel;
	m_hpBox = new QSpinBox;
	m_hpBox->setRange(-10000, 10000);
	m_enemyBox = new QCheckBox;
	m_addInfoEdit = new QLineEdit;

	auto *const rollButton = new QPushButton(tr("Roll random INI value"));
	rollButton->setToolTip(tr("Roll the Initiative. The modifier is added to the rolled value."));

	auto *const addButton = new QPushButton(tr("Add"));
	auto *const resetButton = new QPushButton(tr("Reset all Values"));
	auto *const cancelButton = new QPushButton(tr("Return"));
	auto *const statusEffectButton = new QPushButton(tr("Status Effects"));

	m_animatedLabel = new QLabel;
	m_timer = new QTimer(this);
	m_timer->setSingleShot(true);

	// Labels for character stats
	auto *const nameLabel = new QLabel(tr("Name:"));
	nameLabel->setToolTip(tr("Set the name of the Character. A Character can't be stored without a name."));

	auto *const iniLabel = new QLabel(tr("Initiative:"));
	iniLabel->setToolTip(tr("Set the initiative, including all modifiers. Optional."));

	auto *const iniModifierLabel = new QLabel(tr("Modifier:"));
	iniModifierLabel->setToolTip(tr("Set the modifier of the initiative. Optional."));

	auto *const hpLabel = new QLabel(tr("HP:"));
	hpLabel->setToolTip(tr("Set the HP of this Character. Optional."));

	auto *const enemyLabel = new QLabel(tr("Is Enemy:"));
	enemyLabel->setToolTip(tr("Set if the Character is an enemy. Optional."));

	auto *const addInfoLabel = new QLabel(tr("Add. Info:"));
	addInfoLabel->setToolTip(tr("Set additional information, for example status effects. Optional."));

	auto *const mainLayout = new QGridLayout(this);
	mainLayout->addWidget(nameLabel, 0, 0);
	mainLayout->addWidget(m_nameEdit, 0, 1, 1, 3);

	mainLayout->addWidget(iniLabel, 1, 0);
	mainLayout->addWidget(m_iniBox, 1, 1);
	mainLayout->addWidget(iniModifierLabel, 1, 2);
	mainLayout->addWidget(m_iniModifierBox, 1, 3);

	mainLayout->addWidget(m_labelRolled, 2, 0, 1, 2);
	mainLayout->addWidget(rollButton, 2, 2, 1, 2);

	mainLayout->addWidget(hpLabel, 3, 0);
	mainLayout->addWidget(m_hpBox, 3, 1);
	mainLayout->addWidget(enemyLabel, 3, 2);
	mainLayout->addWidget(m_enemyBox, 3, 3);

	mainLayout->addWidget(addInfoLabel, 4, 0);
	mainLayout->addWidget(m_addInfoEdit, 4, 1, 1, 2);
	mainLayout->addWidget(statusEffectButton, 4, 3);

	mainLayout->addWidget(m_animatedLabel, 5, 0, 1, 2);

	mainLayout->addWidget(addButton, 6, 1);
	mainLayout->addWidget(resetButton, 6, 2);
	mainLayout->addWidget(cancelButton, 6, 3);

	setFocus();

	auto *const saveShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
	auto *const statusEffectShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), this);

	connect(rollButton, &QPushButton::clicked, this, &AddCharacterDialog::setLabelRolled);
	connect(addButton, &QPushButton::clicked, this, &AddCharacterDialog::addCharacter);
	connect(resetButton, &QPushButton::clicked, this, &AddCharacterDialog::resetValues);
	connect(cancelButton, &QPushButton::clicked, this, [this] () {
		m_somethingStored ? QDialog::accept() : QDialog::reject();
	});
	connect(statusEffectButton, &QPushButton::clicked, this, &AddCharacterDialog::openStatusEffectDialog);

	connect(saveShortcut, &QShortcut::activated, this, &AddCharacterDialog::addCharacter);
	connect(statusEffectShortcut, &QShortcut::activated, this, &AddCharacterDialog::openStatusEffectDialog);

	connect(m_timer, &QTimer::timeout, this, &AddCharacterDialog::animateLabel);
}


void
AddCharacterDialog::setLabelRolled()
{
	int rand = Utils::rollDice();
	m_iniBox->setValue(rand + m_iniModifierBox->value());
	m_labelRolled->setText(tr("Rolled number: ") + QString::number(rand));
}


void
AddCharacterDialog::animateLabel()
{
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
AddCharacterDialog::addCharacter()
{
	if (m_nameEdit->text().isEmpty()) {
		auto const reply = QMessageBox::warning(
			this,
			tr("Creation not possible!"),
			tr("No name has been set. Please set at least a name before storing the Character!"));
		return;
	}
	m_somethingStored = true;
	emit characterCreated(m_nameEdit->text(), m_iniBox->value(), m_iniModifierBox->value(), m_hpBox->value(),
			      m_enemyBox->isChecked(), m_addInfoEdit->text());
	resetValues();
	setFocus();

	// Only set the label text after first stored character
	if (!m_isFirstCharStored) {
		m_isFirstCharStored = true;
		m_animatedLabel->setText(tr("Character saved!"));
	}
	// Rest graphics effect and kickoff animation
	m_animatedLabel->setGraphicsEffect(0);
	m_timer->start(LABEL_SHOWN);
}


void
AddCharacterDialog::resetValues()
{
	// Reset all displayed values
	m_nameEdit->clear();
	m_iniBox->setValue(0);
	m_iniModifierBox->setValue(0);
	m_labelRolled->setText("");
	m_hpBox->setValue(0);
	m_enemyBox->setChecked(false);
	m_addInfoEdit->clear();
}


void
AddCharacterDialog::openStatusEffectDialog()
{
	// Open dialog
	auto *const dialog = new StatusEffectDialog(this);
	// Lock until dialog is closed
	if (dialog->exec() == QDialog::Accepted) {
		// If accepted, add status effect
		auto itemText = m_addInfoEdit->text();
		if (!itemText.isEmpty() && itemText.back() != " ") {
			itemText += " ";
		}
		m_addInfoEdit->setText(itemText + dialog->getEffect());
	}
}


// Normally, pressing the Enter or Escape key closes a QDialog, calling reject but we do not want that
// The user has to press the Return or the closing "X" button
void
AddCharacterDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Escape) {
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
