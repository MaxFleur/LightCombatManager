#include "../../../include/GUI/Table/Delegate.h"

#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &,
                                       const QModelIndex &) const {
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    // Set the boundaries of the spinbox
    editor->setMinimum(-100);
    editor->setMaximum(10000);

    return editor;
}
