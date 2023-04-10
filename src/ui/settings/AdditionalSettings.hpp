#pragma once

#include "BaseSettings.hpp"

// Store additional settings
class AdditionalSettings : public BaseSettings {
public:
    AdditionalSettings();

    void
    write(bool newindicatorForMultipleChars);

public:
    bool indicatorForMultipleChars = true;

private:
    void
    read() override;
};
