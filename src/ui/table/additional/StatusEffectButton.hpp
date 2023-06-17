#pragma once

#include <QPointer>
#include <QPushButton>

#include "AdditionalInfoData.hpp"

// This class displays additional information and status effects
class StatusEffectButton : public QPushButton {
    Q_OBJECT

public:
    StatusEffectButton(AdditionalInfoData::StatusEffect& statusEffect);

signals:
    void
    menuCalled();

    void
    effectChanged(AdditionalInfoData::StatusEffect statusEffect);

    void
    removeCalled();

private:
    void
    setButtonText();

    void
    changeDuration(int value);

    void
    createMenu();

private:
    AdditionalInfoData::StatusEffect m_statusEffect;
};
