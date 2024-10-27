#pragma once

#include <QSettings>

// Basic settings, from which all other settings derive
class BaseSettings {
protected:
    template<typename T>
    void
    writeParameter(QSettings&     settings,
                   const T        fromValue,
                   T&             toValue,
                   const QString& parameterName)
    {
        if (toValue != fromValue) {
            toValue = fromValue;
            settings.setValue(parameterName, toValue);
        }
    }

private:
    virtual void
    read() = 0;
};
