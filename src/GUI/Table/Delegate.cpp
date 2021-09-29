#include "../../../include/GUI/Table/Delegate.hpp"

#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}


QWidget *
SpinBoxDelegate::createEditor(QWidget *parent,
			      const QStyleOptionViewItem &,
			      const QModelIndex &) const
{
	auto* const editor = new QSpinBox(parent);
	editor->setFrame(false);
	editor->setMinimum(-100);
	editor->setMaximum(10000);

	return editor;
}
