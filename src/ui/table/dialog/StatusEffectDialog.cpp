#include "StatusEffectDialog.hpp"

#include <QDebug>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QShortcut>
#include <QStringList>
#include <QVBoxLayout>

#include "RuleSettings.hpp"
#include "StatusEffectData.hpp"

StatusEffectDialog::StatusEffectDialog(std::shared_ptr<RuleSettings> RuleSettings, QWidget *parent) :
    m_ruleSettings(RuleSettings)
{
    setWindowTitle(tr("Add Status Effect(s)"));

    m_lineEdit = new QLineEdit(this);
    auto *const shortcut = new QShortcut(QKeySequence::Find, this);
    connect(shortcut, &QShortcut::activated, this, [this] () {
        m_lineEdit->setFocus(Qt::ShortcutFocusReason);
    });

    m_lineEdit->setPlaceholderText(tr("Type in effect or search (%1)").arg(shortcut->key().toString(QKeySequence::NativeText)));
    m_lineEdit->setToolTip(tr("Selected list items are returned as effect.\n"
                              "If nothing is selected, the entered text will be returned."));

    m_listWidget = new QListWidget(this);
    m_listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    const auto effects = StatusEffectData::getEffectList(m_ruleSettings->ruleset);
    for (const auto& effect : effects) {
        m_listWidget->addItem(new QListWidgetItem(effect));
    }

    auto *const buttonBox = new QDialogButtonBox;
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    auto *const layout = new QVBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_listWidget);
    layout->addWidget(buttonBox);
    setLayout(layout);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [this] () {
        findEffect(m_lineEdit->text());
    });
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, [this] (QListWidgetItem *item) {
        m_effect = item->text();
        QDialog::accept();
    });
    connect(okButton, &QPushButton::clicked, this, &StatusEffectDialog::okButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void
StatusEffectDialog::okButtonClicked()
{
    // If nothing is selected, add the line edit text as status effect
    if (m_listWidget->selectedItems().empty() && !m_lineEdit->text().isEmpty()) {
        m_effect = m_lineEdit->text();
        // Otherwise, add the effect in the list
    } else {
        for (auto* const item : m_listWidget->selectedItems()) {
            m_effect += item->text();
            if (item != m_listWidget->selectedItems().back()) {
                m_effect += ", ";
            }
        }
    }
    // Close if effects were added
    QDialog::accept();
}


void
StatusEffectDialog::findEffect(const QString& filter)
{
    // Hide effects not containing the filter
    for (int i = 0; i < m_listWidget->count(); ++i) {
        auto *item = m_listWidget->item(i);
        item->setHidden(!item->text().contains(filter, Qt::CaseInsensitive));
    }
}
