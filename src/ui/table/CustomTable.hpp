#pragma once

#include <QTableWidget>

// Helper class, disabling arrow key input
class CustomTable : public QTableWidget {
	Q_OBJECT

protected:

	void
	keyPressEvent(QKeyEvent *event) override;
};
