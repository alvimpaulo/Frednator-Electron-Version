#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

staticTest* staticTest::instance = NULL;

staticTest::staticTest()
{

}

BehaviorBase* staticTest::Behavior()
{
    if(! instance)
    {
        instance = new staticTest();
    }
    return instance;
}

void staticTest::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    ////qiLogInfo("Behavior") << "Fuciona!" << std::endl;

    ActionCommand::All action;

        action.leds.rightEye.red = 1;
        action.leds.rightEye.blue = 1;
        action.leds.rightEye.green = 0;

        action.leds.leftEye.red = 0;
        action.leds.leftEye.blue = 1;
        action.leds.leftEye.green = 0;

        action.body.actionType = ActionCommand::Body::WALK;

        action.body.forward = 0.0;
        action.body.left = 0.0;
        action.body.turn = 0.0;
        action.body.power = 1.0;
        action.body.bend = 15.0;

        writeCommand(action, unBoard);
}

BehaviorBase* staticTest::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    return this;
}
