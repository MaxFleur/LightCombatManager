#include "EditCombatDialog.hpp"

#include <QCheckBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

EditCombatDialog::EditCombatDialog(QWidget *parent)
{
	setWindowTitle(tr("Add new Characters"));

	m_nameEdit = new QLineEdit;
	m_iniBox = new QSpinBox;
	m_iniBox->setMinimum(-20);
	m_iniModifierBox = new QSpinBox;
	m_iniModifierBox->setMinimum(-10);
	m_hpBox = new QSpinBox;
	m_hpBox->setRange(-100, 10000);
	m_npcBox = new QCheckBox;
	m_addInfoEdit = new QLineEdit;

	auto *const addCharButton = new QPushButton(tr("Add Character"));
	auto *const resetButton = new QPushButton(tr("Reset"));
	auto *const cancelButton = new QPushButton(tr("Return"));

	auto *const mainLayout = new QGridLayout(this);
	mainLayout->addWidget(new QLabel(tr("Name:")), 0, 0);
	mainLayout->addWidget(m_nameEdit, 0, 1, 1, 3);

	mainLayout->addWidget(new QLabel(tr("Initiative:")), 1, 0);
	mainLayout->addWidget(m_iniBox, 1, 1);
	mainLayout->addWidget(new QLabel(tr("Modifier:")), 1, 2);
	mainLayout->addWidget(m_iniModifierBox, 1, 3);

	mainLayout->addWidget(new QLabel(tr("HP:")), 2, 0);
	mainLayout->addWidget(m_hpBox, 2, 1);
	mainLayout->addWidget(new QLabel(tr("Is NPC:")), 2, 2);
	mainLayout->addWidget(m_npcBox, 2, 3);

	mainLayout->addWidget(new QLabel(tr("Info:")), 3, 0);
	mainLayout->addWidget(m_addInfoEdit, 3, 1, 1, 3);

	mainLayout->addWidget(addCharButton, 4, 1);
	mainLayout->addWidget(resetButton, 4, 2);
	mainLayout->addWidget(cancelButton, 4, 3);

	setFocus();
	setFixedSize(380, 200);

	connect(addCharButton, &QPushButton::clicked, this, &EditCombatDialog::addCharacter);
	connect(resetButton, &QPushButton::clicked, this, &EditCombatDialog::resetLayout);
	connect(
		cancelButton,
		&QPushButton::clicked,
		this,
		[this] () {
			m_somethingStored ? QDialog::accept() : QDialog::reject();
		});
}


void
EditCombatDialog::addCharacter()
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
			      m_npcBox->isChecked(), m_addInfoEdit->text());
	resetLayout();
	setFocus();
}


void
EditCombatDialog::resetLayout()
{
	// Reset all displayed values
	m_nameEdit->clear();
	m_iniBox->setValue(0);
	m_iniModifierBox->setValue(0);
	m_hpBox->setValue(0);
	m_npcBox->setChecked(false);
	m_addInfoEdit->clear();
}


void
EditCombatDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_S) {
		if (event->modifiers() == Qt::ControlModifier) {
			addCharacter();
		}
	}
	QWidget::keyPressEvent(event);
}


void
EditCombatDialog::closeEvent(QCloseEvent *event)
{
	// Accept means that a Characters were stored, whereas Reject
	// just returns without new Characters stored
	m_somethingStored ? QDialog::accept() : QDialog::reject();
}
