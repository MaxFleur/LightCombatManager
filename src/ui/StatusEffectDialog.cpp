#include "StatusEffectDialog.hpp"

#include <QAbstractItemView>
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>

StatusEffectDialog::StatusEffectDialog(QWidget *parent)
{
	setWindowTitle(tr("Add Status Effect"));

	auto *const textComboBox = new QComboBox;
	textComboBox->setEditable(true);
	textComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	m_list = new QListWidget;
	m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
	for (const auto& effect : m_effects) {
		m_list->addItem(new QListWidgetItem(effect));
	}

	m_addEffectButton = new QPushButton(tr("Add Effect"));

	auto *const cancelButton = new QPushButton(tr("Cancel"));

	auto *const mainLayout = new QGridLayout(this);
	mainLayout->addWidget(new QLabel(tr("Find:")), 0, 0);
	mainLayout->addWidget(textComboBox, 0, 1, 1, 2);
	mainLayout->addWidget(m_list, 1, 0, 1, 3);
	mainLayout->addWidget(m_addEffectButton, 2, 1);
	mainLayout->addWidget(cancelButton, 2, 2);

	setFixedSize(250, 300);

	connect(
		textComboBox->lineEdit(),
		&QLineEdit::textChanged,
		this,
		[this, textComboBox] () {
			findEffect(textComboBox->lineEdit()->text());
		});
	connect(m_addEffectButton, &QPushButton::clicked, this, &StatusEffectDialog::addEffect);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void
StatusEffectDialog::addEffect()
{
	foreach(QListWidgetItem * item, m_list->selectedItems()) {
		m_effect += item->text();
		if (item != m_list->selectedItems().back()) {
			m_effect += ", ";
		}
	}
	// Close if effects were added
	QDialog::accept();
}


void
StatusEffectDialog::findEffect(const QString filter)
{
	// Hide effects not containing the filter
	for (int i = 0; i < m_list->count(); ++i) {
		auto *item = m_list->item(i);
		item->setHidden(!item->text().contains(filter, Qt::CaseInsensitive));
	}
}
