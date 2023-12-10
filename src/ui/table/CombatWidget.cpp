#include "CombatWidget.hpp"

#include "AddCharacterDialog.hpp"
#include "AdditionalSettings.hpp"
#include "DelegateSpinBox.hpp"
#include "RuleSettings.hpp"
#include "StatusEffectDialog.hpp"
#include "Undo.hpp"
#include "UtilsGeneral.hpp"
#include "UtilsTable.hpp"

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
#include <QShortcut>
#include <QToolButton>
#include <QUndoStack>
#include <QVBoxLayout>

CombatWidget::CombatWidget(const AdditionalSettings& AdditionalSettings,
                           const RuleSettings&       RuleSettings,
                           int                       mainWidgetWidth,
                           bool                      isDataStored,
                           QString                   data,
                           QWidget *                 parent) :
    QWidget(parent),
    m_additionalSettings(AdditionalSettings),
    m_ruleSettings(RuleSettings),
    m_loadedFileData(data),
    m_isDataStored(std::move(isDataStored))
{
    m_characterHandler = std::make_shared<CharacterHandler>();

    m_tableWidget = new DisabledNavigationKeyTable();
    m_tableWidget->setColumnCount(NMBR_COLUMNS);

    m_undoStack = new QUndoStack(this);

    QStringList tableHeader;
    tableHeader << tr("Name") << "INI" << "Mod" << "HP" << tr("Is Enemy") << tr("Additional Information") << "";

    m_tableWidget->setHorizontalHeaderLabels(tableHeader);
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->verticalHeader()->setVisible(true);
    m_tableWidget->verticalHeader()->setSectionsMovable(true);
    m_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_tableWidget->setShowGrid(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_tableWidget->setFocusPolicy(Qt::ClickFocus);

    m_tableWidget->setColumnWidth(COL_NAME, mainWidgetWidth * WIDTH_NAME);
    m_tableWidget->setColumnWidth(COL_INI, mainWidgetWidth * WIDTH_INI);
    m_tableWidget->setColumnWidth(COL_MODIFIER, mainWidgetWidth * WIDTH_MODIFIER);
    m_tableWidget->setColumnWidth(COL_HP, mainWidgetWidth * WIDTH_HP);
    m_tableWidget->setColumnWidth(COL_ENEMY, mainWidgetWidth * WIDTH_ENEMY);

    m_undoAction = m_undoStack->createUndoAction(this, tr("&Undo"));
    m_undoAction->setShortcuts(QKeySequence::Undo);
    this->addAction(m_undoAction);

    m_redoAction = m_undoStack->createRedoAction(this, tr("&Redo"));
    m_redoAction->setShortcuts(QKeySequence::Redo);
    this->addAction(m_redoAction);

    const auto isSystemInDarkMode = Utils::General::isColorDark(this->palette().color(QPalette::Window));
    setUndoRedoIcon(isSystemInDarkMode);

    // Spinbox for the hp column
    auto *const delegateSpinBox = new DelegateSpinBox(this);
    m_tableWidget->setItemDelegateForColumn(COL_HP, delegateSpinBox);

    auto* const undoButton = new QToolButton;
    undoButton->setToolTip(tr("Undo last action."));
    undoButton->setShortcut(QKeySequence::Undo);
    undoButton->setDefaultAction(m_undoAction);

    auto* const redoButton = new QToolButton;
    redoButton->setToolTip(tr("Redo last action."));
    redoButton->setShortcut(QKeySequence::Redo);
    redoButton->setDefaultAction(m_redoAction);

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

    connect(this, &CombatWidget::roundCounterSet, this, [this] {
        m_roundCounterLabel->setText(tr("Round ") + QString::number(m_roundCounter));
    });

    connect(m_tableWidget, &QTableWidget::itemPressed, this, [this] {
        saveOldState();
    });
    connect(m_tableWidget, &QTableWidget::currentCellChanged, this, [this] {
        saveOldState();
    });
    connect(m_tableWidget, &QTableWidget::itemChanged, this, [this] {
        const auto& tableData = Utils::Table::tableDataFromWidget(m_tableWidget);

        if (tableData != m_tableDataOld || m_rowEnteredOld != m_rowEntered || m_roundCounterOld != m_roundCounter) {
            Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);
            pushOnUndoStack();
        }
    });

    // Lower layout
    auto *const lowerLayout = new QHBoxLayout();
    lowerLayout->addWidget(m_roundCounterLabel);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(m_currentPlayerLabel);
    lowerLayout->addStretch();
    lowerLayout->addWidget(undoButton);
    lowerLayout->addWidget(redoButton);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(upButton);
    lowerLayout->addWidget(downButton);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(exitButton);

    auto *const mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_tableWidget);
    mainLayout->addLayout(lowerLayout);
    setLayout(mainLayout);

    auto *const duplicateShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_D), this);
    auto *const deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    auto *const statusEffectShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_E), this);
    auto *const rerollIniShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_I), this);
    auto *const editCombatShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N), this);
    auto *const resortTableShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_R), this);
    auto *const moveCharacterDownwardShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Down), this);
    auto *const moveCharacterUpwardShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Up), this);

    connect(m_tableWidget->verticalHeader(), &QHeaderView::sectionPressed, this, [this](int logicalIndex) {
        m_tableWidget->clearSelection();
        m_tableWidget->selectRow(logicalIndex);
        saveOldState();
    });
    connect(m_tableWidget->verticalHeader(), &QHeaderView::sectionMoved, this, &CombatWidget::dragAndDrop);
    connect(m_tableWidget, &QTableWidget::cellChanged, this, [this] {
        emit changeOccured();
    });

    connect(upButton, &QPushButton::clicked, this, [this] {
        enteredRowChanged(false);
    });
    connect(downButton, &QPushButton::clicked, this, [this] {
        enteredRowChanged(true);
    });
    connect(exitButton, &QPushButton::clicked, this, [this] {
        emit exit();
    });

    connect(duplicateShortcut, &QShortcut::activated, this, &CombatWidget::duplicateRow);
    connect(deleteShortcut, &QShortcut::activated, this, &CombatWidget::removeRow);
    connect(statusEffectShortcut, &QShortcut::activated, this, &CombatWidget::openStatusEffectDialog);
    connect(rerollIniShortcut, &QShortcut::activated, this, &CombatWidget::rerollIni);
    connect(editCombatShortcut, &QShortcut::activated, this, &CombatWidget::openAddCharacterDialog);
    connect(resortTableShortcut, &QShortcut::activated, this, &CombatWidget::sortTable);
    connect(moveCharacterDownwardShortcut, &QShortcut::activated, this, [this] {
        switchCharacterPosition(true);
    });
    connect(moveCharacterUpwardShortcut, &QShortcut::activated, this, [this] {
        switchCharacterPosition(false);
    });
}


void
CombatWidget::generateTable()
{
    // Store the data from file
    setTableDataWithFileData();
    m_tableWidget->setColumnHidden(COL_INI, !m_tableSettings.iniShown);
    m_tableWidget->setColumnHidden(COL_MODIFIER, !m_tableSettings.modifierShown);

    // Then create the table
    pushOnUndoStack();
    // We do not need a save step directly after table creation, so reset the stack
    m_undoStack->clear();
}


unsigned int
CombatWidget::getHeight() const
{
    auto height = 0;
    for (int i = 0; i < m_tableWidget->rowCount(); i++) {
        height += m_tableWidget->rowHeight(i);
    }
    return height + HEIGHT_BUFFER;
}


// Save the old table state before a change occurs, which is important for the undo command
void
CombatWidget::saveOldState()
{
    m_tableDataOld = Utils::Table::tableDataFromWidget(m_tableWidget);
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
        Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);
    }
    // Assemble the new data
    const auto tableData = Utils::Table::tableDataFromCharacterVector(m_characterHandler);
    const auto newData = Undo::UndoData{ tableData, m_rowEntered, m_roundCounter };
    // We got everything, so push
    m_undoStack->push(new Undo(this, m_roundCounterLabel, m_currentPlayerLabel,
                               oldData, newData, &m_rowEntered, &m_roundCounter,
                               m_tableSettings.colorTableRows, m_tableSettings.showIniToolTips));
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
    m_undoAction->setIcon(isDarkMode ? QIcon(":/icons/undo_white.png") : QIcon(":/icons/undo_black.png"));
    m_redoAction->setIcon(isDarkMode ? QIcon(":/icons/redo_white.png") : QIcon(":/icons/redo_black.png"));
}


void
CombatWidget::openAddCharacterDialog()
{
    // Resynchronize because the table could have been modified
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);
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
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);

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
        saveOldState();
        Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);
        auto& characters = m_characterHandler->getCharacters();

        // Add status effect text to characters
        for (const auto& i : m_tableWidget->selectionModel()->selectedRows()) {
            auto& statusEffects = characters[i.row()].additionalInformation.statusEffects;
            for (const auto& effect : dialog->getEffects()) {
                statusEffects.push_back(effect);
            }
            Utils::Table::setStatusEffectInWidget(m_tableWidget, dialog->getEffects(), i.row());
        }
        // Change table
        pushOnUndoStack();
    }
}


void
CombatWidget::addCharacter(CharacterHandler::Character character, int instanceCount)
{
    saveOldState();
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);

    const auto trimmedName = character.name.trimmed();
    for (int i = 0; i < instanceCount; i++) {
        m_characterHandler->storeCharacter(instanceCount > 1 && m_additionalSettings.indicatorMultipleChars ? trimmedName + " #" + QString::number(i + 1)
                                                                                                            : trimmedName,
                                           instanceCount > 1 && m_additionalSettings.rollIniMultipleChars ? Utils::General::rollDice() + character.modifier
                                                                                                          : character.initiative,
                                           character.modifier, character.hp, character.isEnemy, character.additionalInformation);
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
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);

    const auto row = m_tableWidget->currentRow();
    auto& characters = m_characterHandler->getCharacters();
    const auto newRolledDice = Utils::General::rollDice();
    const auto newInitiative = newRolledDice + characters.at(row).modifier;

    characters[row].initiative = newInitiative;

    // Restore the bold font if the current player gets an ini reroll
    if (row == (int) m_rowEntered) {
        setRowAndPlayer();
    }
    pushOnUndoStack();

    const auto messageString = tr("New initiative value: ") + QString::number(newInitiative) + "<br>" +
                               tr("Rolled dice value: ") + QString::number(newRolledDice) + "<br>" +
                               tr("Modifier: ") + QString::number(characters.at(row).modifier);

    QMessageBox::information(this, tr("Rerolled initiative"), messageString);
}


// Set the characters vector, if the table data has been stored in a csv file
void
CombatWidget::setTableDataWithFileData()
{
    if (!m_isDataStored) {
        return;
    }

    auto& characters = m_characterHandler->getCharacters();
    const auto rowOfData = m_loadedFileData.split("\n");
    QStringList rowData;

    // @note For some reason, the splitting of the data creates one empty, obsolete line
    // To ignore this line, start at index 1
    for (auto x = 1; x < rowOfData.size() - 1; x++) {
        rowData = rowOfData.at(x).split(";");

        const auto additionalInfoData = Utils::General::convertStringToAdditionalInfoData(rowData.at(COL_ADDITIONAL));
        characters.push_back(CharacterHandler::Character {
            rowData.at(COL_NAME), rowData.at(COL_INI).toInt(), rowData.at(COL_MODIFIER).toInt(),
            rowData.at(COL_HP).toInt(), rowData.at(COL_ENEMY) == "true", additionalInfoData });

        // If at the first row (which contains information about the round counter
        // and the player on the move), get this data
        if (x == 1) {
            m_rowEntered = rowData[ROW_ENTERED].toInt();
            m_roundCounter = rowData[ROUND_CTR].toInt();
        }
    }
    m_isDataStored = false;
}


void
CombatWidget::sortTable()
{
    saveOldState();
    // Main sorting
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);
    m_characterHandler->sortCharacters(m_ruleSettings.ruleset, m_ruleSettings.rollAutomatical);
    m_rowEntered = 0;
    pushOnUndoStack();
}


void
CombatWidget::setRowAndPlayer() const
{
    Utils::Table::setRowAndPlayer(m_tableWidget, m_roundCounterLabel, m_currentPlayerLabel, m_rowEntered);
}


void
CombatWidget::duplicateRow()
{
    if (m_tableWidget->selectionModel()->selectedRows().size() != 1) {
        return;
    }

    saveOldState();

    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);
    auto& characters = m_characterHandler->getCharacters();
    const auto currentIndex = m_tableWidget->currentIndex().row();
    characters.insert(currentIndex + 1, CharacterHandler::Character(characters.at(currentIndex)));

    pushOnUndoStack();
}


// Remove a row/character of the table
void
CombatWidget::removeRow()
{
    if (!m_tableWidget->selectionModel()->hasSelection()) {
        return;
    }

    saveOldState();
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);

    // If rows are selected, indices are stored in the order a user clicked at the rows
    // So we get the selection and resort it so the rows can be easily removed
    std::vector<int> indicesList;
    for (const auto& index : m_tableWidget->selectionModel()->selectedRows()) {
        indicesList.emplace_back(index.row());
    }
    std::sort(indicesList.begin(), indicesList.end(), [indicesList](const auto& a, const auto& b) {
        // Sort reversed so items in the vector can be removed without using offsets
        return a > b;
    });

    auto& characters = m_characterHandler->getCharacters();
    for (const auto& index : indicesList) {
        // If the deleted row is before the current entered row, move one up
        if (index < (int) m_rowEntered) {
            m_rowEntered--;
        }
        // If the deleted row was the last one in the table and also the current player, select to the first row
        if (index == m_tableWidget->rowCount() - 1 && m_tableWidget->item(index, COL_NAME)->font().bold()) {
            m_rowEntered = 0;
        }

        characters.remove(index);
    }

    // Update the current player row and table
    setRowAndPlayer();
    pushOnUndoStack();
}


void
CombatWidget::switchCharacterPosition(bool goDown)
{
    // Do not fall out of table bounds
    const auto originalIndex = m_tableWidget->currentRow();
    if ((originalIndex == 0 && !goDown) || (originalIndex == m_tableWidget->rowCount() - 1 && goDown)) {
        return;
    }

    Utils::Table::resynchronizeCharacters(m_tableWidget, m_characterHandler);
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
    if (m_tableWidget->rowCount() == 0) {
        return;
    }

    saveOldState();

    // Are we going down or up?
    if (goDown) {
        // If the current selected row is the last one, reset to the first row
        if ((int) m_rowEntered == m_tableWidget->rowCount() - 1) {
            m_rowEntered = 0;
            m_roundCounter++;
            Utils::Table::adjustStatusEffectRoundCounter(m_tableWidget, true);

            emit roundCounterSet();
            // Otherwise just select the next row
        } else {
            m_rowEntered++;
        }
    } else {
        if (m_rowEntered == 0) {
            // Stop for the first round and first char selected
            if (m_roundCounter == 1) {
                return;
            }
            m_rowEntered = m_tableWidget->rowCount() - 1;
            m_roundCounter--;
            Utils::Table::adjustStatusEffectRoundCounter(m_tableWidget, false);

            emit roundCounterSet();
        } else {
            m_rowEntered--;
        }
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
        Utils::Table::setTableRowColor(m_tableWidget, !option);
        break;
    case 3:
        Utils::Table::setIniColumnTooltips(m_tableWidget, !option);
        break;
    default:
        break;
    }

    m_tableSettings.write(option, static_cast<TableSettings::ValueType>(valueType));
}


void
CombatWidget::contextMenuEvent(QContextMenuEvent *event)
{
    auto *const menu = new QMenu(this);
    const auto currentRow = m_tableWidget->indexAt(m_tableWidget->viewport()->mapFrom(this, event->pos())).row();

    // Map from MainWindow coordinates to Table Widget coordinates
    if (currentRow >= 0) {
        // Status Effect, reroll, duplication and removal only if the cursor is above an item
        auto *const statusEffectAction = menu->addAction(tr("Add Status Effect(s)..."), this, [this] () {
            openStatusEffectDialog();
        });
        statusEffectAction->setShortcut(Qt::CTRL | Qt::Key_E);
        statusEffectAction->setShortcutVisibleInContextMenu(true);

        // Enable only for a single selected character
        if (m_tableWidget->selectionModel()->selectedRows().size() == 1) {
            auto *const rerollIniAction = menu->addAction(tr("Reroll Initiative"), this, &CombatWidget::rerollIni);
            rerollIniAction->setShortcut(Qt::CTRL | Qt::Key_I);
            rerollIniAction->setShortcutVisibleInContextMenu(true);

            auto *const duplicateRowAction = menu->addAction(tr("Duplicate"), this, [this] () {
                duplicateRow();
            });
            duplicateRowAction->setShortcut(Qt::CTRL | Qt::Key_D);
            duplicateRowAction->setShortcutVisibleInContextMenu(true);
        }

        auto *const removeRowAction = menu->addAction(tr("Remove"), this, [this] () {
            removeRow();
        });
        removeRowAction->setShortcut(Qt::Key_Delete);
        removeRowAction->setShortcutVisibleInContextMenu(true);

        if (m_tableWidget->selectionModel()->selectedRows().size() == 1) {
            auto *const moveCharacterUpwardAction = menu->addAction(tr("Move Upward"), this, [this] {
                switchCharacterPosition(false);
            });
            moveCharacterUpwardAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Up));
            moveCharacterUpwardAction->setShortcutVisibleInContextMenu(true);

            auto *const moveCharacterDownwardAction = menu->addAction(tr("Move Downward"), this, [this] {
                switchCharacterPosition(true);
            });
            moveCharacterDownwardAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Down));
            moveCharacterDownwardAction->setShortcutVisibleInContextMenu(true);

            moveCharacterUpwardAction->setEnabled(currentRow > 0);
            moveCharacterDownwardAction->setEnabled(currentRow < m_tableWidget->rowCount() - 1);
        }

        menu->addSeparator();
    }

    menu->addAction(m_undoAction);
    menu->addAction(m_redoAction);

    menu->addSeparator();

    auto *const openAddCharacterDialogAction = menu->addAction(tr("Add new Character(s)..."), this, [this] () {
        openAddCharacterDialog();
    });
    openAddCharacterDialogAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N));
    openAddCharacterDialogAction->setShortcutVisibleInContextMenu(true);

    if (m_tableWidget->rowCount() > 1) {
        auto *const resortAction = menu->addAction(tr("Resort Table"), this, [this] () {
            sortTable();
        });
        resortAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
        resortAction->setShortcutVisibleInContextMenu(true);
        menu->addSeparator();
    }

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
