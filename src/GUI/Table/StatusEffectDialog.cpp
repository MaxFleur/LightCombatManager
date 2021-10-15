#include "../../../include/GUI/Table/StatusEffectDialog.hpp"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>

StatusEffectDialog::StatusEffectDialog(const int rowNumber, QWidget *parent)
	: m_rowNumber(rowNumber)
{
	setWindowTitle("Add Status Effect");

	auto *const textComboBox = new QComboBox;
	textComboBox->setEditable(true);
	textComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	m_list = new QListWidget;
	for (const auto& effect : m_effects) {
		m_list->addItem(new QListWidgetItem(effect));
	}

	auto *const m_addEffectButton = new QPushButton("Add Effect");

	auto *const cancelButton = new QPushButton("Cancel");

	auto *const mainLayout = new QGridLayout(this);
	mainLayout->addWidget(new QLabel("Find:"), 0, 0);
	mainLayout->addWidget(textComboBox, 0, 1, 1, 2);
	mainLayout->addWidget(m_list, 1, 0, 1, 3);
	mainLayout->addWidget(m_addEffectButton, 2, 1);
	mainLayout->addWidget(cancelButton, 2, 2);

	connect(
		textComboBox->lineEdit(),
		&QLineEdit::textChanged,
		this,
		[this, textComboBox] () {
			findEffect(textComboBox->lineEdit()->text());
		});

	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
}


void
StatusEffectDialog::findEffect(const QString filter)
{
	for (int i = 0; i < m_list->count(); ++i) {
		auto *item = m_list->item(i);
		item->setHidden(!item->text().contains(filter, Qt::CaseInsensitive));
	}
}


void
StatusEffectDialog::animateFindClick()
{
	m_addEffectButton->animateClick();
}
