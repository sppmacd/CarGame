#include <CG/CompoundWidget.h>

namespace cg
{
bool CompoundWidget::isClicked(Vector2f pos)
{
    bool b = false;
    for(Widget* widget: widgets)
        b |= widget->isClicked(pos);
    return b;
}
}
