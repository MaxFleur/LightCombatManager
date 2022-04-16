#pragma once

#include <QDialog>
#include <QPointer>

class QListWidget;

class SettingsData;

// Dialog used to add certain status effects to characters
class StatusEffectDialog : public QDialog {
	Q_OBJECT

public:
	explicit
	StatusEffectDialog(std::shared_ptr<SettingsData>	settingsData,
			   QWidget *				parent = 0);

	[[nodiscard]] QString
	getEffect() const
	{
		return m_effect;
	}

private slots:

	void
	addEffect();

private:

	void
	findEffect(const QString filter);

	QPointer<QListWidget> m_list;

	QString m_effect;

	std::shared_ptr<SettingsData> m_settingsData;
};
