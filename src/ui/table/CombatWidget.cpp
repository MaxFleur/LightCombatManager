#include "CombatWidget.hpp"

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>
#include <QStringList>
#include <QToolButton>
#include <QUndoStack>
#include <QVBoxLayout>

#include "AddCharacterDialog.hpp"
#include "DelegateSpinBox.hpp"
#include "RuleSettings.hpp"
#include "StatusEffectDialog.hpp"
#include "TableSettings.hpp"
#include "Undo.hpp"
#include "UtilsGeneral.hpp"
#include "UtilsTable.hpp"

CombatWidget::CombatWidget(bool isDataStored, std::shared_ptr<RuleSettings> RuleSettings,
                           int mainWidgetWidth, QString data, QWidget *parent)
    : m_isDataStored(isDataStored), m_ruleSettings(RuleSettings), m_loadedFileData(data)
{
    m_char = std::make_shared<CharacterHandler>();
    m_tableSettings = std::make_shared<TableSettings>();

    m_tableWidget = new DisabledNavigationKeyTable();
    m_tableWidget->setColumnCount(NMBR_COLUMNS);

    m_undoStack = new QUndoStack(this);

    QStringList tableHeader;
    tableHeader << tr("Name") << "INI" << "Mod" << "HP" << tr("Is Enemy") << tr("Additional information") << "";

    m_tableWidget->setHorizontalHeaderLabels(tableHeader);
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->verticalHeader()->setVisible(true);
    m_tableWidget->verticalHeader()->setSectionsMovable(true);

    m_tableWidget->setShowGrid(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_tableWidget->setFocusPolicy(Qt::ClickFocus);

    m_tableWidget->setColumnWidth(COL_NAME, mainWidgetWidth * WIDTH_NAME);
    m_tableWidget->setColumnWidth(COL_INI, mainWidgetWidth * WIDTH_INI);
    m_tableWidget->setColumnWidth(COL_MODIFIER, mainWidgetWidth * WIDTH_MODIFIER);
    m_tableWidget->setColumnWidth(COL_HP, mainWidgetWidth * WIDTH_HP);
    m_tableWidget->setColumnWidth(COL_ENEMY, mainWidgetWidth * WIDTH_ENEMY);

    // Spinbox for the hp column
    auto *const delegateSpinBox = new DelegateSpinBox(this);
    m_tableWidget->setItemDelegateForColumn(COL_HP, delegateSpinBox);

    auto *const downButton = new QToolButton;
    downButton->setArrowType(Qt::DownArrow);
    downButton->setToolTip(tr("Select the next Character (Ctrl + Arrow Down)."));
    downButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down));

    auto *const upButton = new QToolButton;
    upButton->setArrowType(Qt::UpArrow);
    upButton->setToolTip(tr("Select the previous Character (Ctrl + Arrow Up)."));
    upButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up));

    auto *const exitButton = new QPushButton(tr("Return to Main Window"));

    m_roundCounterLabel = new QLabel;
    m_currentPlayerLabel = new QLabel;

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
            Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);
            pushOnUndoStack();
        }
    });

    auto *const spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Lower layout
    auto *const lowerLayout = new QHBoxLayout();
    lowerLayout->addWidget(m_roundCounterLabel);
    lowerLayout->addSpacing(SPACING);
    lowerLayout->addWidget(m_currentPlayerLabel);
    lowerLayout->addWidget(spacer);
    lowerLayout->addWidget(upButton);
    lowerLayout->addWidget(downButton);
    lowerLayout->addWidget(exitButton);

    auto *const mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_tableWidget);
    mainLayout->addLayout(lowerLayout);
    setLayout(mainLayout);

    m_undoAction = m_undoStack->createUndoAction(this, tr("&Undo"));
    m_undoAction->setShortcuts(QKeySequence::Undo);
    this->addAction(m_undoAction);

    m_redoAction = m_undoStack->createRedoAction(this, tr("&Redo"));
    m_redoAction->setShortcuts(QKeySequence::Redo);
    this->addAction(m_redoAction);

    auto *const duplicateShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_D), this);
    auto *const deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    auto *const statusEffectShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_E), this);
    auto *const rerollIniShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_I), this);
    auto *const editCombatShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_R), this);

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
}


void
CombatWidget::generateTable()
{
    // Store the data from file
    setTableDataWithFileData();
    m_tableWidget->setColumnHidden(COL_INI, !m_tableSettings->iniShown);
    m_tableWidget->setColumnHidden(COL_MODIFIER, !m_tableSettings->modifierShown);

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
    // In one specific case (when clicking the isEnemy checkboxes), the character vector still contains
    // the old, now outdated state. So in this case, we need to resynchronize the character vector.
    if (resynchronize) {
        Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);
    }
    // Assemble the new data
    const auto tableDataNew = Utils::Table::tableDataFromCharacterVector(m_char);
    const auto newData = Undo::UndoData{ tableDataNew, m_rowEntered, m_roundCounter };
    // We got everything, so push
    m_undoStack->push(new Undo(oldData, newData, this, &m_rowEntered, &m_roundCounter, m_roundCounterLabel, m_currentPlayerLabel));

    // Update table
    emit changeOccured();
}


void
CombatWidget::openAddCharacterDialog()
{
    // Resynchronize because the table could have been modified
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);
    const auto sizeBeforeDialog = m_char->getCharacters().size();

    auto *const dialog = new AddCharacterDialog(m_ruleSettings, this);
    connect(dialog, &AddCharacterDialog::characterCreated, this, [this] (QString name, int ini, int mod, int hp,
                                                                         bool isEnemy, QString addInfo, int instanceCount) {
        addCharacter(name, ini, mod, hp, isEnemy, addInfo, instanceCount);
    });

    if (dialog->exec() == QDialog::Accepted) {
        // Only ask to sort if there are enough chars and additional chars have been added
        if (m_char->getCharacters().size() > 1 && m_char->getCharacters().size() != sizeBeforeDialog) {
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
CombatWidget::dragAndDrop(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    // @note
    // A section moved signal only applies to the header's view, not the model.
    // Also, for some reason, setting the table, which updates the model, seems to trigger the drag & drop an additional time.
    // For this reason, the header is stored when an item is pressed (which is always done before a drag & drop),
    // then reset after the section has been moved. Afterwards, a manual drag & drop is performed.
    m_tableWidget->verticalHeader()->restoreState(m_headerDataState);
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);

    // Switch the character order according to the indices
    auto& characters = m_char->getCharacters();
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
    auto *const dialog = new StatusEffectDialog(m_ruleSettings, this);

    if (dialog->exec() == QDialog::Accepted) {
        saveOldState();
        Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);
        auto& characters = m_char->getCharacters();

        // Add status effect text to characters
        for (const auto& i : m_tableWidget->selectionModel()->selectedRows()) {
            const auto itemText = Utils::General::appendCommaToString(characters.at(i.row()).additionalInf);
            characters[i.row()].additionalInf = itemText + dialog->getEffect();
        }
        // Change table
        pushOnUndoStack();
    }
}


void
CombatWidget::addCharacter(
    QString name,
    int     ini,
    int     mod,
    int     hp,
    bool    isEnemy,
    QString addInfo,
    int     instanceCount)
{
    saveOldState();
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);

    for (int i = 0; i < instanceCount; i++) {
        m_char->storeCharacter(name, ini, mod, hp, isEnemy, addInfo);
    }
    resetNameInfoWidth(name, addInfo);

    pushOnUndoStack();
}


void
CombatWidget::rerollIni()
{
    if (m_tableWidget->selectionModel()->selectedRows().size() != 1) {
        return;
    }

    saveOldState();
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);

    const auto row = m_tableWidget->currentRow();
    auto& characters = m_char->getCharacters();
    const auto newRolledDice = Utils::General::rollDice();
    const auto newInitiative = newRolledDice + characters.at(row).modifier;

    characters[row].initiative = newInitiative;

    // Restore the bold font if the current player gets an ini reroll
    if (row == m_rowEntered) {
        setRowAndPlayer();
    }
    pushOnUndoStack();

    const auto messageString = tr("New initiative value: ") + QString::number(newInitiative) + "<br>" +
                               tr("Rolled dice value: ") + QString::number(newRolledDice) + "<br>" +
                               tr("Modifier: ") + QString::number(characters.at(row).modifier);

    auto const reply = QMessageBox::information(this, tr("Rerolled initiative"), messageString);
}


// Set the characters vector, if the table data has been stored in a csv file
void
CombatWidget::setTableDataWithFileData()
{
    if (!m_isDataStored) {
        return;
    }

    auto& characters = m_char->getCharacters();
    const auto rowOfData = m_loadedFileData.split("\n");
    QStringList rowData;

    // @note For some reason, the splitting of the data creates one empty, obsolete line
    // To ignore this line, decrement the row count and iteration number
    // The second line is the header and also ignored, so decrement again and be at -2
    for (int x = 1; x < rowOfData.size() - 1; x++) {
        rowData = rowOfData.at(x).split(";");

        characters.push_back(CharacterHandler::Character {
            rowData.at(COL_NAME), rowData.at(COL_INI).toInt(), rowData.at(COL_MODIFIER).toInt(),
            rowData.at(COL_HP).toInt(), rowData.at(COL_ENEMY) == "true", rowData.at(COL_ADDITIONAL) });

        // If at the first row (which contains information about the round counter
        // and the player on the move), get this data
        if (x == 1) {
            m_rowEntered = rowData[ROW_ENTERED].toInt();
            m_roundCounter = rowData[ROUND_CTR].toInt();
        }

        resetNameInfoWidth(rowData[COL_NAME], rowData[COL_ADDITIONAL]);
    }
    m_isDataStored = false;
}


void
CombatWidget::sortTable()
{
    saveOldState();
    // Main sorting
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);
    m_char->sortCharacters(m_ruleSettings->ruleset, m_ruleSettings->rollAutomatical);
    m_rowEntered = 0;
    pushOnUndoStack();
}


void
CombatWidget::setRowAndPlayer()
{
    Utils::Table::setRowAndPlayer(m_tableWidget, m_roundCounterLabel, m_currentPlayerLabel, m_rowEntered, m_roundCounter);
}


void
CombatWidget::duplicateRow()
{
    if (m_tableWidget->selectionModel()->selectedRows().size() != 1) {
        return;
    }

    saveOldState();

    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);
    auto& characters = m_char->getCharacters();
    const auto currentIndex = m_tableWidget->currentIndex().row();
    characters.insert(currentIndex + 1, characters.at(currentIndex));

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
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);

    // If rows are selected, indices are stored in the order a user clicked at the rows
    // So we get the selection and resort it so the rows can be easily removed
    std::vector<int> indicesList;
    for (const auto& index : m_tableWidget->selectionModel()->selectedRows()) {
        indicesList.push_back(index.row());
    }
    std::sort(indicesList.begin(), indicesList.end(), [indicesList](const auto& a, const auto& b) {
        // Sort reversed so items in the vector can be removed without using offsets
        return a > b;
    });

    auto& characters = m_char->getCharacters();
    for (const auto& index : indicesList) {
        // If the deleted row is before the current entered row, move one up
        if (index < (int) m_rowEntered) {
            m_rowEntered--;
        }
        // If the deleted row was the last one in the table and also the current player, select to the first row
        if (index == m_tableWidget->rowCount() - 1) {
            if (m_tableWidget->item(index, COL_NAME)->font().bold()) {
                m_rowEntered = 0;
            }
        }

        characters.remove(index);
    }

    // Update the current player row and table
    setRowAndPlayer();
    pushOnUndoStack();
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
        if (m_rowEntered == m_tableWidget->rowCount() - 1) {
            m_rowEntered = 0;
            m_roundCounter++;
            emit roundCounterSet();
            // Otherwise just select the next row
        } else {
            m_rowEntered++;
        }
    } else {
        // Stop for the first round and first char selected
        if (m_rowEntered == 0 && m_roundCounter == 1) {
            return;
        }
        if (m_rowEntered == 0) {
            m_rowEntered = m_tableWidget->rowCount() - 1;
            m_roundCounter--;
            emit roundCounterSet();
        } else {
            m_rowEntered--;
        }
    }

    // Recreate the table for the updated font
    Utils::Table::resynchronizeCharacters(m_tableWidget, m_char);
    setRowAndPlayer();
    pushOnUndoStack();
}


void
CombatWidget::showTablePart(bool show, int valueType)
{
    switch (valueType) {
    case 0:
        m_tableWidget->setColumnHidden(COL_INI, !show);
        m_tableSettings->write(show, TableSettings::ValueType::INI_SHOWN);
        break;
    case 1:
        m_tableWidget->setColumnHidden(COL_MODIFIER, !show);
        m_tableSettings->write(show, TableSettings::ValueType::MOD_SHOWN);
        break;
    default:
        break;
    }
}


// Set a new width for the name and/or additional info column if longer strings are used
void
CombatWidget::resetNameInfoWidth(const QString& name, const QString& addInfo)
{
    auto changeOccured = false;
    // Use a bold font for longer columns
    auto font = QApplication::font();
    font.setBold(true);

    const auto nameWidth = Utils::General::getStringWidth(name, font);
    // Due to the stretch property, the additional info column will be shortened if the name column
    // is enhanced. To prevent this, we store the old value and reuse it if necessary
    auto addInfoWidth = m_tableWidget->columnWidth(COL_ADDITIONAL);

    if (m_tableWidget->columnWidth(COL_NAME) < nameWidth) {
        m_tableWidget->setColumnWidth(COL_NAME, nameWidth + COL_LENGTH_NAME_BUFFER);
        changeOccured = true;
    }
    // additional info might be empty, so possibly skip
    if (!addInfo.isEmpty()) {
        const auto addInfoNewWidth = Utils::General::getStringWidth(addInfo, font);
        if (addInfoWidth < addInfoNewWidth) {
            // Reuse the old value if the new string is too short
            addInfoWidth = addInfoNewWidth + COL_LENGTH_ADD_BUFFER;
            changeOccured = true;
        }
    }

    if (changeOccured) {
        // Change the main window width
        auto mainWidth = 0;
        // Ignore the stretchable and indentifier column
        for (int i = 0; i < m_tableWidget->columnCount() - 2; i++) {
            mainWidth += m_tableWidget->columnWidth(i);
        }
        // The main window will adjust and the additional info column will be lenghtened
        emit tableWidthSet(mainWidth + addInfoWidth + COL_LENGTH_ADD_BUFFER);
    }
}


void
CombatWidget::contextMenuEvent(QContextMenuEvent *event)
{
    auto *const menu = new QMenu(this);

    // Map from MainWindow coordinates to Table Widget coordinates
    if (m_tableWidget->indexAt(m_tableWidget->viewport()->mapFrom(this, event->pos())).row() >= 0) {
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

        menu->addSeparator();
    }

    menu->addAction(m_undoAction);
    menu->addAction(m_redoAction);

    menu->addSeparator();

    auto *const openAddCharacterDialogAction = menu->addAction(tr("Add new Character(s)..."), this, [this] () {
        openAddCharacterDialog();
    });
    openAddCharacterDialogAction->setShortcut(Qt::CTRL | Qt::Key_R);
    openAddCharacterDialogAction->setShortcutVisibleInContextMenu(true);

    if (m_tableWidget->rowCount() > 1) {
        auto *const resortAction = menu->addAction(tr("Resort Table"), this, [this] () {
            sortTable();
        });
        menu->addSeparator();
    }

    auto *const optionMenu = menu->addMenu("Options");

    auto *const iniAction = optionMenu->addAction(tr("Show Initiative"), this, [this] (bool show) {
        showTablePart(show, 0);
    });
    iniAction->setCheckable(true);
    iniAction->setChecked(m_tableSettings->iniShown);

    auto *const modifierAction = optionMenu->addAction(tr("Show Modifier"), this, [this] (bool show) {
        showTablePart(show, 1);
    });
    modifierAction->setCheckable(true);
    modifierAction->setChecked(m_tableSettings->modifierShown);

    menu->exec(event->globalPos());
}
