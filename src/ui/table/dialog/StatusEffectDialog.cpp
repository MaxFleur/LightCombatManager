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

#include "RuleSettings.hpp"
#include "StatusEffectData.hpp"

StatusEffectDialog::StatusEffectDialog(std::shared_ptr<RuleSettings> RuleSettings, QWidget *parent) :
	m_ruleSettings(RuleSettings)
{
	setWindowTitle(tr("Add Status Effect"));

	auto *const textLineEdit = new QLineEdit;
	auto *const shortcut = new QShortcut(QKeySequence::Find, this);
	connect(shortcut, &QShortcut::activated, this, [this, textLineEdit] () {
		textLineEdit->setFocus(Qt::ShortcutFocusReason);
	});

	textLineEdit->setPlaceholderText(tr("Search (%1)").arg(shortcut->key().toString(QKeySequence::NativeText)));

	m_list = new QListWidget;
	m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
	const auto effects = StatusEffectData::getEffectList(m_ruleSettings->ruleset);
	for (const auto& effect : effects) {
		m_list->addItem(new QListWidgetItem(effect));
	}

	auto *const buttonBox = new QDialogButtonBox;
	auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
	auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

	auto *const layout = new QGridLayout(this);
	layout->addWidget(textLineEdit, 0, 0, 1, 3);
	layout->addWidget(m_list, 1, 0, 1, 3);
	layout->addWidget(buttonBox, 2, 1, 1, 2);
	setLayout(layout);

	connect(textLineEdit, &QLineEdit::textChanged, this, [this, textLineEdit] () {
		findEffect(textLineEdit->text());
	});
	connect(okButton, &QPushButton::clicked, this, &StatusEffectDialog::okButtonClicked);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void
StatusEffectDialog::okButtonClicked()
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
