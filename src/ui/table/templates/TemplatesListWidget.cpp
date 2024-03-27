//
// Created by brown on 18.03.2024.
//

#include "TemplatesListWidget.hpp"
#include <QHeaderView>

TemplatesListWidget::TemplatesListWidget(QWidget *   parent) :
        QListWidget(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setFocusPolicy(Qt::ClickFocus);
}
bool
TemplatesListWidget::addCharacter(const CharacterHandler::Character& character)
{
    for (int i = 0; i < count(); i++) {
        auto const storedCharacter = item(i)->data(Qt::UserRole).value<CharacterHandler::Character>();
        if (storedCharacter.name == character.name) {
            return false;
        }
    }
    auto *const item = new QListWidgetItem(this);
    item->setText(character.name);
    item->setData(Qt::UserRole, QVariant::fromValue(character));
    addItem(item);
    return true;
}

bool
TemplatesListWidget::removeCharacter(const CharacterHandler::Character &character)
{
    for (int i = 0; i < count(); i++) {
        auto const storedCharacter = item(i)->data(Qt::UserRole).value<CharacterHandler::Character>();
        if (storedCharacter.name == character.name) {
            QListWidgetItem *it = this->takeItem(i);
            delete it;
            return true;
        }
    }
    return false;
}