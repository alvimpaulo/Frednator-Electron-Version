#include <includeMapBehavior.hpp>
#include <Settings.hpp>
#include <qi/log.hpp>

GmCtrlSet* GmCtrlSet::instance = NULL;

GmCtrlSet::GmCtrlSet()
{
    penalized = 0;
}

BehaviorBase* GmCtrlSet::Behavior()
{
    if(! instance)
    {
        instance = new GmCtrlSet();
    }
    return instance;
}

void GmCtrlSet::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, gameState, (int)GAME_STATE::SET);
    writeTo(behavior, wasSet, true);

    //Do something
    ////qiLogInfo("Behavior") << "Set!" << std::endl;

    gmCtrlData = readFrom(communication, gmCtrlData);
    int gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;

    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::STAND;
    action.leds.chestButton = getChestColor(unBoard);
    writeCommand(action, unBoard);

    if ((gameControlPenalized> 0)&&(gameControlPenalized <10)) {
       penalized=1;
    }
    else {
       penalized=0;
    }
}

BehaviorBase* GmCtrlSet::transition(void* _ub)
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
                return this;
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
