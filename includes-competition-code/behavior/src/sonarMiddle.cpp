#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

sonarMiddle* sonarMiddle::instance = NULL;

sonarMiddle::sonarMiddle()
{
    sonarOutput = SonarData::NONE;
}

BehaviorBase* sonarMiddle::Behavior()
{
    if(! instance)
    {
        instance = new sonarMiddle();
    }
    return instance;
}

void sonarMiddle::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, walkTransition, true);
    writeTo(behavior, turnFinished, false); //setting this for lookAround

    //Do something
    qiLogInfo("Behavior") << "Sonar Middle!" << std::endl;

    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::WALK;
    //sonarOutput = sonarStatic->getSonar();
    action.body.left = 0.0;
    action.body.forward = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    writeCommand(action, unBoard);
}

BehaviorBase* sonarMiddle::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    if(sonarOutput == SonarData::LEFT)
    {
        return sonarLeft::Behavior();
    }
    else if(sonarOutput == SonarData::RIGHT)
    {
       return sonarRight::Behavior();

    }
    else if(sonarOutput == SonarData::MIDDLE)
    {
        return this;
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
    
    return this;
}
