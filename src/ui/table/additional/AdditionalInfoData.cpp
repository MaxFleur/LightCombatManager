#include "AdditionalInfoData.hpp"

namespace AdditionalInfoData
{
QVariant
getAdditionalInformationVariant(AdditionalInformation additionalInformation)
{
    QVariant variant;
    variant.setValue(additionalInformation);
    return variant;
}
}
