#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

sonarLeft* sonarLeft::instance = NULL;

sonarLeft::sonarLeft()
{
    sonarOutput = SonarData::NONE;
}

BehaviorBase* sonarLeft::Behavior()
{
    if(! instance)
    {
        instance = new sonarLeft();
    }
    return instance;
}

void sonarLeft::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, walkTransition, true);
    writeTo(behavior, turnFinished, false); //setting this for lookAround

    //Do something
    qiLogInfo("Behavior") << "Sonar Left!" << std::endl;

    //sonarOutput = sonarStatic->getSonar();

    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::WALK;
    action.body.left = -300.0;
    action.body.forward = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    writeCommand(action, unBoard);

}

BehaviorBase* sonarLeft::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    if(sonarOutput == SonarData::LEFT)
    {
        return this;
    }
    else if(sonarOutput == SonarData::RIGHT)
    {
       return sonarRight::Behavior();

    }
    else if(sonarOutput == SonarData::MIDDLE)
    {
        return sonarMiddle::Behavior();
    }
    else if(sonarOutput == SonarData::NONE)
    {
        if(lastBehavior == NULL)
        {
            std::cout << "lastBehaviour is NULL" << std::endl;
            return Start::Behavior();
        }
        else
        {
            //return Start::Behavior();
            return lastBehavior;
        }
    }

}
