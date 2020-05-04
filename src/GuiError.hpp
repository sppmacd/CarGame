#ifndef GUIERROR_HPP
#define GUIERROR_HPP

#include <CG/CG.h>

class GuiError : public cg::GuiOk
{
public:
    GuiError(std::string errorStr);
};

#endif // GUIERROR_HPP

