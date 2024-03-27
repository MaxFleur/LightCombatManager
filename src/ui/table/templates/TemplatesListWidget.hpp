#pragma once

#include <QListWidget>
#include "CharacterHandler.hpp"

class QLabel;

// Widget used to display the list of templates
class TemplatesListWidget : public QListWidget {
Q_OBJECT

public:
    explicit TemplatesListWidget(QWidget * parent = 0);

    bool
    addCharacter(const CharacterHandler::Character& character);

    bool
    removeCharacter(const CharacterHandler::Character& character);
};


