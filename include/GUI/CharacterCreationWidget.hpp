#pragma once

#include "../CharacterHandler.hpp"

#include <QLineEdit>
#include <QPointer>
#include <QWidget>

class QCheckBox;
class QPushButton;
class QSpinBox;
class QWidget;

// This class handles the widget used for the creation of characters
class CharacterCreationWidget : public QWidget {
	Q_OBJECT

public:
	CharacterCreationWidget(CharacterHandlerRef	charSort,
				QWidget *		parent = 0);

	// Set the focus to the name label at the start of the character creation
	void
	setNameFocus()
	{
		m_nameEdit->setFocus(Qt::TabFocusReason);
	}
	
    // Returns if a name has been entered
	bool
	isNameEmpty()
	{
		return m_nameEdit->text().isEmpty();
	}
	
	void
    storeLastCharacter();

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

	void
	addStatusEffect();

private:

	void
	storeCharacter();

	void
	keyPressEvent(QKeyEvent *event);

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

	// character sort object to store new created objects
	CharacterHandlerRef m_char;
};
