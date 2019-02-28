#include <includeMapBehavior.hpp>
#include <behavior.hpp>


#include <qi/log.hpp>

#define MIN_STANDING_WEIGHT 0.55f
#define FALLEN 9
#define FALLING 8
#define FALLEN_ANG 70
#define FALLING_ANG 45

behavior::behavior(void* _ub) : filtered_fsr_sum(0)
{
    this->unBoard = static_cast<UnBoard*>(_ub);
    //stand();

    // Initial Behavior
    current = GmCtrlInitial::Behavior();
    //current = Start::Behavior();

    //walk(0.0, 0.0, 1.57);
}

void behavior::tick()
{

    if(!getUpIfFalling())
    {
        current->action(this->unBoard);
        current = current->transition(this->unBoard);
/*
        std::string input;
        std::cout << "Input: (S = stand, F = Forward, B = Backward, L = Left, R = Right)" << std::endl;
        std::cin >> input;

        if(input == "S" || input == "s")
        {
            stand();
        }
        else if(input == "F" || input == "f")
        {
            walk(300.0, 0.0, 0.0);
        }
        else if(input == "B" || input == "b")
        {
            walk(-300.0, 0.0, 0.0);
        }
        else if(input == "L" || input == "l")
        {
            walk(0.0, 300.0, 0.0);
        }
        else if(input == "R" || input == "r")
        {
            walk(0.0, -300.0, 0.0);
        }
        else if(input == "K" || input == "k")
        {
            kick(160.0, -45.0, 0.0, ActionCommand::Body::RIGHT);
        }

        /*int kF = readFrom(motion, kickFinished);
        if(kF != 1)
            kick();
        else
            stand();*/


        //stand();
    }
    else
    {

        current = Start::Behavior();
    }

    /*int ballX = readFrom(vision, ballX);
    int ballY = readFrom(vision, ballY);
    if(ballX != -1)
        lightRedLed();
    qiLogInfo("Behavior") << "ball pos " << ballX << " " << ballY << std::endl;*/



   //int fieldDistance = readFrom(perception, fieldDistance);


    /*int kickF = readFrom(motion, kickFinished);
    if(kickF != 1)
    {
        kick();
    }
    else
    {
        stand();
    }*/
}

void behavior::stand()
{
    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::STAND;

    writeCommand(action);
}

void behavior::kick(const float &x, const float &y, const float &dir, const ActionCommand::Body::Foot &foot)
{
    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::KICK;
    action.body.ballX = x;
    action.body.ballY = y;
    action.body.kickDirection = dir;
    action.body.foot = foot;
    action.body.misalignedKick = false;
    action.body.power = 1.0;
    action.body.speed = 1.0;
    action.body.bend = 15.0;



    writeCommand(action);
}

void behavior::walk(const float &x, const float &y, const float &theta)
{
    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = x;
    action.body.left = y;
    action.body.turn = theta;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    writeCommand(action);
}

bool behavior::getUpIfFalling()
{
    SensorValues s = readFrom(motion, sensors);

    float fsr_sum = s.sensors[Sensors::LFoot_FSR_FrontLeft]
                    + s.sensors[Sensors::LFoot_FSR_FrontRight]
                    + s.sensors[Sensors::LFoot_FSR_RearLeft]
                    + s.sensors[Sensors::LFoot_FSR_RearRight]
                    + s.sensors[Sensors::RFoot_FSR_FrontLeft]
                    + s.sensors[Sensors::RFoot_FSR_FrontRight]
                    + s.sensors[Sensors::RFoot_FSR_RearLeft]
                    + s.sensors[Sensors::RFoot_FSR_RearRight];
    filtered_fsr_sum = filtered_fsr_sum + 0.2 * (fsr_sum - filtered_fsr_sum);
    // Verificar se o robo sera REF_PICKUP
    // Precisa de uma variavel externa filtered_frs_sum para armazenar o valor

    float ang[2] = {RAD2DEG(s.sensors[Sensors::InertialSensor_AngleX]),
                   RAD2DEG(s.sensors[Sensors::InertialSensor_AngleY])};
    //std::cout << "X: " << ang[0] << " Y: " << ang[1] << std::endl;
    if(ang[1] < -FALLEN_ANG || ang[0] < -FALLEN_ANG)
    {
        getUpBack();
        return true;
    }
    else if(ang[1] > FALLEN_ANG || ang[0] > FALLEN_ANG)
    {
        getUpFront();
        return true;
    }
    else if (ABS(ang[0]) > FALLING_ANG || ABS(ang[1]) > FALLING_ANG)
    {
        deadStiff();
        return true;
    }
    else if (filtered_fsr_sum < MIN_STANDING_WEIGHT)
    {
        refPickUp();
        return true;
    }
    return false;
}

void behavior::getUpBack()
{
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::GETUP_BACK;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action);
}

void behavior::getUpFront()
{
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::GETUP_FRONT;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action);
}

void behavior::deadStiff()
{
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::DEAD;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action);
}

void behavior::refPickUp()
{
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::REF_PICKUP;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;

    writeCommand(action);
}

void behavior::lightRedLed()
{
    ActionCommand::All action;

    action.leds.leftEye.red = 1;
    action.leds.leftEye.blue = 0;
    action.leds.leftEye.green = 0;

    writeCommand(action);
}

void behavior::writeCommand(ActionCommand::All action)
{
    int index = readFrom(behavior, requestIndex);
    int actionIndex = readFrom(behavior, request[index].actions.actionIndex);
    actionIndex++;
    index = (index + 1) % 2;
    action.actionIndex = actionIndex;
    writeTo(behavior, request[index].actions, action);
    writeTo(behavior, requestIndex, index);
}
