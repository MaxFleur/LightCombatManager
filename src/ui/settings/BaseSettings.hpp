#pragma once

#include <QSettings>

template<typename T>
concept SettingsValue = std::same_as<T, int> || std::same_as<T, bool> ||
                        std::same_as<T, QString> || std::is_enum_v<T>;

// Basic settings, from which all other settings derive
class BaseSettings {
protected:
    template<typename T>
    requires SettingsValue<T>
    void
    writeParameter(QSettings&     settings,
                   const T        fromValue,
                   T&             toValue,
                   const QString& identifier)
    {
        if (toValue == fromValue) {
            return;
        }

        toValue = fromValue;
        settings.setValue(identifier, toValue);
    }

private:
    virtual void
    read() = 0;
};
