#pragma once

#include "BaseSettings.hpp"

// Store the main settings affecting the entire application
class RuleSettings : public BaseSettings {
public:
    RuleSettings();

    enum Ruleset {
        PATHFINDER_1E_DND_35E = 0,
        PATHFINDER_2E         = 1,
        DND_5E                = 2,
        DND_30E               = 3,
        STARFINDER            = 4
    };

    void
    write(unsigned int newRuleset,
          bool         newRollAutomatical);

public:
    Ruleset ruleset{ PATHFINDER_1E_DND_35E };
    bool rollAutomatical{ false };

private:
    void
    read() override;
};
