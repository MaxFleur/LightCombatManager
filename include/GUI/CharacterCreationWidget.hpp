#pragma once

#include "../CharacterHandler.hpp"

#include <QLineEdit>
#include <QPointer>
#include <QWidget>

class QCheckBox;
class QLineEdit;
class QPushButton;
class QSpinBox;

// This class handles the widget used for the creation of characters
class CharacterCreationWidget : public QWidget {
	Q_OBJECT

public:
	CharacterCreationWidget(CharacterHandlerRef	charSort,
				QWidget *		parent = 0);

	// Store the last created character before creating the table widget
	void
	storeLastCharacter();

	// Returns if a name has been entered
	bool
	isNameEmpty()
	{
		return m_nameEdit->text().isEmpty();
	}

signals:

	void
	cancel();

	void
	finish();

private slots:

	void
	saveAndCreateNewCharacter();

	void
	resetCharacter();

private:

	void
	storeCharacter();

	// Widgets
	QPointer<QLineEdit> m_nameEdit;
	QPointer<QSpinBox> m_initBox;
	QPointer<QSpinBox> m_initModifierBox;
	QPointer<QSpinBox> m_hpBox;
	QPointer<QCheckBox> m_isNPCBox;
	QPointer<QLineEdit> m_addInfoEdit;

	QPointer<QPushButton> m_saveButton;
	QPointer<QPushButton> m_finishButton;
	QPointer<QPushButton> m_resetButton;
	QPointer<QPushButton> m_cancelButton;

	// character sort object to store newly created objects
	CharacterHandlerRef m_char;
};
