#include <includeMapBehavior.hpp>
#include <Settings.hpp>
#include <qi/log.hpp>

GmCtrlFinished* GmCtrlFinished::instance = NULL;

GmCtrlFinished::GmCtrlFinished()
{
    penalized = 0;
}

BehaviorBase* GmCtrlFinished::Behavior()
{
    if(! instance)
    {
        instance = new GmCtrlFinished();
    }
    return instance;
}

void GmCtrlFinished::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, gameState, (int)GAME_STATE::FINISHED);

    //Do something
    ////qiLogInfo("Behavior") << "Finished!" << std::endl;

    gmCtrlData = readFrom(communication, gmCtrlData);
    int gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;

    if ((gameControlPenalized> 0)&&(gameControlPenalized <10)){
        penalized=1;
    }
    else if(ButtonPressed(unBoard))
    {
        penalized=1;
        writeTo(behavior, play_chest, true);
    }
    else
    {
        penalized=0;
    }
    //Penalizar robo aqui
    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::REF_PICKUP;
    action.leds.chestButton = getChestColor(unBoard);
    writeCommand(action, unBoard);
}

BehaviorBase* GmCtrlFinished::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    if (penalized==1)
        return GmCtrlPenalized::Behavior();
    else {
        switch(gmCtrlData.state) {
            case STATE_INITIAL:
                return GmCtrlInitial::Behavior();
                break;
            case STATE_READY:
                return GmCtrlReady::Behavior();
                break;
            case STATE_SET:
                return GmCtrlSet::Behavior();
                break;
            case STATE_PLAYING:
                return GmCtrlPlaying::Behavior();
                break;
            case STATE_FINISHED:
                return this;
                break;
        }
    }
    
    return this;
}
