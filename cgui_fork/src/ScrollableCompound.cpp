#include <CG/ScrollableCompound.h>
#include <CG/GuiHandler.h>

namespace cg
{
ScrollableCompound::ScrollableCompound(Gui* gui, Vector2f initialPos, Vector2f initialSize, int id) : CompoundWidget(gui, initialPos, id), currentSize(initialSize)
{
    scrollX = ScrollBar(gui, Vector2f(initialPos.x, initialPos.y + initialSize.y - colors::sliderHeight), initialSize.x - colors::sliderHeight, 1.f, false, -1);
    scrollY = ScrollBar(gui, Vector2f(initialPos.x + initialSize.x - colors::sliderHeight, initialPos.y), initialSize.y - colors::sliderHeight, 1.f, true, -2);
    WidgetContainer::addWidget(&scrollX);
    WidgetContainer::addWidget(&scrollY);
    size = initialSize;
}
ScrollableCompound::ScrollableCompound() : ScrollableCompound(NULL, Vector2f(), Vector2f(), 0)
{

}
void ScrollableCompound::draw(RenderWindow& wnd)
{
    //draw border
    RectangleShape rs(size);
    rs.setPosition(getPosition());
    rs.setOutlineColor(colors::sliderOutline);
    rs.setOutlineThickness(2.f);
    rs.setFillColor(Color(128, 128, 128, 50));
    wnd.draw(rs);

    float scrollXOffset = currentSize.x * scrollX.getScrollPosition() - size.x;
    float scrollYOffset = currentSize.y * scrollY.getScrollPosition() - size.y;

    //update scrolls
    if(scrollXOffset < 0)
    {
        scrollX.setEnabled(false);
        scrollXOffset = 0.f;
    }
    else
        scrollX.setEnabled(true);

    if(scrollYOffset < 0)
    {
        scrollY.setEnabled(false);
        scrollYOffset = 0.f;
    }
    else
        scrollY.setEnabled(true);

    scrollX.draw(wnd);
    scrollY.draw(wnd);

    //calculate viewport
    float posX = getPosition().x / parent->guiHandler->getSize().x;
    float posY = getPosition().y / parent->guiHandler->getSize().y;
    float sizeX = size.x / parent->guiHandler->getSize().x;
    float sizeY = size.y / parent->guiHandler->getSize().y;

    //View view = parent->guiHandler->getWindow()->getView();
    //view.setViewport(FloatRect(posX, posY, sizeX, sizeY)); //set viewport to widget area
    //wnd.setView(view);
    for(Widget* widget: widgets) //draw all widgets
    {
        widget->setPosition(widget->getPosition() - Vector2f(scrollXOffset, scrollYOffset)); //set scrolled position
        widget->draw(wnd);
        widget->setPosition(widget->getPosition() + Vector2f(scrollXOffset, scrollYOffset)); //restore position
    }
    //view.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f)); //restore viewport
    //wnd.setView(view);
}
void ScrollableCompound::addWidget(Widget* widget)
{
    FloatRect rect(widget->getBoundingBox());
    Vector2f maxWidgetPos = Vector2f(rect.left + rect.width, rect.top + rect.height);
    if(maxWidgetPos.x > currentSize.x)
        currentSize.x = maxWidgetPos.x;
    if(maxWidgetPos.y > currentSize.y)
        currentSize.y = maxWidgetPos.y;
    parent->addWidget(widget);
    WidgetContainer::addWidget(widget);
}
void ScrollableCompound::setSize(Vector2f s)
{
    size = s;
}
}
