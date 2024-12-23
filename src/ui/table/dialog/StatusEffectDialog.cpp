#include "StatusEffectDialog.hpp"

#include "AddCustomEffectDialog.hpp"
#include "RuleSettings.hpp"
#include "StatusEffectData.hpp"
#include "UtilsFiles.hpp"
#include "UtilsGeneral.hpp"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>

StatusEffectDialog::StatusEffectDialog(const RuleSettings& RuleSettings, QWidget *parent) :
    QDialog(parent), m_ruleSettings(RuleSettings)
{
    setWindowTitle(tr("Add Status Effect(s)"));

    m_lineEdit = new QLineEdit;
    auto *const shortcut = new QShortcut(QKeySequence::Find, this);
    connect(shortcut, &QShortcut::activated, this, [this] () {
        m_lineEdit->setFocus(Qt::ShortcutFocusReason);
    });

    m_lineEdit->setPlaceholderText(tr("Type in effect or search (%1)").arg(shortcut->key().toString(QKeySequence::NativeText)));
    m_lineEdit->setToolTip(tr("Selected list items are returned as effect.\n"
                              "If nothing is selected, the entered text will be returned."));

    m_effectFileHandler = std::make_unique<EffectFileHandler>();

    m_treeWidget = new QTreeWidget;
    m_treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_treeWidget->setHeaderHidden(true);

    // Apply standard effects
    auto* const standardItem = new QTreeWidgetItem;
    standardItem->setText(COL_NAME, "Standard Effects:");
    m_treeWidget->addTopLevelItem(standardItem);

    QList<QTreeWidgetItem*> items;
    for (const auto effects = StatusEffectData::getEffectList(m_ruleSettings.ruleset); const auto& effect : effects) {
        auto* const item = new QTreeWidgetItem;
        item->setText(COL_NAME, effect);
        items.append(item);
    }
    standardItem->addChildren(items);
    items.clear();

    // Apply custom effects
    m_customHeaderItem = new QTreeWidgetItem;
    m_customHeaderItem->setText(COL_NAME, "Custom Effects:");
    m_treeWidget->addTopLevelItem(m_customHeaderItem);

    QDirIterator it(m_effectFileHandler->getDirectoryString(), { "*.effect" }, QDir::Files);
    while (it.hasNext()) {
        it.next();

        if (const auto code = m_effectFileHandler->getStatus(it.fileName()); code == 0) {
            const auto effectObject = m_effectFileHandler->getData();
            auto* const item = new QTreeWidgetItem;
            item->setText(COL_NAME, effectObject["name"].toString());
            items.append(item);
        }
    }
    m_customHeaderItem->addChildren(items);
    m_treeWidget->expandAll();

    m_removeEffectButton = new QToolButton;
    m_removeEffectButton->setEnabled(false);
    m_removeEffectButton->setToolTip(tr("Remove a custom effect."));
    m_addEffectButton = new QToolButton;
    m_addEffectButton->setToolTip(tr("Add a custom effect."));
    setButtonIcons();

    auto* const effectButtonLayout = new QHBoxLayout;
    effectButtonLayout->addStretch();
    effectButtonLayout->addWidget(m_removeEffectButton);
    effectButtonLayout->addWidget(m_addEffectButton);

    m_checkBox = new QCheckBox(tr("Permanent"));
    m_checkBox->setTristate(false);
    m_checkBox->setCheckState(Qt::Unchecked);
    m_checkBox->setToolTip(tr("If the effect is permanent, it won't be removed from the Character."));

    m_spinBox = new QSpinBox;
    m_spinBox->setRange(0, 99);
    m_spinBox->setValue(1);

    auto* const spinBoxLabel = new QLabel(tr("Rounds:"));

    auto* const spinBoxLayout = new QHBoxLayout;
    spinBoxLayout->addWidget(m_checkBox);
    spinBoxLayout->addStretch();
    spinBoxLayout->addWidget(spinBoxLabel);
    spinBoxLayout->addWidget(m_spinBox);

    auto *const buttonBox = new QDialogButtonBox;
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    auto *const layout = new QVBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_treeWidget);
    layout->addLayout(effectButtonLayout);
    layout->addLayout(spinBoxLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [this] () {
        findEffect(m_lineEdit->text());
    });
    connect(m_treeWidget, &QTreeWidget::itemDoubleClicked, this, [this, standardItem] (QTreeWidgetItem *item, int /* column */) {
        if (item == standardItem || item == m_customHeaderItem) {
            return;
        }

        createEffect(item->text(COL_NAME));
        QDialog::accept();
    });
    connect(m_treeWidget, &QTreeWidget::itemSelectionChanged, this, &StatusEffectDialog::setRemoveButtonEnabling);
    connect(m_addEffectButton, &QPushButton::clicked, this, &StatusEffectDialog::addEffectButtonClicked);
    connect(m_removeEffectButton, &QPushButton::clicked, this, &StatusEffectDialog::removeEffectButtonClicked);
    connect(m_checkBox, &QCheckBox::stateChanged, this, [this, spinBoxLabel] {
        spinBoxLabel->setEnabled(m_checkBox->checkState() != Qt::Checked);
        m_spinBox->setEnabled(m_checkBox->checkState() != Qt::Checked);
    });
    connect(okButton, &QPushButton::clicked, this, &StatusEffectDialog::okButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void
StatusEffectDialog::setRemoveButtonEnabling()
{
    const auto& selectedItems = m_treeWidget->selectedItems();
    if (selectedItems.size() != 1) {
        m_removeEffectButton->setEnabled(false);
        return;
    }

    QList<QTreeWidgetItem*> customChildrenItem;
    for (auto i = 0; i < m_customHeaderItem->childCount(); i++) {
        customChildrenItem.append(m_customHeaderItem->child(i));
    }

    for (auto i = 0; i < selectedItems.count(); i++) {
        if (!customChildrenItem.contains(selectedItems.at(i))) {
            m_removeEffectButton->setEnabled(false);
            return;
        }
    }
    m_removeEffectButton->setEnabled(true);
}


void
StatusEffectDialog::removeEffectButtonClicked()
{
    const auto& selectedItems = m_treeWidget->selectedItems();
    if (selectedItems.size() != 1) {
        return;
    }

    const auto foundEffect = Utils::Files::findObject(m_effectFileHandler->getDirectoryString(), "*.effect", selectedItems.at(0)->text(COL_NAME));
    if (!foundEffect.has_value()) {
        Utils::General::displayWarningMessageBox(this, tr("Effect not found!"), tr("The Effect was not found on disc!"));
        return;
    }

    if (const auto effectRemoved = Utils::Files::removeFile(foundEffect.value()); !effectRemoved) {
        Utils::General::displayWarningMessageBox(this, tr("Action not possible!"), tr("Could not remove Effect!"));
        return;
    }
    delete selectedItems.at(0);
}


void
StatusEffectDialog::addEffectButtonClicked()
{
    QList<QString> otherEffects;
    for (auto i = 0; i < m_customHeaderItem->childCount(); i++) {
        auto* const customEffectItem = m_customHeaderItem->child(i);
        otherEffects.append(customEffectItem->text(COL_NAME));
    }

    if (auto *const dialog = new AddCustomEffectDialog(otherEffects, this); dialog->exec() == QDialog::Accepted) {
        const auto effectName = dialog->getName();
        if (!m_effectFileHandler->writeToFile(effectName)) {
            Utils::General::displayWarningMessageBox(this, tr("Action not possible!"), tr("The Effect could not be saved!"));
            return;
        }

        auto* const newItem = new QTreeWidgetItem;
        newItem->setText(COL_NAME, effectName);
        m_customHeaderItem->addChild(newItem);
    }
}


void
StatusEffectDialog::okButtonClicked()
{
    // If nothing is selected, add the line edit text as status effect
    if (m_treeWidget->selectedItems().empty() && !m_lineEdit->text().isEmpty()) {
        createEffect(m_lineEdit->text());
    } else {
        // Otherwise, add the effect in the list
        for (auto* const item : m_treeWidget->selectedItems()) {
            createEffect(item->text(COL_NAME));
        }
    }

    // Close if effects were added
    QDialog::accept();
}


void
StatusEffectDialog::createEffect(const QString& effectName)
{
    AdditionalInfoData::StatusEffect effect(effectName, m_checkBox->checkState() == Qt::Checked,
                                            (unsigned int) m_spinBox->value());

    m_effects.push_back(effect);
}


void
StatusEffectDialog::findEffect(const QString& filter)
{
    // Hide effects not containing the filter
    QTreeWidgetItemIterator it(m_treeWidget);
    while (*it) {
        if ((*it)->childCount() == 0) {
            (*it)->setHidden(!(*it)->text(COL_NAME).contains(filter, Qt::CaseInsensitive));
        }
        ++it;
    }
}


void
StatusEffectDialog::setButtonIcons()
{
    const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();
    m_removeEffectButton->setIcon(isSystemInDarkMode ? QIcon(":/icons/table/remove_white.svg") : QIcon(":/icons/table/remove_black.svg"));
    m_addEffectButton->setIcon(isSystemInDarkMode ? QIcon(":/icons/table/add_white.svg") : QIcon(":/icons/table/add_black.svg"));
}


bool
StatusEffectDialog::event(QEvent *event)
{
    [[unlikely]] if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        setButtonIcons();
    }
    return QWidget::event(event);
}
