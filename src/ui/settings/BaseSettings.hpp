#pragma once

// Basic settings, from which all other settings derive
class BaseSettings {
private:
	virtual void
	read() = 0;
};
