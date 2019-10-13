#include "Math.hpp"
#include <cmath>

namespace Math
{
    int round(double _1)
    {
        return ceil(_1 > 0 ? _1 - 0.5 : _1 + 0.5);
    }
}
