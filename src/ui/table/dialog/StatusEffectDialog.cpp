#include "StatusEffectDialog.hpp"

#include <QDebug>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QShortcut>
#include <QString>
#include <QStringList>

#include "MainSettings.hpp"
#include "StatusEffectData.hpp"

StatusEffectDialog::StatusEffectDialog(std::shared_ptr<MainSettings> MainSettings, QWidget *parent) :
	m_mainSettings(MainSettings)
{
	setWindowTitle(tr("Add Status Effect"));

	auto *const textLineEdit = new QLineEdit;
	textLineEdit->setPlaceholderText(tr("Search (Ctrl + F)"));

	auto * const shortcut = new QShortcut(QKeySequence::Find, this);
	connect(shortcut, &QShortcut::activated, this, [this, textLineEdit] () {
		textLineEdit->setFocus(Qt::ShortcutFocusReason);
	});

	m_list = new QListWidget;
	m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
	const auto effects = StatusEffectData::getEffectList(m_mainSettings->m_ruleset);
	for (const auto& effect : effects) {
		m_list->addItem(new QListWidgetItem(effect));
	}

	auto *const buttonBox = new QDialogButtonBox;
	auto *const saveButton = buttonBox->addButton(QDialogButtonBox::Save);
	auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

	auto *const mainLayout = new QGridLayout(this);
	mainLayout->addWidget(textLineEdit, 0, 0, 1, 3);
	mainLayout->addWidget(m_list, 1, 0, 1, 3);
	mainLayout->addWidget(buttonBox, 2, 1, 1, 2);

	connect(textLineEdit, &QLineEdit::textChanged, this, [this, textLineEdit] () {
		findEffect(textLineEdit->text());
	});
	connect(saveButton, &QPushButton::clicked, this, &StatusEffectDialog::saveButtonClicked);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void
StatusEffectDialog::saveButtonClicked()
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
