#pragma once

#include "BaseSettings.hpp"

// Store additional settings
class AdditionalSettings : public BaseSettings {
public:
    AdditionalSettings();

    void
    write(bool newIndicatorMultipleChars,
          bool newRollIniMultipleChars,
          bool newModAddedToIni);

public:
    bool indicatorMultipleChars{ true };
    bool rollIniMultipleChars{ false };
    bool modAddedToIni{ true };

private:
    void
    read() override;
};
