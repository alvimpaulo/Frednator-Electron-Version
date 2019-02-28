#ifndef BASEMODULE_HPP
#define BASEMODULE_HPP

#include <unBoard.hpp>

class baseModule
{
public:
    baseModule();
    virtual void tick() = 0;
//protected:
    UnBoard* unBoard;
};

#endif // BASEMODULE_HPP
