#include "GuiError.hpp"

GuiError::GuiError(std::string errStr) : cg::GuiOk(errStr)
{
    cg::colors::textSize = 15;
}
