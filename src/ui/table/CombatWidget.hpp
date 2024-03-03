#pragma once

#include "CharacterHandler.hpp"
#include "CombatTableWidget.hpp"
#include "TableSettings.hpp"

#include <QJsonObject>
#include <QPointer>
#include <QWidget>

class QAction;
class QLabel;
class QUndoStack;

class AdditionalSettings;
class RuleSettings;

// This class handles the main combat widget
class CombatWidget : public QWidget {
    Q_OBJECT

public:
    CombatWidget(const AdditionalSettings& AdditionalSettings,
                 const RuleSettings&       RuleSettings,
                 int                       mainWidgetWidth,
                 bool                      isDataStored,
                 const QJsonObject&        data = {},
                 QWidget *                 parent = 0);

    void
    generateTable();

    [[nodiscard]] CombatTableWidget*
    getCombatTableWidget() const
    {
        return m_tableWidget;
    }

    [[nodiscard]] unsigned int
    getRowEntered() const
    {
        return m_rowEntered;
    }

    [[nodiscard]] unsigned int
    getRoundCounter() const
    {
        return m_roundCounter;
    }

    [[nodiscard]] bool
    isEmpty() const
    {
        return m_tableWidget->rowCount() == 0;
    }

    [[nodiscard]] unsigned int
    getHeight() const
    {
        return m_tableWidget->getHeight();
    }

    void
    saveOldState();

    void
    pushOnUndoStack(bool resynchronize = false);

    void
    resetNameAndInfoWidth(const int nameWidth,
                          const int addInfoWidth);

    void
    setUndoRedoIcon(bool isDarkMode);

public slots:
    void
    openAddCharacterDialog();

signals:
    void
    exit();

    void
    tableWidthSet(unsigned int width);

    void
    tableHeightSet(unsigned int height);

    void
    setCurrentPlayer();

    void
    roundCounterSet();

    void
    changeOccured();

private slots:
    void
    dragAndDrop(int logicalIndex,
                int oldVisualIndex,
                int newVisualIndex);

    void
    openStatusEffectDialog();

    void
    addCharacter(CharacterHandler::Character character,
                 int                         instanceCount);

    void
    rerollIni();

    void
    handleTableWidgetItemPressed(QTableWidgetItem *item);

private:
    void
    setTableDataWithFileData();

    void
    sortTable();

    void
    setRowAndPlayer() const;

    void
    duplicateRow();

    void
    removeRow();

    void
    switchCharacterPosition(bool goDown);

    void
    enteredRowChanged(bool goDown);

    void
    setTableOption(bool option,
                   int  valueType);

    void
    setRowIdentifiers();

    void
    contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPointer<CombatTableWidget> m_tableWidget;

    QPointer<QLabel> m_roundCounterLabel;
    QPointer<QLabel> m_currentPlayerLabel;

    QPointer<QUndoStack> m_undoStack;
    QPointer<QAction> m_undoAction;
    QPointer<QAction> m_redoAction;

    std::shared_ptr<CharacterHandler> m_characterHandler;

    const AdditionalSettings& m_additionalSettings;
    const RuleSettings& m_ruleSettings;
    TableSettings m_tableSettings;

    QVector<QVector<QVariant> > m_tableDataOld;

    std::vector<int> m_removedOrAddedRowIndices;

    QByteArray m_headerDataState;

    QJsonObject m_loadedFileData;

    unsigned int m_rowEntered{ 0 };
    unsigned int m_roundCounter{ 1 };

    // Data storing old values before pushing on undo stack
    unsigned int m_rowEnteredOld;
    unsigned int m_roundCounterOld;

    bool m_isDataStored;

    static constexpr int COL_NAME = 0;
    static constexpr int COL_INI = 1;
    static constexpr int COL_MODIFIER = 2;
    static constexpr int COL_HP = 3;
    static constexpr int COL_ENEMY = 4;
    static constexpr int COL_ADDITIONAL = 5;

    static constexpr int SPACING = 30;

    static constexpr int COL_LENGTH_BUFFER_NAME = 20;
    static constexpr int COL_LENGTH_BUFFER_ADDITIONAL = 35;
};
