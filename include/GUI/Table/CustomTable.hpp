#pragma once

#include <QTableWidget>
#include <QtWidgets>

// This is just a short helping class, disabling the arrow key input
class CustomTable : public QTableWidget {
	Q_OBJECT
public:
	CustomTable(QWidget *parent = nullptr);

protected:
	// Key press event that is reimplemented
	void
	keyPressEvent(QKeyEvent *event) override;
};
