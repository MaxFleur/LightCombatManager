#pragma once

#include "CharacterHandler.hpp"
#include "CombatTableWidget.hpp"
#include "TableFileHandler.hpp"
#include "TableSettings.hpp"

#include <QJsonObject>
#include <QPointer>
#include <QWidget>

class QAction;
class QLabel;
class QUndoStack;
class QTimer;

class AdditionalSettings;
class RuleSettings;

// This class handles the main combat widget
class CombatWidget : public QWidget {
    Q_OBJECT

public:
    CombatWidget(std::shared_ptr<TableFileHandler> tableFilerHandler,
                 const AdditionalSettings&         AdditionalSettings,
                 const RuleSettings&               RuleSettings,
                 int                               mainWidgetWidth,
                 bool                              isDataStored,
                 QWidget *                         parent = 0);

    void
    generateTableFromTableData();

    [[nodiscard]] CombatTableWidget*
    getCombatTableWidget() const
    {
        return m_tableWidget;
    }

    [[nodiscard]] bool
    isEmpty() const
    {
        return m_tableWidget->rowCount() == 0;
    }

    [[nodiscard]] unsigned int
    getHeight() const
    {
        return m_tableWidget->getHeight() + 40;
    }

    [[nodiscard]] bool
    saveTableData(const QString& fileName);

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
    insertTable();

    void
    openStatusEffectDialog();

    void
    addCharacter(CharacterHandler::Character character,
                 int                         instanceCount);

    void
    rerollIni();

    void
    changeHPForMultipleChars();

    void
    removeRow();

    void
    duplicateRow();

    void
    handleTableWidgetItemPressed(QTableWidgetItem *item);

private:
    void
    sortTable();

    void
    setRowAndPlayer() const;

    void
    switchCharacterPosition(bool goDown);

    void
    enteredRowChanged(bool goDown);

    void
    setTableOption(bool option,
                   int  valueType);

    void
    writeStoredCharacters(const QJsonObject& jsonObject);

    [[nodiscard]] QAction*
    createAction(const QString&      text,
                 const QString&      toolTip,
                 const QKeySequence& keySequence,
                 bool                enabled);

    void
    contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPointer<CombatTableWidget> m_tableWidget;

    QPointer<QLabel> m_roundCounterLabel;
    QPointer<QLabel> m_currentPlayerLabel;
    QPointer<QLabel> m_iniRerolledLabel;

    QPointer<QUndoStack> m_undoStack;

    QPointer<QTimer> m_timer;

    QPointer<QAction> m_addCharacterAction;
    QPointer<QAction> m_insertTableAction;
    QPointer<QAction> m_removeAction;
    QPointer<QAction> m_addEffectAction;
    QPointer<QAction> m_duplicateAction;
    QPointer<QAction> m_rerollAction;
    QPointer<QAction> m_changeHPAction;
    QPointer<QAction> m_undoAction;
    QPointer<QAction> m_redoAction;
    QPointer<QAction> m_resortAction;
    QPointer<QAction> m_moveUpwardAction;
    QPointer<QAction> m_moveDownwardAction;

    std::shared_ptr<CharacterHandler> m_characterHandler;
    std::shared_ptr<TableFileHandler> m_tableFileHandler;

    const AdditionalSettings& m_additionalSettings;
    const RuleSettings& m_ruleSettings;
    TableSettings m_tableSettings;

    QVector<QVector<QVariant> > m_tableDataOld;

    std::vector<int> m_removedOrAddedRowIndices;

    QByteArray m_headerDataState;

    unsigned int m_rowEntered{ 0 };
    unsigned int m_roundCounter{ 1 };

    // Data storing old values before pushing on undo stack
    unsigned int m_rowEnteredOld;
    unsigned int m_roundCounterOld;

    bool m_isDataStored;

    static constexpr int SPACING = 30;

    static constexpr int COL_LENGTH_BUFFER_NAME = 20;
    static constexpr int COL_LENGTH_BUFFER_ADDITIONAL = 35;

    static constexpr int LABEL_SHOWN_DURATION = 3000;
};
