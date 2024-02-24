#pragma once

#include <QMetaType>
#include <QVariant>

struct AdditionalInfoData {
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

    QVector<StatusEffect> statusEffects;
    QString               mainInfoText;
};

Q_DECLARE_METATYPE(AdditionalInfoData);
