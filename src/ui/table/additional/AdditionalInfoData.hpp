#pragma once

#include <QMetaType>
#include <QVariant>

namespace AdditionalInfoData
{
// Utility functions for the Combat Table
struct StatusEffect {
    QString      name;
    bool         isPermanent;
    unsigned int duration;

    StatusEffect(const QString& name, bool isPermanent,
                 unsigned int duration) :
        name(std::move(name)), isPermanent(std::move(isPermanent)),
        duration(std::move(duration))
    {
    }
};

struct AdditionalInformation {
    QString               mainInfo{ "" };
    QVector<StatusEffect> statusEffects{};
};

[[nodiscard]] const QVariant
getAdditionalInformationVariant(const AdditionalInformation& additionalInformation);
}

Q_DECLARE_METATYPE(AdditionalInfoData::AdditionalInformation);
