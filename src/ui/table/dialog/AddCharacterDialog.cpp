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

#include "StatusEffectDialog.hpp"
#include "UtilsGeneral.hpp"

AddCharacterDialog::AddCharacterDialog(std::shared_ptr<RuleSettings> RuleSettings, QWidget *parent) :
	m_ruleSettings(RuleSettings)
{
	setWindowTitle(tr("Add new Character(s)"));

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

	m_multipleEnabledBox = new QCheckBox(tr("Add Character multiple times:"));
	m_multipleEnabledBox->setTristate(false);
	m_multipleEnabledBox->setCheckState(Qt::Unchecked);
	QObject::connect(m_multipleEnabledBox, &QCheckBox::stateChanged, this, [this] {
		m_instanceNumberBox->setEnabled(m_multipleEnabledBox->checkState() == Qt::Checked);
	});

	m_instanceNumberBox = new QSpinBox;
	m_instanceNumberBox->setRange(2, 10);
	m_instanceNumberBox->setEnabled(false);

	auto *const rollButton = new QPushButton(tr("Roll random INI value"));
	rollButton->setToolTip(tr("Roll the Initiative. The modifier is added to the rolled value."));

	auto *const buttonBox = new QDialogButtonBox;
	auto *const saveButton = buttonBox->addButton(QDialogButtonBox::Save);
	auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
	auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

	auto *const resetButton = new QPushButton(tr("Reset"));
	auto *const statusEffectButton = new QPushButton(tr("Status Effects..."));

	saveButton->setShortcut(QKeySequence::Save);
	statusEffectButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));

	m_animatedLabel = new QLabel;
	m_timer = new QTimer(this);
	m_timer->setSingleShot(true);

	// Labels for character stats
	auto *const nameLabel = new QLabel(tr("Name:"));
	nameLabel->setToolTip(tr("Set the name of the Character. A Character can't be stored without a name."));

	auto *const iniLabel = new QLabel(tr("Initiative:"));
	iniLabel->setToolTip(tr("Set the initiative, including all modifiers. Optional."));

	auto *const iniModifierLabel = new QLabel(m_ruleSettings->ruleset ==
						  RuleSettings::Ruleset::DND_30E ? tr("DEX value:") : tr("INI Modifier:"));
	iniModifierLabel->setToolTip(m_ruleSettings->ruleset ==
				     RuleSettings::Ruleset::DND_30E ? tr("Set the dexterity value of this character. Optional.") :
				     tr("Set the modifier of the initiative. Optional."));

	auto *const hpLabel = new QLabel(tr("HP:"));
	hpLabel->setToolTip(tr("Set the HP of this Character. Optional."));

	auto *const enemyLabel = new QLabel(tr("Is Enemy:"));
	enemyLabel->setToolTip(tr("Set if the Character is an enemy. Optional."));

	auto *const addInfoLabel = new QLabel(tr("Add. Info:"));
	addInfoLabel->setToolTip(tr("Set additional information, for example status effects. Optional."));

	auto *const layout = new QGridLayout(this);
	layout->addWidget(nameLabel, 0, 0);
	layout->addWidget(m_nameEdit, 0, 1, 1, 3);

	layout->addWidget(iniLabel, 1, 0);
	layout->addWidget(m_iniBox, 1, 1);
	layout->addWidget(iniModifierLabel, 1, 2);
	layout->addWidget(m_iniModifierBox, 1, 3);

	layout->addWidget(m_labelRolled, 2, 0, 1, 2);
	layout->addWidget(rollButton, 2, 2, 1, 2);

	layout->addWidget(hpLabel, 3, 0);
	layout->addWidget(m_hpBox, 3, 1);
	layout->addWidget(enemyLabel, 3, 2);
	layout->addWidget(m_enemyBox, 3, 3);

	layout->addWidget(addInfoLabel, 4, 0);
	layout->addWidget(m_addInfoEdit, 4, 1, 1, 2);
	layout->addWidget(statusEffectButton, 4, 3);

	layout->addWidget(m_multipleEnabledBox, 5, 0, 1, 3);
	layout->addWidget(m_instanceNumberBox, 5, 3, 1, 1);

	layout->addWidget(m_animatedLabel, 6, 0, 1, 2);
	layout->addWidget(resetButton, 6, 3, 1, 1);

	// Keep a little space to the button box
	layout->setRowMinimumHeight(7, 20);

	layout->addWidget(buttonBox, 8, 1, 1, 3);

	setLayout(layout);
	setFocus();

	connect(rollButton, &QPushButton::clicked, this, &AddCharacterDialog::setLabelRolled);
	connect(saveButton, &QPushButton::clicked, this, &AddCharacterDialog::saveButtonClicked);
	connect(resetButton, &QPushButton::clicked, this, &AddCharacterDialog::resetButtonClicked);
	connect(okButton, &QPushButton::clicked, this, &AddCharacterDialog::okButtonClicked);
	connect(statusEffectButton, &QPushButton::clicked, this, &AddCharacterDialog::openStatusEffectDialog);

	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	connect(m_timer, &QTimer::timeout, this, &AddCharacterDialog::animateLabel);
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
		auto const reply = QMessageBox::warning(
			this,
			tr("Creation not possible!"),
			tr("No name has been set. Please set at least a name before storing the Character!"));
		return;
	}
	m_somethingStored = true;

	const auto numberOfInstances = m_multipleEnabledBox->checkState() == Qt::Checked ? m_instanceNumberBox->value() : 1;
	emit characterCreated(m_nameEdit->text(), m_iniBox->value(), m_iniModifierBox->value(), m_hpBox->value(),
			      m_enemyBox->isChecked(), m_addInfoEdit->text(), numberOfInstances);
	resetButtonClicked();
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


void
AddCharacterDialog::openStatusEffectDialog()
{
	// Open dialog
	auto *const dialog = new StatusEffectDialog(m_ruleSettings, this);
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


void
AddCharacterDialog::setFocus()
{
	m_nameEdit->setFocus(Qt::TabFocusReason);
}


// Normally, pressing the Enter or Escape key closes a QDialog, calling reject but we do not want that
// The user has to press Return or the closing "X" button
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
