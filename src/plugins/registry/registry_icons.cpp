#include "registry_icons.h"

namespace Registry
{
namespace Icons
{
const Utils::Icon MODE_REGISTRY_CLASSIC(
        QLatin1String(":/registry/images/mode_registry.png"));
const Utils::Icon MODE_REGISTRY_FLAT({
        {QLatin1String(":/registry/images/mode_registry_mask.png"), Utils::Theme::IconsBaseColor}});
const Utils::Icon MODE_REGISTRY_FLAT_ACTIVE({
        {QLatin1String(":/registry/images/mode_registry_mask.png"), Utils::Theme::IconsModeRegistryActiveColor}});
}

}
