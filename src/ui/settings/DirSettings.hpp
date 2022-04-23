#pragma once

#include <QString>

#include "BaseSettings.hpp"

// Store data used for the opening and saving directories
class DirSettings : public BaseSettings {
public:

	DirSettings();

	void
	write(QString	fileName,
	      bool	setSaveDir);

public:
	QString openDir;
	QString saveDir;

private:

	void
	read() override;

	void
	handleSubDir();
};
