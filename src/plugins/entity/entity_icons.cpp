#include "entity_icons.h"

namespace Entity
{
namespace Icons
{
const Utils::Icon MODE_ENTITY_CLASSIC(
        QLatin1String(":/entity/images/mode_entity.png"));
const Utils::Icon MODE_ENTITY_FLAT({
        {QLatin1String(":/entity/images/mode_entity_mask.png"), Utils::Theme::IconsBaseColor}});
const Utils::Icon MODE_ENTITY_FLAT_ACTIVE({
        {QLatin1String(":/entity/images/mode_entity_mask.png"), Utils::Theme::IconsModeEntityActiveColor}});
}

}
