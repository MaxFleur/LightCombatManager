#pragma once

#include <QMainWindow>
#include <QPointer>

#include "AdditionalSettings.hpp"
#include "DirSettings.hpp"
#include "FileHandler.hpp"
#include "RuleSettings.hpp"

class QAction;

class CombatWidget;
class WelcomeWidget;

// This class handles the creation and managing of the main GUI window. It also serves
// as "main anchor point", switching between LCM's widgets, which are created in other classes.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

signals:
    void
    setSaveAction(bool enable);

private slots:
    void
    newCombat();

    bool
    saveTable();

    void
    saveAs();

    void
    openTable();

    void
    openSettings();

    void
    about();

    void
    exitCombat();

private:
    void
    setWelcomingWidget();

    void
    setTableWidget(bool           isDataStored,
                   bool           newCombatStarted,
                   const QString& data = "");

    void
    setCombatTitle(bool isCombatActive);

    [[nodiscard]] int
    createSaveMessageBox(const QString& tableMessage,
                         bool           isClosing);

    [[nodiscard]] QString
    createRuleChangeMessageBoxText() const;

    void
    closeEvent(QCloseEvent *event) override;

    [[nodiscard]] bool
    checkStoredTableRules(QString data);

    bool
    event(QEvent *event) override;

private:
    QPointer<WelcomeWidget> m_welcomeWidget;
    QPointer<CombatWidget> m_combatWidget;
    QPointer<QAction> m_openSettingsAction;

    bool m_isTableActive{ false };
    bool m_tableInFile{ false };

    QString m_fileName{ "" };

    FileHandlerRef m_file;

    AdditionalSettings m_additionalSettings;
    RuleSettings m_ruleSettings;
    DirSettings m_dirSettings;

    RuleSettings::Ruleset m_loadedTableRule;
    bool m_loadedTableRollAutomatically;

    static constexpr int START_WIDTH = 860;
    static constexpr int START_HEIGHT = 240;

    static constexpr int COL_RULESET = 8;
    static constexpr int COL_ROLL_AUTOMATICALLY = 9;
};
