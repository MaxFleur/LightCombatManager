#pragma once

#include "AdditionalSettings.hpp"
#include "DirSettings.hpp"
#include "FileHandler.hpp"
#include "RuleSettings.hpp"

#include <QMainWindow>
#include <QPointer>

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
    setTableWidget(bool               isDataStored,
                   bool               newCombatStarted,
                   const QJsonObject& jsonObjectData = {});

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
    checkStoredTableRules(const QJsonObject& jsonObjectData);

    void
    setMainWindowIcons();

    bool
    event(QEvent *event) override;

private:
    QPointer<WelcomeWidget> m_welcomeWidget;
    QPointer<CombatWidget> m_combatWidget;

    QPointer<QAction> m_newCombatAction;
    QPointer<QAction> m_openCombatAction;
    QPointer<QAction> m_saveAction;
    QPointer<QAction> m_saveAsAction;
    QPointer<QAction> m_closeAction;
    QPointer<QAction> m_openSettingsAction;
    QPointer<QAction> m_aboutLCMAction;

    std::unique_ptr<FileHandler> m_fileHandler;

    AdditionalSettings m_additionalSettings;
    RuleSettings m_ruleSettings;
    DirSettings m_dirSettings;

    RuleSettings::Ruleset m_loadedTableRule;

    QString m_fileName{ "" };
    QString m_fileDir{ "" };

    bool m_isTableActive{ false };
    bool m_isTableSavedInFile{ false };

    bool m_loadedTableRollAutomatically;

    static constexpr unsigned int START_WIDTH = 860;
    static constexpr unsigned int START_HEIGHT = 240;
};
