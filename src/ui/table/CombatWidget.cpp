#include "CombatWidget.hpp"

#include "AddCharacterDialog.hpp"
#include "AdditionalSettings.hpp"
#include "DelegateSpinBox.hpp"
#include "RuleSettings.hpp"
#include "StatusEffectDialog.hpp"
#include "Undo.hpp"
#include "UtilsGeneral.hpp"

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QUndoStack>
#include <QVBoxLayout>

CombatWidget::CombatWidget(const AdditionalSettings& AdditionalSettings,
                           const RuleSettings&       RuleSettings,
                           int                       mainWidgetWidth,
                           bool                      isDataStored,
                           const QJsonObject&        data,
                           QWidget *                 parent) :
    QWidget(parent),
    m_additionalSettings(AdditionalSettings),
    m_ruleSettings(RuleSettings),
    m_loadedFileData(data),
    m_isDataStored(std::move(isDataStored))
{
    m_characterHandler = std::make_shared<CharacterHandler>();

    m_undoStack = new QUndoStack(this);

    m_addCharacterAction = createAction(tr("Add new Character(s)..."), tr("Add new Character(s)"),
                                        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N), true);
    m_removeAction = createAction(tr("Remove"), tr("Remove Character(s)"), QKeySequence(Qt::Key_Delete), false);
    m_addEffectAction = createAction(tr("Add Status Effect(s)..."), tr("Add Status Effect(s)"), QKeySequence(Qt::CTRL | Qt::Key_E), false);
    m_duplicateAction = createAction(tr("Duplicate"), tr("Duplicate Character"), QKeySequence(Qt::CTRL | Qt::Key_D), false);
    m_rerollAction = createAction(tr("Reroll Initiative"), tr("Reroll Initiative"), QKeySequence(Qt::CTRL | Qt::Key_I), false);
    m_resortAction = createAction(tr("Resort Table"), "", QKeySequence(Qt::CTRL | Qt::Key_R), true);
    m_moveUpwardAction = createAction(tr("Move Upward"), "", QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Up), true);
    m_moveDownwardAction = createAction(tr("Move Downward"), "", QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Down), true);

    m_undoAction = m_undoStack->createUndoAction(this, tr("&Undo"));
    m_undoAction->setShortcuts(QKeySequence::Undo);
    m_redoAction = m_undoStack->createRedoAction(this, tr("&Redo"));
    m_redoAction->setShortcuts(QKeySequence::Redo);

    addAction(m_resortAction);
    addAction(m_moveUpwardAction);
    addAction(m_moveDownwardAction);

    const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();
    setUndoRedoIcon(isSystemInDarkMode);

    auto* const toolBar = new QToolBar("Actions");
    toolBar->addAction(m_addCharacterAction);
    toolBar->addAction(m_removeAction);
    toolBar->addAction(m_addEffectAction);
    toolBar->addSeparator();
    toolBar->addAction(m_duplicateAction);
    toolBar->addAction(m_rerollAction);
    toolBar->addSeparator();
    toolBar->addAction(m_undoAction);
    toolBar->addAction(m_redoAction);

    m_tableWidget = new CombatTableWidget(m_characterHandler, mainWidgetWidth, this);

    // Spinbox for the hp column
    auto *const delegateSpinBox = new DelegateSpinBox(this);
    m_tableWidget->setItemDelegateForColumn(COL_HP, delegateSpinBox);

    auto *const upButton = new QToolButton;
    upButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    upButton->setText(tr("Previous"));
    upButton->setArrowType(Qt::UpArrow);
    upButton->setToolTip(tr("Select the previous Character (Ctrl + Arrow Up)."));
    upButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up));

    auto *const downButton = new QToolButton;
    downButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    downButton->setText(tr("Next"));
    downButton->setArrowType(Qt::DownArrow);
    downButton->setToolTip(tr("Select the next Character (Ctrl + Arrow Down)."));
    downButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down));

    auto *const exitButton = new QPushButton(tr("Return to Main Window"));

    m_roundCounterLabel = new QLabel(tr("Current: None"));
    m_currentPlayerLabel = new QLabel(tr("Round 0"));

    // Lower layout
    auto *const lowerLayout = new QHBoxLayout();
    lowerLayout->addWidget(m_roundCounterLabel);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(m_currentPlayerLabel);
    lowerLayout->addStretch();
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(upButton);
    lowerLayout->addWidget(downButton);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(exitButton);

    auto *const mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(m_tableWidget);
    mainLayout->addLayout(lowerLayout);
    setLayout(mainLayout);

    connect(this, &CombatWidget::roundCounterSet, this, [this] {
        m_roundCounterLabel->setText(tr("Round ") + QString::number(m_roundCounter));
    });

    connect(m_addCharacterAction, &QAction::triggered, this, &CombatWidget::openAddCharacterDialog);
    connect(m_removeAction, &QAction::triggered, this, &CombatWidget::removeRow);
    connect(m_addEffectAction, &QAction::triggered, this, &CombatWidget::openStatusEffectDialog);
    connect(m_duplicateAction, &QAction::triggered, this, &CombatWidget::duplicateRow);
    connect(m_rerollAction, &QAction::triggered, this, &CombatWidget::rerollIni);
    connect(m_resortAction, &QAction::triggered, this, &CombatWidget::sortTable);
    connect(m_moveUpwardAction, &QAction::triggered, this, [this] {
        switchCharacterPosition(false);
    });
    connect(m_moveDownwardAction, &QAction::triggered, this, [this] {
        switchCharacterPosition(true);
    });

    connect(m_tableWidget->verticalHeader(), &QHeaderView::sectionPressed, this, [this](int logicalIndex) {
        m_tableWidget->clearSelection();
        m_tableWidget->selectRow(logicalIndex);
        saveOldState();
    });
    connect(m_tableWidget->verticalHeader(), &QHeaderView::sectionMoved, this, &CombatWidget::dragAndDrop);
    connect(m_tableWidget, &QTableWidget::cellChanged, this, [this] {
        emit changeOccured();
    });
    connect(m_tableWidget, &QTableWidget::itemPressed, this, [this] {
        saveOldState();
    });
    connect(m_tableWidget, &QTableWidget::itemSelectionChanged, this, [this] {
        m_removeAction->setEnabled(m_tableWidget->selectionModel()->hasSelection());
        m_addEffectAction->setEnabled(m_tableWidget->selectionModel()->hasSelection());
        m_duplicateAction->setEnabled(m_tableWidget->selectionModel()->selectedRows().size() == 1);
        m_rerollAction->setEnabled(m_tableWidget->selectionModel()->selectedRows().size() == 1);
    });
    connect(m_tableWidget, &QTableWidget::currentCellChanged, this, [this] {
        saveOldState();
    });
    connect(m_tableWidget, &QTableWidget::itemChanged, this, &CombatWidget::handleTableWidgetItemPressed);

    connect(upButton, &QPushButton::clicked, this, [this] {
        enteredRowChanged(false);
    });
    connect(downButton, &QPushButton::clicked, this, [this] {
        enteredRowChanged(true);
    });
    connect(exitButton, &QPushButton::clicked, this, [this] {
        emit exit();
    });
}


void
CombatWidget::generateTable()
{
    // Store the data from file
    setTableDataWithFileData();
    m_tableWidget->setColumnHidden(COL_INI, !m_tableSettings.iniShown);
    m_tableWidget->setColumnHidden(COL_MODIFIER, !m_tableSettings.modifierShown);

    for (auto i = 0; i < m_characterHandler->getCharacters().size(); i++) {
        m_removedOrAddedRowIndices.push_back(i);
    }

    // Then create the table
    pushOnUndoStack();
    // We do not need a save step directly after table creation, so reset the stack
    m_undoStack->clear();
}


// Save the old table state before a change occurs, which is important for the undo command
void
CombatWidget::saveOldState()
{
    m_tableDataOld = m_tableWidget->tableDataFromWidget();
    m_rowEnteredOld = m_rowEntered;
    m_roundCounterOld = m_roundCounter;

    m_headerDataState = m_tableWidget->verticalHeader()->saveState();
}


void
CombatWidget::pushOnUndoStack(bool resynchronize)
{
    // Assemble old data
    const auto oldData = Undo::UndoData{ m_tableDataOld, m_rowEnteredOld, m_roundCounterOld };
    if (resynchronize) {
        m_tableWidget->resynchronizeCharacters();
    }
    // Assemble the new data
    const auto tableData = m_tableWidget->tableDataFromCharacterVector();
    const auto newData = Undo::UndoData{ tableData, m_rowEntered, m_roundCounter };
    // We got everything, so push
    m_undoStack->push(new Undo(this, m_roundCounterLabel, m_currentPlayerLabel,
                               oldData, newData, m_removedOrAddedRowIndices, &m_rowEntered, &m_roundCounter,
                               m_tableSettings.colorTableRows, m_tableSettings.showIniToolTips));
    m_removedOrAddedRowIndices.clear();
}


// Set a new width for the name and/or additional info column if longer strings are used
void
CombatWidget::resetNameAndInfoWidth(const int nameWidth, const int addInfoWidth)
{
    auto changeOccured = false;

    // Due to the stretch property, the additional info column will be shortened if the name column
    // is enhanced. To prevent this, we store the old value and reuse it if necessary
    const auto oldAddInfoWidth = m_tableWidget->columnWidth(COL_ADDITIONAL);

    if (m_tableWidget->columnWidth(COL_NAME) < nameWidth) {
        m_tableWidget->setColumnWidth(COL_NAME, nameWidth + COL_LENGTH_BUFFER_NAME);
        changeOccured = true;
    }
    if (oldAddInfoWidth < addInfoWidth) {
        m_tableWidget->setColumnWidth(COL_ADDITIONAL, addInfoWidth + COL_LENGTH_BUFFER_ADDITIONAL);
        changeOccured = true;
    }

    if (changeOccured) {
        // Change the main window width
        auto mainWidth = 0;
        for (int i = 0; i < m_tableWidget->columnCount(); i++) {
            mainWidth += m_tableWidget->columnWidth(i);
        }
        // The main window will adjust and the additional info column will be lenghtened
        emit tableWidthSet(mainWidth + COL_LENGTH_BUFFER_ADDITIONAL);
    }
}


void
CombatWidget::setUndoRedoIcon(bool isDarkMode)
{
    m_addCharacterAction->setIcon(isDarkMode ? QIcon(":/icons/table/add_white.svg") : QIcon(":/icons/table/add_black.svg"));
    m_removeAction->setIcon(isDarkMode ? QIcon(":/icons/table/remove_white.svg") : QIcon(":/icons/table/remove_black.svg"));
    m_addEffectAction->setIcon(isDarkMode ? QIcon(":/icons/table/effect_white.svg") : QIcon(":/icons/table/effect_black.svg"));
    m_duplicateAction->setIcon(isDarkMode ? QIcon(":/icons/table/duplicate_white.svg") : QIcon(":/icons/table/duplicate_black.svg"));
    m_rerollAction->setIcon(isDarkMode ? QIcon(":/icons/table/reroll_white.svg") : QIcon(":/icons/table/reroll_black.svg"));
    m_undoAction->setIcon(isDarkMode ? QIcon(":/icons/table/undo_white.svg") : QIcon(":/icons/table/undo_black.svg"));
    m_redoAction->setIcon(isDarkMode ? QIcon(":/icons/table/redo_white.svg") : QIcon(":/icons/table/redo_black.svg"));
}


void
CombatWidget::openAddCharacterDialog()
{
    // Resynchronize because the table could have been modified
    m_tableWidget->resynchronizeCharacters();
    const auto sizeBeforeDialog = m_characterHandler->getCharacters().size();

    auto *const dialog = new AddCharacterDialog(this);
    connect(dialog, &AddCharacterDialog::characterCreated, this, [this] (CharacterHandler::Character character, int instanceCount) {
        addCharacter(character, instanceCount);
    });

    if (dialog->exec() == QDialog::Accepted) {
        // Only ask to sort if there are enough chars and additional chars have been added
        if (m_characterHandler->getCharacters().size() > 1 && m_characterHandler->getCharacters().size() != sizeBeforeDialog) {
            auto const reply = QMessageBox::question(this, tr("Sort characters?"), tr("Do you want to sort the table?"),
                                                     QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                sortTable();
            }
        }
    }
}


// This function enables drag and drop of table rows
void
CombatWidget::dragAndDrop(int /* logicalIndex */, int oldVisualIndex, int newVisualIndex)
{
    // @note
    // A section moved signal only applies to the header's view, not the model.
    // Also, for some reason, setting the table, which updates the model, seems to trigger the drag & drop an additional time.
    // For this reason, the header is stored when an item is pressed (which is always done before a drag & drop),
    // then reset after the section has been moved. Afterwards, a manual drag & drop is performed.
    m_tableWidget->verticalHeader()->restoreState(m_headerDataState);
    m_tableWidget->resynchronizeCharacters();

    // Switch the character order according to the indices
    auto& characters = m_characterHandler->getCharacters();
    if (oldVisualIndex > newVisualIndex) {
        std::rotate(characters.rend() - oldVisualIndex - 1, characters.rend() - oldVisualIndex, characters.rend() - newVisualIndex);
    } else {
        std::rotate(characters.begin() + oldVisualIndex, characters.begin() + oldVisualIndex + 1, characters.begin() + newVisualIndex + 1);
    }
    // Then set the table
    pushOnUndoStack();

    // Highlight the row which has been dragged
    m_tableWidget->clearSelection();
    m_tableWidget->selectRow(newVisualIndex);
}


void
CombatWidget::openStatusEffectDialog()
{
    if (!m_tableWidget->selectionModel()->hasSelection()) {
        return;
    }

    // Open dialog
    if (auto *const dialog = new StatusEffectDialog(m_ruleSettings, this); dialog->exec() == QDialog::Accepted) {
        if (const auto& effects = dialog->getEffects(); effects.empty()) {
            return;
        }
        saveOldState();
        m_tableWidget->resynchronizeCharacters();
        auto& characters = m_characterHandler->getCharacters();

        auto needsUndo = false;
        // Add status effect text to characters
        for (const auto& i : m_tableWidget->selectionModel()->selectedRows()) {
            auto& statusEffects = characters[i.row()].additionalInfoData.statusEffects;

            for (const auto& dialogEffect : dialog->getEffects()) {
                // Store normally for DnD 5E, for the others only if it's new or the duration is bigger
                if (m_ruleSettings.ruleset == RuleSettings::Ruleset::DND_5E) {
                    statusEffects.push_back(dialogEffect);
                    needsUndo = true;
                    continue;
                }

                auto it = std::find_if(statusEffects.begin(), statusEffects.end(), [dialogEffect] (const auto& statusEffect) {
                    return statusEffect.name == dialogEffect.name;
                });
                if (it != statusEffects.end()) {
                    if (it->duration < dialogEffect.duration) {
                        it->duration = dialogEffect.duration;
                        needsUndo = true;
                    }
                    continue;
                }

                statusEffects.push_back(dialogEffect);
                needsUndo = true;
            }
            if (needsUndo) {
                m_tableWidget->setStatusEffectInWidget(dialog->getEffects(), i.row());
            }
        }
        if (needsUndo) {
            pushOnUndoStack();
        }
    }

    m_tableWidget->itemSelectionChanged();
}


void
CombatWidget::addCharacter(CharacterHandler::Character character, int instanceCount)
{
    saveOldState();
    m_tableWidget->resynchronizeCharacters();

    const auto trimmedName = character.name.trimmed();
    for (int i = 0; i < instanceCount; i++) {
        m_characterHandler->storeCharacter(instanceCount > 1 && m_additionalSettings.indicatorMultipleChars ? trimmedName + " #" + QString::number(i + 1)
                                                                                                            : trimmedName,
                                           instanceCount > 1 && m_additionalSettings.rollIniMultipleChars ? Utils::General::rollDice() + character.modifier
                                                                                                          : character.initiative,
                                           character.modifier, character.hp, character.isEnemy, character.additionalInfoData);
        m_removedOrAddedRowIndices.emplace_back(m_characterHandler->getCharacters().size() - 1);
    }

    pushOnUndoStack();
}


void
CombatWidget::rerollIni()
{
    if (m_tableWidget->selectionModel()->selectedRows().size() != 1) {
        return;
    }

    saveOldState();
    m_tableWidget->resynchronizeCharacters();

    const auto row = m_tableWidget->currentRow();
    auto& characters = m_characterHandler->getCharacters();
    const auto newRolledDice = Utils::General::rollDice();
    const auto newInitiative = newRolledDice + characters.at(row).modifier;

    characters[row].initiative = newInitiative;
    pushOnUndoStack();

    const auto messageString = tr("New initiative value: ") + QString::number(newInitiative) + "<br>" +
                               tr("Rolled dice value: ") + QString::number(newRolledDice) + "<br>" +
                               tr("Modifier: ") + QString::number(characters.at(row).modifier);

    QMessageBox::information(this, tr("Rerolled initiative"), messageString);
    m_tableWidget->itemSelectionChanged();
}


// Remove a row/character of the table
void
CombatWidget::removeRow()
{
    if (!m_tableWidget->selectionModel()->hasSelection()) {
        return;
    }

    saveOldState();
    m_tableWidget->resynchronizeCharacters();

    // Get selected rows indices
    for (const auto& index : m_tableWidget->selectionModel()->selectedRows()) {
        m_removedOrAddedRowIndices.emplace_back(index.row());
    }

    // Sort reversed so items in the vector can be removed without using offsets
    std::sort(m_removedOrAddedRowIndices.begin(), m_removedOrAddedRowIndices.end(), [](const auto& a, const auto& b) {
        return a > b;
    });

    auto& characters = m_characterHandler->getCharacters();
    for (const auto& index : m_removedOrAddedRowIndices) {
        // If the deleted row is before the current entered row, move one up
        if (index < (int) m_rowEntered) {
            m_rowEntered--;
        } else if (index == m_tableWidget->rowCount() - 1 && m_tableWidget->item(index, COL_NAME)->font().bold()) {
            // If the deleted row was the last one in the table and also the current player, select to the first row
            m_rowEntered = 0;
        }

        characters.remove(index);
    }

    // Reverse for the undo stack removal operation
    std::reverse(m_removedOrAddedRowIndices.begin(), m_removedOrAddedRowIndices.end());

    // Update the current player row and table
    setRowAndPlayer();
    pushOnUndoStack();
    m_tableWidget->itemSelectionChanged();
}


void
CombatWidget::duplicateRow()
{
    if (m_tableWidget->selectionModel()->selectedRows().size() != 1) {
        return;
    }

    saveOldState();

    m_tableWidget->resynchronizeCharacters();
    auto& characters = m_characterHandler->getCharacters();
    const auto currentIndex = m_tableWidget->currentIndex().row();
    characters.insert(currentIndex + 1, CharacterHandler::Character(characters.at(currentIndex)));

    m_removedOrAddedRowIndices.emplace_back(currentIndex + 1);
    pushOnUndoStack();
    m_tableWidget->itemSelectionChanged();
}


void
CombatWidget::handleTableWidgetItemPressed(QTableWidgetItem *item)
{
    if (item->column() == COL_ENEMY) {
        m_tableWidget->blockSignals(true);
        // We need to store the old checkbox state, so we will reset the state for a short time
        // Then, after saving, reset to the new value and set the correct undo command
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        saveOldState();
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        pushOnUndoStack(true);

        emit changeOccured();
        m_tableWidget->blockSignals(false);
        return;
    }
    const auto& tableData = m_tableWidget->tableDataFromWidget();

    if (tableData != m_tableDataOld || m_rowEnteredOld != m_rowEntered || m_roundCounterOld != m_roundCounter) {
        m_tableWidget->resynchronizeCharacters();
        pushOnUndoStack();
    }
}


// Set the characters vector, if the table data has been stored in a csv file
void
CombatWidget::setTableDataWithFileData()
{
    if (!m_isDataStored) {
        return;
    }

    m_rowEntered = m_loadedFileData.value("row_entered").toInt();
    m_roundCounter = m_loadedFileData.value("round_counter").toInt();

    auto& characters = m_characterHandler->getCharacters();
    const auto& charactersObject = m_loadedFileData.value("characters").toObject();

    // Single character
    for (const auto& character : charactersObject) {
        const auto& characterObject = character.toObject();
        const auto& additionalInfoObject = characterObject.value("additional_info").toObject();

        // Additional info
        AdditionalInfoData additionalInfoData;
        additionalInfoData.mainInfoText = additionalInfoObject.value("main_info").toString();

        // Status effects
        const auto& statusEffectsObject = additionalInfoObject.value("status_effects").toObject();
        for (const auto& singleEffect : statusEffectsObject) {
            const auto& singleEffectObject = singleEffect.toObject();

            AdditionalInfoData::StatusEffect effect(singleEffectObject.value("name").toString(),
                                                    singleEffectObject.value("is_permanent").toBool(),
                                                    singleEffectObject.value("duration").toInt());
            additionalInfoData.statusEffects.push_back(effect);
        }

        characters.push_back(CharacterHandler::Character {
            characterObject.value("name").toString(), characterObject.value("initiative").toInt(),
            characterObject.value("modifier").toInt(), characterObject.value("hp").toInt(),
            characterObject.value("is_enemy").toBool(), additionalInfoData });
    }
    m_isDataStored = false;
}


void
CombatWidget::sortTable()
{
    if (m_tableWidget->rowCount() <= 1) {
        return;
    }

    saveOldState();
    // Main sorting
    m_tableWidget->resynchronizeCharacters();
    m_characterHandler->sortCharacters(m_ruleSettings.ruleset, m_ruleSettings.rollAutomatical);
    m_rowEntered = 0;
    pushOnUndoStack();
}


void
CombatWidget::setRowAndPlayer() const
{
    m_tableWidget->setRowAndPlayer(m_roundCounterLabel, m_currentPlayerLabel, m_rowEntered);
}


void
CombatWidget::switchCharacterPosition(bool goDown)
{
    // Do not fall out of table bounds
    const auto originalIndex = m_tableWidget->currentRow();
    if ((originalIndex == 0 && !goDown) || (originalIndex == m_tableWidget->rowCount() - 1 && goDown)) {
        return;
    }

    m_tableWidget->resynchronizeCharacters();
    saveOldState();

    auto& characters = m_characterHandler->getCharacters();
    const auto indexToSwap = goDown ? 1 : -1;
    std::iter_swap(characters.begin() + originalIndex, characters.begin() + originalIndex + indexToSwap);

    setRowAndPlayer();
    pushOnUndoStack();

    m_tableWidget->clearSelection();
    // selectRow seems to enable a continued shifting, but the table is not visually highlighted.
    // Not sure why this does not work, but setCurrentIndex seems to do the trick
    m_tableWidget->setCurrentIndex(m_tableWidget->model()->index(originalIndex + indexToSwap, 0));
}


void
CombatWidget::enteredRowChanged(bool goDown)
{
    if (m_tableWidget->rowCount() == 0 || (!goDown && m_rowEntered == 0 && m_roundCounter == 1)) {
        return;
    }

    saveOldState();

    const auto setForBound = [this, goDown] {
        m_rowEntered = goDown ? 0 : m_tableWidget->rowCount() - 1;
        m_roundCounter += goDown ? 1 : -1;
        m_tableWidget->adjustStatusEffectRoundCounter(goDown);

        emit roundCounterSet();
    };

    const auto rowEnteredModifier = goDown ? 1 : -1;
    const auto tableWidgetBoundIndex = goDown ? m_tableWidget->rowCount() - 1 : 0;
    if ((int) m_rowEntered == tableWidgetBoundIndex) {
        setForBound();
    } else {
        m_rowEntered += rowEnteredModifier;
    }

    // Recreate the table for the updated font´
    setRowAndPlayer();
    pushOnUndoStack(true);
}


void
CombatWidget::setTableOption(bool option, int valueType)
{
    switch (valueType) {
    case 0:
        m_tableWidget->setColumnHidden(COL_INI, !option);
        break;
    case 1:
        m_tableWidget->setColumnHidden(COL_MODIFIER, !option);
        break;
    case 2:
        m_tableWidget->setTableRowColor(!option);
        break;
    case 3:
        m_tableWidget->setIniColumnTooltips(!option);
        break;
    default:
        break;
    }

    m_tableSettings.write(static_cast<TableSettings::ValueType>(valueType), option);
}


QAction*
CombatWidget::createAction(const QString& text, const QString& toolTip, const QKeySequence& keySequence, bool enabled)
{
    auto* const action = new QAction(text);
    action->setToolTip(toolTip);
    action->setShortcut(keySequence);
    action->setEnabled(enabled);
    action->setShortcutVisibleInContextMenu(true);

    return action;
}


void
CombatWidget::contextMenuEvent(QContextMenuEvent *event)
{
    auto *const menu = new QMenu(this);
    const auto currentRow = m_tableWidget->indexAt(m_tableWidget->viewport()->mapFrom(this, event->pos())).row();

    menu->addAction(m_addCharacterAction);

    // Map from MainWindow coordinates to Table Widget coordinates
    if (currentRow >= 0) {
        menu->addAction(m_removeAction);
        menu->addAction(m_addEffectAction);

        if (m_tableWidget->rowCount() > 1) {
            menu->addAction(m_resortAction);
        }
        menu->addSeparator();

        // Enable only for a single selected character
        if (m_tableWidget->selectionModel()->selectedRows().size() == 1) {
            auto *const editingMenu = menu->addMenu(tr("Editing"));
            editingMenu->addAction(m_duplicateAction);
            editingMenu->addAction(m_rerollAction);
            editingMenu->addAction(m_moveUpwardAction);
            editingMenu->addAction(m_moveDownwardAction);

            m_moveUpwardAction->setEnabled(currentRow > 0);
            m_moveDownwardAction->setEnabled(currentRow < m_tableWidget->rowCount() - 1);
        }
    }
    menu->addSeparator();

    auto *const optionMenu = menu->addMenu(tr("Table Options"));

    auto *const iniAction = optionMenu->addAction(tr("Show Initiative"), this, [this] (bool show) {
        setTableOption(show, 0);
    });
    iniAction->setCheckable(true);
    iniAction->setChecked(m_tableSettings.iniShown);

    auto *const modifierAction = optionMenu->addAction(tr("Show Modifier"), this, [this] (bool show) {
        setTableOption(show, 1);
    });
    modifierAction->setCheckable(true);
    modifierAction->setChecked(m_tableSettings.modifierShown);

    auto *const colorTableAction = optionMenu->addAction(tr("Color Rows for Friends and Enemies"), this, [this] (bool show) {
        setTableOption(show, 2);
    });
    colorTableAction->setCheckable(true);
    colorTableAction->setChecked(m_tableSettings.colorTableRows);

    auto *const showIniTooltipsAction = optionMenu->addAction(tr("Show Initiative Calculation Tooltips"), this, [this] (bool show) {
        setTableOption(show, 3);
    });
    showIniTooltipsAction->setCheckable(true);
    showIniTooltipsAction->setChecked(m_tableSettings.showIniToolTips);

    menu->exec(event->globalPos());
}
