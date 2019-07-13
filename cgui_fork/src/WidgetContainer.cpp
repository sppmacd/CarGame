#include <CG/WidgetContainer.h>

namespace cg
{
void WidgetContainer::onDraw(RenderWindow&)
{
    /* * * * * * * * * * * * * * * *
    for(Widget* widget: widgets)
        widget->draw(wnd);
     * * * * * * * * * * * * * * * */
}

Widget* WidgetContainer::onMouseClick(Vector2f pos)
{
    return NULL;
}
Widget* WidgetContainer::onMouseClick(Vector2f pos, bool release, Mouse::Button button)
{
    if(release)
    {
        for(Widget* widget: widgets)
        {
            widget->onMouseClick(pos);
            if(widget->isClicked(pos))
            {
                bool b = widget->onClick();

                if(b)
                    return widget;
            }
        }
    }
    return NULL;
}
void WidgetContainer::onMouseMove(Vector2f pos)
{
    for(Widget* widget: widgets)
    {
        if(widget->isClicked(pos))
        {
            widget->setMouseOver();
        }
        else
        {
            widget->setMouseOver(false);
        }
    }
}
Widget* WidgetContainer::getFocusedWidget()
{
    for(unsigned i = 0; i < widgets.size(); i++)
    {
        if(widgets[i]->hasFocus())
            return widgets[i];
    }
    return nullptr;
}
void WidgetContainer::addWidget(Widget* widget)
{
    widgets.push_back(widget);
}

void WidgetContainer::setFocus(Widget* widget)
{
    for(unsigned i = 0; i < widgets.size(); i++)
    {
        if(widgets[i] == widget)
            widgets[i]->setFocus();
        else
            widgets[i]->setFocus(false);
    }
}

void WidgetContainer::setFocus(int wId)
{
    for(unsigned i = 0; i < widgets.size(); i++)
    {
        if(widgets[i]->getID() == wId)
            widgets[i]->setFocus();
        else
            widgets[i]->setFocus(false);
    }
}

void WidgetContainer::removeWidget(Widget& widget)
{
    for(unsigned int i = 0; i < widgets.size(); i++)
    {
        if(*widgets[i] == widget)
        {
            widgets.erase(widgets.begin() + i);
            return;
        }
    }
}

void WidgetContainer::onClick(int)
{
    //cout << "You'll never see this message! \n\tButton (ID " << buttonId << ")" << endl;
}
}
