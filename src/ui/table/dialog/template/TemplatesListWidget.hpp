#pragma once

#include <QListWidget>

#include "CharacterHandler.hpp"

// Widget used to display the list of templates
class TemplatesListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit
    TemplatesListWidget(QWidget * parent = 0);

    bool
    addCharacter(const CharacterHandler::Character& character);

    void
    removeCharacter(const QString& characterName);
};
