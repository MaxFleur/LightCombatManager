#include "DisabledNavigationKeyTable.hpp"

#include <QKeyEvent>

void
DisabledNavigationKeyTable::keyPressEvent(QKeyEvent *event)
{
    // Ignore arrow and tab keys
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
        event->ignore();
        return;
    }
    QTableWidget::keyPressEvent(event);
}
