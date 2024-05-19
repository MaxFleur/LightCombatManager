#include "TemplatesListWidget.hpp"

TemplatesListWidget::TemplatesListWidget(QWidget* parent) :
    QListWidget(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setFocusPolicy(Qt::ClickFocus);
}


bool
TemplatesListWidget::addCharacter(const CharacterHandler::Character& character)
{
    for (auto i = 0; i < count(); i++) {
        const auto storedCharacter = item(i)->data(Qt::UserRole).value<CharacterHandler::Character>();
        if (storedCharacter.name == character.name) {
            return false;
        }
    }

    auto *const item = new QListWidgetItem;
    item->setText(character.name);
    item->setData(Qt::UserRole, QVariant::fromValue(character));
    addItem(item);
    return true;
}


bool
TemplatesListWidget::removeCharacter(const CharacterHandler::Character &character)
{
    for (auto i = 0; i < count(); i++) {
        auto const storedCharacter = item(i)->data(Qt::UserRole).value<CharacterHandler::Character>();
        if (storedCharacter.name == character.name) {
            QListWidgetItem *it = this->takeItem(i);
            delete it;
            return true;
        }
    }
    return false;
}
