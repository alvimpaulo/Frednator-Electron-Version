#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

Avoid* Avoid::instance = NULL;

Avoid::Avoid()
{

}

BehaviorBase* Avoid::Behavior()
{
    if(! instance)
    {
        instance = new Avoid();
    }
    return instance;
}

void Avoid::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    ////qiLogInfo("Behaviour") << "Avoid State!" << std::endl;

    writeTo(behavior, walkTransition, true);
    writeTo(behavior, turnFinished, false); //setting this for lookAround

    sonarStatic->getSonar(unBoard);

    int sign = 0;
    int sonarTmp;
    sonarTmp = readFrom(behavior, sonarPos);



    if(sonarTmp == SonarData::LEFT)
        sign = -1;
    else if(sonarTmp == SonarData::MIDDLE)
        sign = 0;
    else if(sonarTmp == SonarData::RIGHT)
        sign = 1;

    ActionCommand::All action;

    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 1;
    action.leds.rightEye.green = 1;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.left = sign*300.0;
    action.body.forward = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    writeCommand(action, unBoard);

}

BehaviorBase* Avoid::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    int sonarTmp;
    sonarTmp = readFrom(behavior, sonarPos);

    ////std::cout << "sonar" << sonarTmp << std::endl;

   if(sonarTmp == 0)
        return lastBehavior;
    return this;
}
