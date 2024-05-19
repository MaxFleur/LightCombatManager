#pragma once

#include <QPointer>
#include <QWidget>

#include "CharacterHandler.hpp"
#include "CharFileHandler.hpp"
#include "TemplatesListWidget.hpp"

// Widget used to handle the templates
class TemplatesWidget : public QWidget {
    Q_OBJECT

public:
    explicit
    TemplatesWidget(QWidget * parent = 0);

    void
    loadTemplates();

    void
    addCharacter(const CharacterHandler::Character& character);

    [[nodiscard]] int
    getTemplatesCount()
    {
        return m_templatesListWidget->count();
    }

signals:
    void
    characterLoaded(const CharacterHandler::Character& character);

private slots:
    void
    applyButtonClicked();

    void
    removeButtonClicked();

private:
    QPointer<TemplatesListWidget> m_templatesListWidget;

    std::unique_ptr<CharFileHandler> m_charFileHandler;
};
