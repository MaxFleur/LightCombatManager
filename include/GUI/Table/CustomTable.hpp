#pragma once

#include <QTableWidget>

class QKeyEvent;

// Helper class, disabling arrow key input
class CustomTable : public QTableWidget {
	Q_OBJECT

public:
	CustomTable(QWidget *parent = nullptr);

protected:

	void
	keyPressEvent(QKeyEvent *event) override;
};
