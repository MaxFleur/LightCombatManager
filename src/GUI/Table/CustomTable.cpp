#include "../../../include/GUI/Table/CustomTable.hpp"

CustomTable::CustomTable(QWidget *parent)
{
}


// Ignore event if the arrow keys are pressed. Otherwise just do a normal key press event.
void
CustomTable::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
		event->ignore();
		return;
	} else {
		QTableWidget::keyPressEvent(event);
	}
}
