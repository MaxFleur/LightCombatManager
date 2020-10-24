#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
// Small helper class so that the hp column in the table contains spinboxes
class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

};
#endif 
