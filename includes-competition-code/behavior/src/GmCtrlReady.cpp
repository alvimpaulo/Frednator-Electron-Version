#include <includeMapBehavior.hpp>
#include <Settings.hpp>
#include <qi/log.hpp>

#define MIN_STANDING_WEIGHT 0.55f
#define FALLEN 9
#define FALLING 8
#define FALLEN_ANG 70
#define FALLING_ANG 45

GmCtrlReady* GmCtrlReady::instance = NULL;

GmCtrlReady::GmCtrlReady()
{
    penalized = 0;
    readyNumber = 0;
}

BehaviorBase* GmCtrlReady::Behavior()
{
    if(! instance)
    {
        instance = new GmCtrlReady();
    }
    return instance;
}

void GmCtrlReady::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, gameState, (int)GAME_STATE::READY);

    //Do something
    ////qiLogInfo("Behavior") << "Ready!" << std::endl;

    gmCtrlData = readFrom(communication, gmCtrlData);
    int gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;
    BehaviorBase* current;
    //current = ReadyMachine::Behavior();
    current = readyWalk::Behavior();
    //commandStatic.leds = LEDAux::changeColor(3,0,0);

    if (readyNumber!=0) {
        //Se não for o primeiro Ready, ele não anda
        while((gmCtrlData.state==STATE_READY) && (penalized == 0))
        {
            gmCtrlData = readFrom(communication, gmCtrlData);
            gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;
            //STAND AQUI
        }
    }

    else {
        while((gmCtrlData.state==STATE_READY) && (penalized == 0))
        {
            //Se for o primeiro ready, ele anda um pouco para frente e vira
            gmCtrlData = readFrom(communication, gmCtrlData);
            gameControlPenalized = (int)gmCtrlData.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty;

            if ((gameControlPenalized> 0)&&(gameControlPenalized <10))
            {
                penalized=1;
            }
            else if(ButtonPressed(unBoard))
            {
                penalized=1;
                writeTo(behavior, play_chest, true);
            }
            else{
                penalized=0;
                if(!getUpIfFalling(unBoard))
                {
                    current->action(unBoard);
                    current = current->transition(unBoard);
                }
            }
        }
    }
}

BehaviorBase* GmCtrlReady::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    if (penalized==1){
        return GmCtrlPenalized::Behavior();
    }
    else {
        switch(gmCtrlData.state) {
            case STATE_INITIAL:
                return GmCtrlInitial::Behavior();
                break;
            case STATE_READY:
                return this;
                break;
            case STATE_SET:
                return GmCtrlSet::Behavior();
                break;
            case STATE_PLAYING:
                readyNumber++;
                return GmCtrlPlaying::Behavior();
                break;
            case STATE_FINISHED:
                return GmCtrlFinished::Behavior();
                break;
        }
    }
    
    return this;
}

bool GmCtrlReady::getUpIfFalling(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    SensorValues s = readFrom(motion, sensors);

    /*float fsr_sum = s.sensors[Sensors::LFoot_FSR_FrontLeft]
                    + s.sensors[Sensors::LFoot_FSR_FrontRight]
                    + s.sensors[Sensors::LFoot_FSR_RearLeft]
                    + s.sensors[Sensors::LFoot_FSR_RearRight]
                    + s.sensors[Sensors::RFoot_FSR_FrontLeft]
                    + s.sensors[Sensors::RFoot_FSR_FrontRight]
                    + s.sensors[Sensors::RFoot_FSR_RearLeft]
                    + s.sensors[Sensors::RFoot_FSR_RearRight];
    filtered_fsr_sum = filtered_fsr_sum + 0.2 * (fsr_sum - filtered_fsr_sum);*/
    // Verificar se o robo sera REF_PICKUP
    // Precisa de uma variavel externa filtered_frs_sum para armazenar o valor

    float ang[2] = {RAD2DEG(s.sensors[Sensors::InertialSensor_AngleX]),
                   RAD2DEG(s.sensors[Sensors::InertialSensor_AngleY])};
    ////std::cout << "X: " << ang[0] << " Y: " << ang[1] << std::endl;
    if(ang[1] < -FALLEN_ANG || ang[0] < -FALLEN_ANG)
    {
        getUpBack(unBoard);
        return true;
    }
    else if(ang[1] > FALLEN_ANG || ang[0] > FALLEN_ANG)
    {
        getUpFront(unBoard);
        return true;
    }
    else if (ABS(ang[0]) > FALLING_ANG || ABS(ang[1]) > FALLING_ANG)
    {
        deadStiff(unBoard);
        return true;
    }
    /*else if (filtered_fsr_sum < MIN_STANDING_WEIGHT)
    {
        refPickUp(unBoard);
        return true;
    }*/
    return false;
}

void GmCtrlReady::getUpBack(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, getUp, true);
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::GETUP_BACK;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action, unBoard);
}

void GmCtrlReady::getUpFront(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    writeTo(behavior, getUp, true);
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::GETUP_FRONT;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action, unBoard);
}

void GmCtrlReady::deadStiff(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::DEAD;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action, unBoard);
}

void GmCtrlReady::refPickUp(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::REF_PICKUP;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action, unBoard);
}

