#pragma once

#include "BaseSettings.hpp"

// Store additional settings
class AdditionalSettings : public BaseSettings {
public:
    AdditionalSettings();

    void
    write(bool newIndicatorMultipleChars,
          bool newRollIniMultipleChars);

public:
    bool indicatorMultipleChars{ true };
    bool rollIniMultipleChars{ false };

private:
    void
    read() override;
};
