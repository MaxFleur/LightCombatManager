#pragma once

#include <QTableWidget>

class QKeyEvent;

// This is a helping class, disabling the arrow key input
class CustomTable : public QTableWidget {
	Q_OBJECT

public:
	CustomTable(QWidget *parent = nullptr);

protected:
	// Key press event that is reimplemented
	void
	keyPressEvent(QKeyEvent *event) override;
};
