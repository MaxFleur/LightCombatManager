#pragma once

#include <QString>

// Store data used for setting the opening and saving directories
class DirSettings {
public:

	DirSettings();

	void
	write(QString	fileName,
	      bool	setSaveDir);

public:
	QString m_openDir;
	QString m_saveDir;

private:

	void
	read();

	void
	handleSubDir();
};
