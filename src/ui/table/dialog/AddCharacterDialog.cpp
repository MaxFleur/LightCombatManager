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
#include "Utils.hpp"

AddCharacterDialog::AddCharacterDialog(std::shared_ptr<MainSettings> MainSettings, QWidget *parent) :
	m_mainSettings(MainSettings)
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

	auto *const buttonBox = new QDialogButtonBox;
	auto *const saveButton = buttonBox->addButton(QDialogButtonBox::Save);
	auto *const resetButton = buttonBox->addButton(QDialogButtonBox::Reset);
	auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);

	auto *const statusEffectButton = new QPushButton(tr("Status Effects..."));

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

	layout->addWidget(m_animatedLabel, 5, 0, 1, 2);

	layout->addWidget(buttonBox, 6, 1, 1, 3);

	setLayout(layout);
	setFocus();

	auto *const saveShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
	auto *const statusEffectShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), this);

	connect(rollButton, &QPushButton::clicked, this, &AddCharacterDialog::setLabelRolled);
	connect(saveButton, &QPushButton::clicked, this, &AddCharacterDialog::saveButtonClicked);
	connect(resetButton, &QPushButton::clicked, this, &AddCharacterDialog::resetButtonClicked);
	connect(okButton, &QPushButton::clicked, this, &AddCharacterDialog::okButtonClicked);
	connect(statusEffectButton, &QPushButton::clicked, this, &AddCharacterDialog::openStatusEffectDialog);

	connect(saveShortcut, &QShortcut::activated, this, &AddCharacterDialog::saveButtonClicked);
	connect(statusEffectShortcut, &QShortcut::activated, this, &AddCharacterDialog::openStatusEffectDialog);

	connect(m_timer, &QTimer::timeout, this, &AddCharacterDialog::animateLabel);
}


void
AddCharacterDialog::setLabelRolled()
{
	auto rand = Utils::rollDice();
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
	emit characterCreated(m_nameEdit->text(), m_iniBox->value(), m_iniModifierBox->value(), m_hpBox->value(),
			      m_enemyBox->isChecked(), m_addInfoEdit->text());
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
}


void
AddCharacterDialog::okButtonClicked()
{
	if (!m_nameEdit->text().isEmpty()) {
		saveButtonClicked();
		QDialog::accept();
	}
	QDialog::reject();
}


void
AddCharacterDialog::openStatusEffectDialog()
{
	// Open dialog
	auto *const dialog = new StatusEffectDialog(m_mainSettings, this);
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
