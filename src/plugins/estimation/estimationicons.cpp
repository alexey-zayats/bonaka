#include "estimationicons.h"

namespace Estimation
{
namespace Icons
{
const Utils::Icon MODE_ESTIMATION_CLASSIC(
        QLatin1String(":/estimation/images/mode_estimation.png"));
const Utils::Icon MODE_ESTIMATION_FLAT({
        {QLatin1String(":/estimation/images/mode_estimation_mask.png"), Utils::Theme::IconsBaseColor}});
const Utils::Icon MODE_ESTIMATION_FLAT_ACTIVE({
        {QLatin1String(":/estimation/images/mode_estimation_mask.png"), Utils::Theme::IconsModeEstimationActiveColor}});
}

}
