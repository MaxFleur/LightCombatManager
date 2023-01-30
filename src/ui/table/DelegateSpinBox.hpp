#pragma once

#include <QStyledItemDelegate>

// Small helper class so that the hp column in the table contains spinboxes
class DelegateSpinBox : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit
    DelegateSpinBox(QObject *parent = nullptr);

    [[nodiscard]] QWidget*
    createEditor(QWidget*                    parent,
                 const QStyleOptionViewItem& option,
                 const QModelIndex&          index) const override;
};
