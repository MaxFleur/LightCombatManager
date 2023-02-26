#pragma once

#include <QPointer>
#include <QWidget>

#include "CharacterHandler.hpp"
#include "DisabledNavigationKeyTable.hpp"

class QAction;
class QLabel;
class QUndoStack;

class RuleSettings;
class TableSettings;

// This class handles the creation of the table widget
class CombatWidget : public QWidget {
    Q_OBJECT

public:
    CombatWidget(bool                          isDataStored,
                 std::shared_ptr<RuleSettings> RuleSettings,
                 int                           mainWidgetWidth,
                 QString                       data = "",
                 QWidget*                      parent = 0);

    void
    generateTable();

    [[nodiscard]] QTableWidget*
    getTableWidget() const
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
    getHeight() const;

    void
    saveOldState();

    void
    pushOnUndoStack(bool resynchronize = false);

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
    addCharacter(QString name,
                 int     ini,
                 int     mod,
                 int     hp,
                 bool    isEnemy,
                 QString addInfo,
                 int     instanceCount);

    void
    rerollIni();

private:
    void
    setTableDataWithFileData();

    void
    sortTable();

    void
    setRowAndPlayer();

    void
    duplicateRow();

    void
    removeRow();

    void
    enteredRowChanged(bool goDown);

    void
    showTablePart(bool show,
                  int  valueType);

    void
    resetNameInfoWidth(const QString& strName,
                       const QString& strAdd);

    void
    setRowIdentifiers();

    void
    contextMenuEvent(QContextMenuEvent *event) override;

private:
    // Main table widget
    QPointer<DisabledNavigationKeyTable> m_tableWidget;

    QPointer<QLabel> m_roundCounterLabel;
    QPointer<QLabel> m_currentPlayerLabel;

    QPointer<QUndoStack> m_undoStack;
    QPointer<QAction> m_undoAction;
    QPointer<QAction> m_redoAction;

    bool m_isDataStored;

    QString m_loadedFileData;

    CharacterHandlerRef m_char;
    std::shared_ptr<RuleSettings> m_ruleSettings;
    std::shared_ptr<TableSettings> m_tableSettings;

    unsigned int m_rowEntered = 0;
    unsigned int m_roundCounter = 1;

    // Data storing old values before pushing on undo stack
    QVector<QVector<QVariant> > m_tableDataOld;
    unsigned int m_rowEnteredOld;
    unsigned int m_roundCounterOld;

    QByteArray m_headerDataState;

    static constexpr int NMBR_COLUMNS = 6;

    static constexpr int COL_NAME = 0;
    static constexpr int COL_INI = 1;
    static constexpr int COL_MODIFIER = 2;
    static constexpr int COL_HP = 3;
    static constexpr int COL_ENEMY = 4;
    static constexpr int COL_ADDITIONAL = 5;

    static constexpr int ROW_ENTERED = 6;
    static constexpr int ROUND_CTR = 7;

    static constexpr int SPACING = 30;

    static constexpr float COL_LENGTH_NAME_BUFFER = 20;
    static constexpr float COL_LENGTH_ADD_BUFFER = 20;

    // Width rations of the first five columns
    static constexpr float WIDTH_NAME = 0.25f;
    static constexpr float WIDTH_INI = 0.05f;
    static constexpr float WIDTH_MODIFIER = 0.05f;
    static constexpr float WIDTH_HP = 0.11f;
    static constexpr float WIDTH_ENEMY = 0.12f;

    static constexpr int HEIGHT_BUFFER = 140;
};
