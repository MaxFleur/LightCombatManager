#pragma once

#include "BaseSettings.hpp"

// Store additional settings
class AdditionalSettings : public BaseSettings {
public:
    AdditionalSettings();

    void
    write(bool newIndicatorForMultipleChars,
          bool newRollIniForMultipleChars);

public:
    bool indicatorForMultipleChars = true;
    bool rollIniForMultipleChars = false;

private:
    void
    read() override;
};
