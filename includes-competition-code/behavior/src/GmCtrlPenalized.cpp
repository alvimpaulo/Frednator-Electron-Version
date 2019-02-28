#include <includeMapBehavior.hpp>
#include <Settings.hpp>
#include <qi/log.hpp>

GmCtrlPenalized* GmCtrlPenalized::instance = NULL;

GmCtrlPenalized::GmCtrlPenalized()
{
    penalized = 1;
    button_pressed = 0;
    play_chest = false;
}

BehaviorBase* GmCtrlPenalized::Behavior()
{
    if(! instance)
    {
        instance = new GmCtrlPenalized();
    }
    return instance;
}

void GmCtrlPenalized::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    ////qiLogInfo("Behavior") << "Penalized!" << std::endl;

    penalized = 1;
    gmCtrlData = readFrom(communication, gmCtrlData);
    int gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;

    ////qiLogInfo("Behavior") << "Penalized! "<< gameControlPenalized << std::endl;

    if ((gameControlPenalized> 0)&&(gameControlPenalized <10)) {
        penalized=1;
    }
    else if(ButtonPressed(unBoard)) {
        writeTo(behavior, play_chest, false);
        penalized=0;
    }

    while(penalized==1)
    {
        //Set Led Color to Red and Position to Penalized
        ActionCommand::All action;
        action.body.actionType = ActionCommand::Body::REF_PICKUP;
        action.leds.chestButton.red = 1;
        action.leds.chestButton.blue = 0;
        action.leds.chestButton.green = 0;
        writeCommand(action, unBoard);

        //Check if still penalized
        gmCtrlData = readFrom(communication, gmCtrlData);
        gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;
        play_chest = readFrom(behavior, play_chest);

        //commandStatic.leds = LEDAux::changeColor(1,0,0);
        if ((gameControlPenalized==0)&&(!play_chest)) {
            penalized= 0;
        }
        else if(ButtonPressed(unBoard))
        {
            penalized = 0;
            writeTo(behavior, play_chest, false);
            button_pressed = 1;
        }
        else {
            penalized = 1;
        }
    }
}

BehaviorBase* GmCtrlPenalized::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    if (penalized == 1)
    {
        return this;
    }
    else
    {
        if(button_pressed)
        {
            gmCtrlData = readFrom(communication, gmCtrlData);
            gmCtrlData.state = STATE_PLAYING;
            gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty = 0;
            writeTo(communication, gmCtrlData, gmCtrlData);
            button_pressed = 0;
            return GmCtrlPlaying::Behavior();
        }
        else
        {
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
                return GmCtrlFinished::Behavior();
                break;
            }
        }

    }
    
    return this;
}
