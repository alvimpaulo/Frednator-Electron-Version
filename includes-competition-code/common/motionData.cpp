#include <motionData.hpp>

//Overload of operator= to the 3 main structs


Command0& Command0::operator=(Command const& copy)
    {
        head    = copy.head;
        body    = copy.body;
        leds    = copy.leds;
        sonar   = copy.sonar;
        return *this;
    }

Command0& Command0::operator=(Command1 const& copy)
{
    head    = copy.head;
    body    = copy.body;
    leds    = copy.leds;
    sonar   = copy.sonar;
    return *this;
}

Command1& Command1::operator=(Command const& copy)
{
    head    = copy.head;
    body    = copy.body;
    leds    = copy.leds;
    sonar   = copy.sonar;
    return *this;
}

Command1& Command1::operator=(Command0 const& copy)
{
    head    = copy.head;
    body    = copy.body;
    leds    = copy.leds;
    sonar   = copy.sonar;
    return *this;
}

Command& Command::operator=(Command0 const& copy)
{
    head    = copy.head;
    body    = copy.body;
    leds    = copy.leds;
    sonar   = copy.sonar;
    return *this;
}

Command& Command::operator=(Command1 const& copy)
{
    head    = copy.head;
    body    = copy.body;
    leds    = copy.leds;
    sonar   = copy.sonar;
    return *this;
}
