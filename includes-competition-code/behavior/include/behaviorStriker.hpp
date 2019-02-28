#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP

#include <basemodule.hpp>
#include "BehaviorBase.hpp"

class behavior : public baseModule
{
public:
    behavior(void*);
    void tick();

    BehaviorBase* current;

    void writeCommand(ActionCommand::All action);

private:
    int a;
    float filtered_fsr_sum;

    void lightRedLed();
    void kick(const float &x = 160.0, const float &y = 45.0, const float &dir = 0.0, const ActionCommand::Body::Foot &foot = ActionCommand::Body::LEFT);
    void stand();
    void walk(const float &x, const float &y, const float &theta);
    bool getUpIfFalling();
    void getUpFront();
    void getUpBack();
    void deadStiff();
    void refPickUp();
    void test();

};

#endif // BEHAVIOR_HPP
