    #include <includeMapBehavior.hpp>
#include <Settings.hpp>
#include <qi/log.hpp>

GmCtrlInitial* GmCtrlInitial::instance = NULL;

GmCtrlInitial::GmCtrlInitial()
{
    penalized = 0;
}

BehaviorBase* GmCtrlInitial::Behavior()
{
    if(! instance)
    {
        instance = new GmCtrlInitial();
    }
    return instance;
}

void GmCtrlInitial::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, gameState, (int)GAME_STATE::INITIAL);

    //Do something
    ////qiLogInfo("Behavior") << "Initial!" << std::endl;
    gmCtrlData = readFrom(communication, gmCtrlData);
    int gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;

    if ((gameControlPenalized> 0)&&(gameControlPenalized <10))
    {
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
    //Stand
    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::STAND;
    action.leds.chestButton = getChestColor(unBoard);
    writeCommand(action, unBoard);
}

BehaviorBase* GmCtrlInitial::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    if (penalized==1)
    {
        return GmCtrlPenalized::Behavior();
    }
    else {
        switch(gmCtrlData.state) {
            case STATE_INITIAL:
                return this;
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
                return GmCtrlFinished::Behavior();
                break;
        }
    }

    return this;
}
