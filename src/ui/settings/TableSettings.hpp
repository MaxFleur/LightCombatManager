#pragma once

#include "BaseSettings.hpp"

// Store settings used for customizing the table
class TableSettings : public BaseSettings {
public:
    TableSettings();

    enum class ValueType {
        INI_SHOWN         = 0,
        MOD_SHOWN         = 1,
        COLOR_TABLE       = 2,
        SHOW_INI_TOOLTIPS = 3
    };

    void
    write(bool      valueToWrite,
          ValueType valueType);

public:
    bool iniShown{ true };
    bool modifierShown{ true };
    bool colorTableRows{ false };
    bool showIniToolTips{ false };

private:
    void
    read() override;
};
