#include "StatusEffectDialog.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QVBoxLayout>

#include "RuleSettings.hpp"
#include "StatusEffectData.hpp"

StatusEffectDialog::StatusEffectDialog(const RuleSettings& RuleSettings, QWidget *parent) :
    QDialog(parent), m_ruleSettings(RuleSettings)
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
    const auto effects = StatusEffectData::getEffectList(m_ruleSettings.ruleset);
    for (const auto& effect : effects) {
        m_listWidget->addItem(new QListWidgetItem(effect));
    }

    m_checkBox = new QCheckBox(tr("Is Permanent"));
    m_checkBox->setTristate(false);
    m_checkBox->setCheckState(Qt::Unchecked);

    m_spinBox = new QSpinBox;
    m_spinBox->setRange(0, 99);
    m_spinBox->setValue(1);

    auto* const spinBoxLabel = new QLabel(tr("Rounds:"));

    auto* const spinBoxLayout = new QHBoxLayout;
    spinBoxLayout->addWidget(spinBoxLabel);
    spinBoxLayout->addWidget(m_spinBox);

    auto* const spinBoxLayoutWidget = new QWidget;
    spinBoxLayoutWidget->setLayout(spinBoxLayout);

    auto *const buttonBox = new QDialogButtonBox;
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    auto *const layout = new QVBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_listWidget);
    layout->addWidget(m_checkBox);
    layout->addWidget(spinBoxLayoutWidget);
    layout->addWidget(buttonBox);
    setLayout(layout);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [this] () {
        findEffect(m_lineEdit->text());
    });
    connect(m_checkBox, &QCheckBox::stateChanged, this, [this, spinBoxLayoutWidget] {
        spinBoxLayoutWidget->setEnabled(m_checkBox->checkState() != Qt::Checked);
    });
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, [this] (QListWidgetItem *item) {
        createEffect(item->text());
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
        createEffect(m_lineEdit->text());
    } else {
        // Otherwise, add the effect in the list
        for (auto* const item : m_listWidget->selectedItems()) {
            createEffect(item->text());
        }
    }

    // Close if effects were added
    QDialog::accept();
}


void
StatusEffectDialog::createEffect(const QString& effectName)
{
    AdditionalInfoData::StatusEffect effect { effectName, m_checkBox->checkState() == Qt::Checked,
                                              (unsigned int) m_spinBox->value() };

    m_effects.push_back(effect);
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
