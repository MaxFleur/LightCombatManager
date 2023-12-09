#include "AdditionalInfoData.hpp"

namespace AdditionalInfoData
{
const QVariant
getAdditionalInformationVariant(const AdditionalInformation& additionalInformation)
{
    QVariant variant;
    variant.setValue(additionalInformation);
    return variant;
}
}
