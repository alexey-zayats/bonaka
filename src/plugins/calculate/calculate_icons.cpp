#include "calculate_icons.h"

namespace Calculate
{
namespace Icons
{
const Utils::Icon MODE_CALCULATE_CLASSIC(
        QLatin1String(":/calculate/images/mode_calculate.png"));
const Utils::Icon MODE_CALCULATE_FLAT({
        {QLatin1String(":/calculate/images/mode_calculate_mask.png"), Utils::Theme::IconsBaseColor}});
const Utils::Icon MODE_CALCULATE_FLAT_ACTIVE({
        {QLatin1String(":/calculate/images/mode_calculate_mask.png"), Utils::Theme::IconsModeCalculateActiveColor}});
}

}
