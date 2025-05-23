#include "CombatWidget.hpp"

#include "AddCharacterDialog.hpp"
#include "AdditionalSettings.hpp"
#include "ChangeStatDialog.hpp"
#include "DelegateSpinBox.hpp"
#include "RuleSettings.hpp"
#include "StatusEffectDialog.hpp"
#include "Undo.hpp"
#include "UtilsGeneral.hpp"
#include "UtilsTable.hpp"

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QUndoStack>
#include <QVBoxLayout>

using enum LogListWidget::LoggingType;

CombatWidget::CombatWidget(std::shared_ptr<TableFileHandler> tableFilerHandler,
                           const AdditionalSettings&         AdditionalSettings,
                           const RuleSettings&               RuleSettings,
                           int                               mainWidgetWidth,
                           bool                              isDataStored,
                           QWidget *                         parent) :
    QWidget(parent),
    m_tableFileHandler(tableFilerHandler),
    m_additionalSettings(AdditionalSettings),
    m_ruleSettings(RuleSettings),
    m_isDataStored(std::move(isDataStored))
{
    m_characterHandler = std::make_shared<CharacterHandler>();

    m_undoStack = new QUndoStack(this);

    m_removeCharacterAction = createAction(tr("Remove"), tr("Remove Character(s)"), QKeySequence(Qt::Key_Delete), false);
    m_addCharacterAction = createAction(tr("Add new Character(s)..."), tr("Add new Character(s)"),
                                        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N), true);
    m_insertTableAction = createAction(tr("Insert other Table..."), tr("Insert another table without overwriting the current one"),
                                       QKeySequence(tr("Ctrl+T")), false);
    m_addEffectAction = createAction(tr("Add Status Effect(s)..."), tr("Add Status Effect(s)"), QKeySequence(tr("Ctrl+E")), false);
    m_duplicateAction = createAction(tr("Duplicate"), tr("Duplicate Character"), QKeySequence(tr("Ctrl+D")), false);
    m_rerollAction = createAction(tr("Reroll Initiative"), tr("Reroll Initiative"), QKeySequence(tr("Ctrl+I")), false);
    m_changeHPAction = createAction(tr("Change HP..."), tr("Change HP for multiple Characters at once"), QKeySequence(tr("Ctrl+H")), false);
    m_addInfoTextAction = createAction(tr("Add Info Text..."), tr("Add Info Text for multiple Characters at once"), QKeySequence(tr("Ctrl+J")), false);
    m_resortAction = createAction(tr("Resort Table"), "", QKeySequence(tr("Ctrl+R")), true);
    m_moveUpwardAction = createAction(tr("Move Upward"), "", QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Up), true);
    m_moveDownwardAction = createAction(tr("Move Downward"), "", QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Down), true);
    m_showLogAction = createAction(tr("Show Combat Log"), tr("Show a Log for the current Combat"), QKeySequence(tr("Ctrl+L")), true);

    m_undoAction = m_undoStack->createUndoAction(this, tr("&Undo"));
    m_undoAction->setShortcuts(QKeySequence::Undo);
    m_redoAction = m_undoStack->createRedoAction(this, tr("&Redo"));
    m_redoAction->setShortcuts(QKeySequence::Redo);

    addAction(m_resortAction);
    addAction(m_changeHPAction);
    addAction(m_addInfoTextAction);
    addAction(m_moveUpwardAction);
    addAction(m_moveDownwardAction);
    addAction(m_showLogAction);

    const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();
    setUndoRedoIcon(isSystemInDarkMode);

    auto* const toolBar = new QToolBar("Actions");
    toolBar->addAction(m_removeCharacterAction);
    toolBar->addAction(m_addCharacterAction);
    toolBar->addSeparator();
    toolBar->addAction(m_insertTableAction);
    toolBar->addAction(m_addEffectAction);
    toolBar->addAction(m_resortAction);
    toolBar->addSeparator();
    toolBar->addAction(m_duplicateAction);
    toolBar->addAction(m_rerollAction);
    toolBar->addSeparator();
    toolBar->addAction(m_undoAction);
    toolBar->addAction(m_redoAction);
    // Spacer widget with empty space so that the log action is on the right side
    auto* const spacerWidget = new QWidget;
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(spacerWidget);
    toolBar->addAction(m_showLogAction);

    m_tableWidget = new CombatTableWidget(m_characterHandler, mainWidgetWidth, this);

    // Spinbox for the hp column
    auto *const delegateSpinBox = new DelegateSpinBox(this);
    m_tableWidget->setItemDelegateForColumn(Utils::Table::COL_HP, delegateSpinBox);

    m_logListWidget = new LogListWidget;
    m_logListWidget->setVisible(false);

    auto *const upButton = new QToolButton;
    upButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    upButton->setText(tr("Previous"));
    upButton->setArrowType(Qt::UpArrow);
    upButton->setToolTip(tr("Select the previous Character (Ctrl + Arrow Up)."));
    upButton->setShortcut(QKeySequence(tr("Ctrl+Up")));

    auto *const downButton = new QToolButton;
    downButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    downButton->setText(tr("Next"));
    downButton->setArrowType(Qt::DownArrow);
    downButton->setToolTip(tr("Select the next Character (Ctrl + Arrow Down)."));
    downButton->setShortcut(QKeySequence(tr("Ctrl+Down")));

    auto *const exitButton = new QPushButton(tr("Return to Main Window"));

    m_roundCounterLabel = new QLabel(tr("Current: None"));
    m_currentPlayerLabel = new QLabel(tr("Round 0"));
    m_iniRerolledLabel = new QLabel;

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);

    // Lower layout
    auto *const lowerLayout = new QHBoxLayout;
    lowerLayout->addWidget(m_roundCounterLabel);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(m_currentPlayerLabel);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(m_iniRerolledLabel);
    lowerLayout->addStretch();
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(upButton);
    lowerLayout->addWidget(downButton);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(exitButton);

    auto *const combatLayout = new QVBoxLayout;
    combatLayout->addWidget(toolBar);
    combatLayout->addWidget(m_tableWidget);
    combatLayout->addLayout(lowerLayout);

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addLayout(combatLayout);
    m_mainLayout->addWidget(m_logListWidget);
    setLayout(m_mainLayout);

    connect(this, &CombatWidget::roundCounterSet, this, [this] {
        m_roundCounterLabel->setText(tr("Round ") + QString::number(m_roundCounter));
    });

    connect(m_removeCharacterAction, &QAction::triggered, this, &CombatWidget::removeRow);
    connect(m_addCharacterAction, &QAction::triggered, this, &CombatWidget::openAddCharacterDialog);
    connect(m_insertTableAction, &QAction::triggered, this, &CombatWidget::insertTable);
    connect(m_addEffectAction, &QAction::triggered, this, &CombatWidget::openStatusEffectDialog);
    connect(m_duplicateAction, &QAction::triggered, this, &CombatWidget::duplicateRow);
    connect(m_rerollAction, &QAction::triggered, this, &CombatWidget::rerollIni);
    connect(m_changeHPAction, &QAction::triggered, this, [this] {
        changeStatForMultipleChars(true);
    });
    connect(m_addInfoTextAction, &QAction::triggered, this, [this] {
        changeStatForMultipleChars(false);
    });
    connect(m_resortAction, &QAction::triggered, this, &CombatWidget::sortTable);
    connect(m_moveUpwardAction, &QAction::triggered, this, [this] {
        switchCharacterPosition(false);
    });
    connect(m_moveDownwardAction, &QAction::triggered, this, [this] {
        switchCharacterPosition(true);
    });
    connect(m_showLogAction, &QAction::triggered, this, &CombatWidget::setLoggingWidgetVisibility);

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
        m_removeCharacterAction->setEnabled(m_tableWidget->selectionModel()->hasSelection());
        m_addEffectAction->setEnabled(m_tableWidget->selectionModel()->hasSelection());
        m_duplicateAction->setEnabled(m_tableWidget->selectionModel()->selectedRows().size() == 1);
        m_rerollAction->setEnabled(m_tableWidget->selectionModel()->selectedRows().size() == 1);
        m_changeHPAction->setEnabled(m_tableWidget->selectionModel()->selectedRows().size() > 1);
        m_addInfoTextAction->setEnabled(m_tableWidget->selectionModel()->selectedRows().size() > 1);
    });
    connect(m_tableWidget, &QTableWidget::currentCellChanged, this, [this] {
        saveOldState();
    });
    connect(m_tableWidget, &QTableWidget::itemChanged, this, &CombatWidget::handleTableWidgetItemPressed);
    connect(m_tableWidget->model(), &QAbstractItemModel::rowsInserted, this, [this] {
        m_insertTableAction->setEnabled(m_tableWidget->rowCount() != 0);
    });
    connect(m_tableWidget->model(), &QAbstractItemModel::rowsRemoved, this, [this] {
        m_insertTableAction->setEnabled(m_tableWidget->rowCount() != 0);
    });

    connect(m_undoStack, &QUndoStack::indexChanged, this, &CombatWidget::showLoggingUndoOperations);

    connect(upButton, &QPushButton::clicked, this, [this] {
        enteredRowChanged(false);
    });
    connect(downButton, &QPushButton::clicked, this, [this] {
        enteredRowChanged(true);
    });
    connect(exitButton, &QPushButton::clicked, this, [this] {
        emit exit();
    });

    connect(m_timer, &QTimer::timeout, this, [this] {
        Utils::General::animateLabel(m_iniRerolledLabel);
    });
}


void
CombatWidget::generateTableFromTableData()
{
    if (!m_isDataStored) {
        return;
    }

    // Load the data from file
    const auto& loadedFileData = m_tableFileHandler->getData();
    loadCharactersFromTable(loadedFileData);
    m_rowEntered = loadedFileData.value("row_entered").toInt();
    m_roundCounter = loadedFileData.value("round_counter").toInt();

    m_isDataStored = false;
    m_tableWidget->setColumnHidden(Utils::Table::COL_INI, !m_tableSettings.iniShown);
    m_tableWidget->setColumnHidden(Utils::Table::COL_MODIFIER, !m_tableSettings.modifierShown);

    m_affectedRowIndices.resize(m_characterHandler->getCharacters().size());
    std::iota(m_affectedRowIndices.begin(), m_affectedRowIndices.end(), 0);

    // Then create the table in the ui
    m_logListWidget->logConditionalValue(COUNT, m_characterHandler->getCharacters().size(), true);
    pushOnUndoStack();
    // We do not need a save step directly after table creation
    m_undoStack->clear();
}


bool
CombatWidget::writeTableToFile(const QString& fileName)
{
    return m_tableFileHandler->writeToFile(m_tableWidget->tableDataFromWidget(), fileName, m_rowEntered,
                                           m_roundCounter, m_ruleSettings.ruleset, m_ruleSettings.rollAutomatical);
}


// Save the old table state before a table change occurs (later used for the undo stack)
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
    const Undo::UndoData oldData{ m_tableDataOld, m_rowEnteredOld, m_roundCounterOld };
    if (resynchronize) {
        m_tableWidget->resynchronizeCharacters();
    }
    // Assemble the new data
    const auto tableData = m_tableWidget->tableDataFromCharacterVector();
    const Undo::UndoData newData{ tableData, m_rowEntered, m_roundCounter };
    // We got everything, so push
    m_undoStack->push(new Undo(this, m_logListWidget, m_roundCounterLabel, m_currentPlayerLabel,
                               oldData, newData, m_affectedRowIndices, &m_rowEntered, &m_roundCounter,
                               m_tableSettings.colorTableRows, m_tableSettings.showIniToolTips,
                               m_tableSettings.adjustHeightAfterRemove));
    m_affectedRowIndices.clear();
}


// Set a new width for the name and/or additional info column if longer strings are used
void
CombatWidget::resetNameAndInfoWidth(const int nameWidth, const int addInfoWidth)
{
    auto changeOccured = false;

    // Due to the stretch property, the additional info column will be shortened if the name column
    // is enhanced. To prevent this, we store the old value and reuse it if necessary
    const auto oldAddInfoWidth = m_tableWidget->columnWidth(Utils::Table::COL_ADDITIONAL);

    if (m_tableWidget->columnWidth(Utils::Table::COL_NAME) < nameWidth) {
        m_tableWidget->setColumnWidth(Utils::Table::COL_NAME, nameWidth + COL_LENGTH_BUFFER_NAME);
        changeOccured = true;
    }
    if (oldAddInfoWidth < addInfoWidth) {
        m_tableWidget->setColumnWidth(Utils::Table::COL_ADDITIONAL, addInfoWidth + COL_LENGTH_BUFFER_ADDITIONAL);
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
    m_removeCharacterAction->setIcon(isDarkMode ? QIcon(":/icons/table/remove_white.svg") : QIcon(":/icons/table/remove_black.svg"));
    m_addCharacterAction->setIcon(isDarkMode ? QIcon(":/icons/table/add_white.svg") : QIcon(":/icons/table/add_black.svg"));
    m_insertTableAction->setIcon(isDarkMode ? QIcon(":/icons/table/insert_table_white.svg") : QIcon(":/icons/table/insert_table_black.svg"));
    m_addEffectAction->setIcon(isDarkMode ? QIcon(":/icons/table/effect_white.svg") : QIcon(":/icons/table/effect_black.svg"));
    m_duplicateAction->setIcon(isDarkMode ? QIcon(":/icons/table/duplicate_white.svg") : QIcon(":/icons/table/duplicate_black.svg"));
    m_rerollAction->setIcon(isDarkMode ? QIcon(":/icons/table/reroll_white.svg") : QIcon(":/icons/table/reroll_black.svg"));
    m_changeHPAction->setIcon(isDarkMode ? QIcon(":/icons/table/change_hp_white.svg") : QIcon(":/icons/table/change_hp_black.svg"));
    m_addInfoTextAction->setIcon(isDarkMode ? QIcon(":/icons/table/text_white.svg") : QIcon(":/icons/table/text_black.svg"));
    m_resortAction->setIcon(isDarkMode ? QIcon(":/icons/table/sort_white.svg") : QIcon(":/icons/table/sort_black.svg"));
    m_undoAction->setIcon(isDarkMode ? QIcon(":/icons/table/undo_white.svg") : QIcon(":/icons/table/undo_black.svg"));
    m_redoAction->setIcon(isDarkMode ? QIcon(":/icons/table/redo_white.svg") : QIcon(":/icons/table/redo_black.svg"));
    m_moveUpwardAction->setIcon(isDarkMode ? QIcon(":/icons/table/move_up_white.svg") : QIcon(":/icons/table/move_up_black.svg"));
    m_moveDownwardAction->setIcon(isDarkMode ? QIcon(":/icons/table/move_down_white.svg") : QIcon(":/icons/table/move_down_black.svg"));
    m_showLogAction->setIcon(isDarkMode ? QIcon(":/icons/table/log_white.svg") : QIcon(":/icons/table/log_black.svg"));
}


void
CombatWidget::openAddCharacterDialog()
{
    // Resynchronize because the table could have been modified
    m_tableWidget->resynchronizeCharacters();
    const auto sizeBeforeDialog = m_characterHandler->getCharacters().size();

    auto *const dialog = new AddCharacterDialog(m_additionalSettings.modAddedToIni, this);
    connect(dialog, &AddCharacterDialog::characterCreated, this, [this, &sizeBeforeDialog] (CharacterHandler::Character character, int instanceCount) {
        addCharacter(character, instanceCount);
        emit tableHeightSet(m_tableWidget->getHeight() + Utils::Table::HEIGHT_BUFFER);

        m_logListWidget->logConditionalValue(COUNT, m_characterHandler->getCharacters().size() - sizeBeforeDialog, true);
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
    m_logListWidget->logTwoValues(SWITCH, oldVisualIndex + 1, newVisualIndex + 1);
    pushOnUndoStack();

    // Highlight the row which has been dragged
    m_tableWidget->clearSelection();
    m_tableWidget->selectRow(newVisualIndex);
}


void
CombatWidget::insertTable()
{
    const auto fileName = QFileDialog::getOpenFileName(this, "Insert other Table", "", ("lcm File(*.lcm)"));
    if (fileName.isEmpty()) {
        return;
    }

    switch (const auto code = m_tableFileHandler->getStatus(fileName); code) {
    case 0:
    {
        saveOldState();
        const auto oldSize = m_characterHandler->getCharacters().size();
        loadCharactersFromTable(m_tableFileHandler->getData());
        // Update to new table size and reset identifiers
        m_affectedRowIndices.resize(m_affectedRowIndices.size() + m_characterHandler->getCharacters().size() - oldSize);
        std::iota(m_affectedRowIndices.begin(), m_affectedRowIndices.end(), oldSize);

        pushOnUndoStack();
        emit tableHeightSet(m_tableWidget->getHeight() + Utils::Table::HEIGHT_BUFFER);

        auto const reply = QMessageBox::question(this, tr("Sort Table?"), tr("Do you want to resort the Table?"),
                                                 QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            sortTable();
        }

        break;
    }
    case 1:
    {
        QMessageBox::critical(this, tr("Wrong Table format!"),
                              tr("The loading of the Table failed because the Table has the wrong format."));
        break;
    }
    case 2:
        QMessageBox::critical(this, tr("Failure reading File!"),
                              tr("The file reading failed. Make sure that the file is accessible and readable."));
        break;
    }
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
                // Store normally for DnD 5E
                if (m_ruleSettings.ruleset == RuleSettings::Ruleset::DND_5E) {
                    statusEffects.push_back(dialogEffect);
                    needsUndo = true;
                    continue;
                }
                // For the others only if it's new or the duration is bigger
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
            m_logListWidget->logSingleValue(INFO_EFFECT, m_tableWidget->selectionModel()->selectedRows().size());
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
    for (auto i = 0; i < instanceCount; i++) {
        m_characterHandler->storeCharacter(instanceCount > 1 && m_additionalSettings.indicatorMultipleChars ? trimmedName + " #" + QString::number(i + 1)
                                                                                                            : trimmedName,
                                           instanceCount > 1 && m_additionalSettings.rollIniMultipleChars ? Utils::General::rollDice() + character.modifier
                                                                                                          : character.initiative,
                                           character.modifier, character.hp, character.isEnemy, character.additionalInfoData);
        m_affectedRowIndices.emplace_back(m_characterHandler->getCharacters().size() - 1);
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
    m_logListWidget->logSingleValue(INI, row + 1);
    pushOnUndoStack();

    // Reset the graphics effect and kickoff the animation
    m_iniRerolledLabel->setGraphicsEffect(0);
    m_iniRerolledLabel->setText(tr("New Value: ") + QString::number(newInitiative) + tr(" (Rolled ") +
                                QString::number(newRolledDice) + tr(" + Mod ") + QString::number(characters.at(row).modifier) + ").");
    m_timer->start(LABEL_SHOWN_DURATION);

    m_tableWidget->itemSelectionChanged();
}


void
CombatWidget::changeStatForMultipleChars(bool changeHP)
{
    if (m_tableWidget->selectionModel()->selectedRows().size() < 2) {
        return;
    }

    if (auto *const dialog = new ChangeStatDialog(changeHP, this); dialog->exec() == QDialog::Accepted) {
        const auto variant = dialog->getVariant();
        if (variant.isNull()) {
            return;
        }

        saveOldState();
        m_tableWidget->resynchronizeCharacters();

        for (auto& characters = m_characterHandler->getCharacters();
             const auto& index : m_tableWidget->selectionModel()->selectedRows()) {
            if (changeHP) {
                characters[index.row()].hp = std::clamp(characters[index.row()].hp + variant.toInt(), -10000, 10000);
            } else {
                characters[index.row()].additionalInfoData.mainInfoText += " " + variant.toString();
            }
        }

        m_logListWidget->logConditionalValue(MULTIPLE_CHARS, m_tableWidget->selectionModel()->selectedRows().size(), changeHP);
        pushOnUndoStack();
    }
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
        m_affectedRowIndices.emplace_back(index.row());
    }
    // Sort reversed so items in the vector can be removed without using offsets
    std::ranges::sort(m_affectedRowIndices, std::ranges::greater());

    for (auto& characters = m_characterHandler->getCharacters(); const auto& index : m_affectedRowIndices) {
        // If the deleted row is before the current entered row, move one up
        if (index < (int) m_rowEntered) {
            m_rowEntered--;
        } else if (index == m_tableWidget->rowCount() - 1 && m_tableWidget->item(index, Utils::Table::COL_NAME)->font().bold()) {
            // If the deleted row was the last one in the table and also the current player, select to the first row
            m_rowEntered = 0;
        }

        characters.remove(index);
    }

    // Reverse for the undo stack removal operation
    std::ranges::reverse(m_affectedRowIndices);

    // Update the current player row and table
    m_logListWidget->logConditionalValue(COUNT, (int) m_affectedRowIndices.size(), false);
    setRowAndPlayer();
    pushOnUndoStack();
    m_tableWidget->itemSelectionChanged();

    if (m_tableSettings.adjustHeightAfterRemove) {
        emit tableHeightSet(m_tableWidget->getHeight() + Utils::Table::HEIGHT_BUFFER);
    }
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

    m_affectedRowIndices.emplace_back(currentIndex + 1);
    m_logListWidget->logSingleValue(DUPLICATE, currentIndex + 1);
    pushOnUndoStack();
    m_tableWidget->itemSelectionChanged();
}


void
CombatWidget::handleTableWidgetItemPressed(QTableWidgetItem *item)
{
    if (item->column() == Utils::Table::COL_NAME) {
        const auto nameWidth = Utils::General::getStringWidth(item->text());
        resetNameAndInfoWidth(nameWidth, m_tableWidget->columnWidth(Utils::Table::COL_ADDITIONAL));
    } else if (item->column() == Utils::Table::COL_ENEMY) {
        m_tableWidget->blockSignals(true);
        // We need to store the old checkbox state, so we will reset the state for a short time
        // Then, after saving, reset to the new value and set the correct undo command
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        saveOldState();
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);

        // Need to call here, because pushing on the undo stack will replace the item with a new one
        m_logListWidget->logConditionalValue(ALLY, item->row(), item->checkState() != Qt::Checked);
        pushOnUndoStack(true);

        emit changeOccured();
        m_tableWidget->blockSignals(false);

        return;
    }
    const auto& tableData = m_tableWidget->tableDataFromWidget();

    if (tableData != m_tableDataOld || m_rowEnteredOld != m_rowEntered || m_roundCounterOld != m_roundCounter) {
        m_tableWidget->resynchronizeCharacters();
        m_logListWidget->logSingleValue(static_cast<LogListWidget::LoggingType>(item->column()), item->row());
        pushOnUndoStack();
    }
}


void
CombatWidget::sortTable()
{
    if (m_tableWidget->rowCount() < 2) {
        return;
    }

    saveOldState();
    // Main sorting
    m_tableWidget->resynchronizeCharacters();
    m_characterHandler->sortCharacters(m_ruleSettings.ruleset, m_ruleSettings.rollAutomatical);
    m_rowEntered = 0;
    m_logListWidget->logOther("Table resorted.");
    pushOnUndoStack();
}


void
CombatWidget::setLoggingWidgetVisibility()
{
    static int mainWidth;
    if (!m_logListWidget->isVisible()) {
        mainWidth = width();
    }
    m_logListWidget->setVisible(!m_logListWidget->isVisible());

    emit tableWidthSet(m_logListWidget->isVisible() ? mainWidth + m_mainLayout->spacing() + m_logListWidget->width() : mainWidth);
}


void
CombatWidget::showLoggingUndoOperations(int index)
{
    static auto lastUndoStackIndex = 0;
    static auto oldStackCount = 0;

    if (m_undoStack->count() == 0) {
        return;
    }
    // Log every undo and "undo undo" command
    if (index < lastUndoStackIndex) {
        m_logListWidget->logOther("Undo called.");
    } else if (index > lastUndoStackIndex && oldStackCount == m_undoStack->count()) {
        m_logListWidget->logOther("Redo called.");
    }
    lastUndoStackIndex = index;
    oldStackCount = m_undoStack->count();
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
    m_logListWidget->logTwoValues(SWITCH, originalIndex + 1, goDown ? originalIndex + 2 : originalIndex);
    pushOnUndoStack();

    m_tableWidget->clearSelection();
    // selectRow seems to enable a continued shifting, but the table is not visually highlighted.
    // Not sure why this does not work, but setCurrentIndex seems to do the trick
    m_tableWidget->setCurrentIndex(m_tableWidget->model()->index(originalIndex + indexToSwap, 0));
}


void
CombatWidget::enteredRowChanged(bool isGoingDown)
{
    if (m_tableWidget->rowCount() == 0 || (!isGoingDown && m_rowEntered == 0 && m_roundCounter == 1)) {
        return;
    }

    saveOldState();
    const auto rowEnteredModifier = isGoingDown ? 1 : -1;
    const auto boundaryIndex = isGoingDown ? m_tableWidget->rowCount() - 1 : 0;

    if ((int) m_rowEntered == boundaryIndex) {
        m_rowEntered = isGoingDown ? 0 : m_tableWidget->rowCount() - 1;
        m_roundCounter += isGoingDown ? 1 : -1;
        m_tableWidget->adjustStatusEffectRoundCounter(isGoingDown);

        emit roundCounterSet();
    } else {
        m_rowEntered += rowEnteredModifier;
    }

    // Recreate the table for the updated font
    setRowAndPlayer();
    m_logListWidget->logTwoValues(NEXT_TURN, m_roundCounter, m_rowEntered + 1);
    pushOnUndoStack(true);
}


void
CombatWidget::setTableOption(bool option, int valueType)
{
    switch (valueType) {
    case 0:
        m_tableWidget->setColumnHidden(Utils::Table::COL_INI, !option);
        break;
    case 1:
        m_tableWidget->setColumnHidden(Utils::Table::COL_MODIFIER, !option);
        break;
    case 2:
        m_tableWidget->setTableRowColor(!option);
        break;
    case 3:
        m_tableWidget->setIniColumnTooltips(!option);
        break;
    case 4:
    default:
        break;
    }

    m_tableSettings.write(static_cast<TableSettings::ValueType>(valueType), option);
}


// Load characters stored in a file to the table
void
CombatWidget::loadCharactersFromTable(const QJsonObject& jsonObject)
{
    auto& characters = m_characterHandler->getCharacters();

    // Single character
    for (const auto& charactersObject = jsonObject.value("characters").toObject(); const auto& character : charactersObject) {
        const auto& characterObject = character.toObject();
        const auto& additionalInfoObject = characterObject.value("additional_info").toObject();

        // Additional info
        AdditionalInfoData additionalInfoData;
        additionalInfoData.mainInfoText = additionalInfoObject.value("main_info").toString();

        // Status effects
        for (const auto& statusEffectsObject = additionalInfoObject.value("status_effects").toObject();
             const auto& singleEffect : statusEffectsObject) {
            const auto& singleEffectObject = singleEffect.toObject();

            AdditionalInfoData::StatusEffect effect(singleEffectObject.value("name").toString(),
                                                    singleEffectObject.value("is_permanent").toBool(),
                                                    singleEffectObject.value("duration").toInt());
            additionalInfoData.statusEffects.push_back(effect);
        }

        characters.emplace_back(CharacterHandler::Character {
            characterObject.value("name").toString(), characterObject.value("initiative").toInt(),
            characterObject.value("modifier").toInt(), characterObject.value("hp").toInt(),
            characterObject.value("is_enemy").toBool(), additionalInfoData });
    }
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
    if (currentRow >= 0) {
        menu->addAction(m_removeCharacterAction);
    }
    menu->addAction(m_addCharacterAction);

    if (m_tableWidget->rowCount() > 0) {
        menu->addAction(m_insertTableAction);
    }
    // Map from MainWindow coordinates to Table Widget coordinates
    if (currentRow >= 0) {
        menu->addAction(m_addEffectAction);

        if (m_tableWidget->rowCount() > 1) {
            menu->addAction(m_resortAction);
        }
        menu->addSeparator();

        // Enable only for a single selected character
        if (m_tableWidget->selectionModel()->selectedRows().size() != 0) {
            auto *const editingMenu = menu->addMenu(tr("Editing"));

            if (m_tableWidget->selectionModel()->selectedRows().size() > 1) {
                editingMenu->addAction(m_changeHPAction);
                editingMenu->addAction(m_addInfoTextAction);
            } else {
                editingMenu->addAction(m_duplicateAction);
                editingMenu->addAction(m_rerollAction);
                editingMenu->addAction(m_moveUpwardAction);
                editingMenu->addAction(m_moveDownwardAction);

                m_moveUpwardAction->setEnabled(currentRow > 0);
                m_moveDownwardAction->setEnabled(currentRow < m_tableWidget->rowCount() - 1);
            }
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

    auto *const adjustHeightAfterRemoveAction = optionMenu->addAction(tr("Readjust Height after Character Removal"), this, [this] (bool show) {
        setTableOption(show, 4);
    });
    adjustHeightAfterRemoveAction->setCheckable(true);
    adjustHeightAfterRemoveAction->setChecked(m_tableSettings.adjustHeightAfterRemove);

    menu->exec(event->globalPos());
}
