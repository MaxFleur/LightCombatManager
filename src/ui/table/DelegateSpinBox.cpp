#include "DelegateSpinBox.hpp"

#include <QSpinBox>

DelegateSpinBox::DelegateSpinBox(QObject *parent)
    : QStyledItemDelegate(parent)
{
}


QWidget *
DelegateSpinBox::createEditor(QWidget *parent,
                              const QStyleOptionViewItem &,
                              const QModelIndex &) const
{
    auto *const editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(-10000);
    editor->setMaximum(10000);

    return editor;
}
