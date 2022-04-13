#include "DisabledArrowKeyTable.hpp"

#include <QKeyEvent>

void
DisabledArrowKeyTable::keyPressEvent(QKeyEvent *event)
{
	// Ignore event if the arrow keys are pressed
	if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
		event->ignore();
		return;
	}
	QTableWidget::keyPressEvent(event);
}