#include <motionAuxiliarFunctions.hpp>
#include <includeMapBehavior.hpp>
#include <BehaviorBase.hpp>

//auxiliar functions for all motion behaviors

namespace MotionAux
{
	#define MIN_STANDING_WEIGHT 0.55f
	#define FALLEN 9
	#define FALLING 8
	#define FALLEN_ANG 70
	#define FALLING_ANG 45

    unBoard<Command0> CommandBoard0;
    unBoard<Command1> CommandBoard1;
    unBoard<CommandIndex> indexBoard;

	ActionCommand::Body Walk (const float& x, const float& y, const float& theta)
	{
	    ActionCommand::Body body;

	    body.actionType = ActionCommand::Body::WALK;
	    body.forward = x;
	    body.left = y;
	    body.turn = theta;
	    body.power = 1.0;
	    body.bend = 15.0;

	    return body;
	}

    ActionCommand::Body Kick (const float& x, const float& y, const float& direction, const ActionCommand::Body::Foot& foot, const bool& misaligned)
	{
	    ActionCommand::Body body;

	    body.actionType = ActionCommand::Body::KICK;
	    body.ballX = x;
	    body.ballY = y;
	    body.kickDirection = direction;
	    body.foot = foot;
	    body.misalignedKick = misaligned;
	    body.power = 1.0;
	    body.speed = 1.0;
	    body.bend = 15.0;

	    return body;
	}

    ActionCommand::Head MoveHead (const float& pitch, const float& yaw, const float& pSpeed, const float& ySpeed, const bool& isRelative)
	{
	    ActionCommand::Head head;

	    head.pitch = pitch;
	    head.yaw = yaw;
	    head.pitchSpeed = pSpeed;
	    head.yawSpeed = ySpeed;
	    head.isRelative = isRelative;

	    return head;
	}

	ActionCommand::Body Stop()
	{
	    ActionCommand::Body body;

	    body.actionType = ActionCommand::Body::WALK;
	    body.forward = 0.0;
	    body.left = 0.0;
	    body.turn = 0.0;
	    body.power = 1.0;
	    body.bend = 15.0;

	    return body;
	}

	ActionCommand::Body Stand()
	{
	    ActionCommand::Body body;

	    body.actionType = ActionCommand::Body::STAND;
	    body.forward = 0.0;
	    body.left = 0.0;
	    body.turn = 0.0;
	    body.power = 1.0;
	    body.bend = 15.0;

	    return body;
	}

	ActionCommand::Body Penalize()
	{
	    ActionCommand::Body body;

	    body.actionType = ActionCommand::Body::STAND;
	    body.forward = 0.0;
	    body.left = 0.0;
	    body.turn = 0.0;
	    body.power = 1.0;
	    body.bend = 15.0;

	    return body;
	}

	BehaviorBase* getUpIfFalling(SensorValues &s)
	{



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
		std::cout << "X: " << ang[0] << " Y: " << ang[1] << std::endl;
		if(ang[1] < -FALLEN_ANG || ang[0] < -FALLEN_ANG)
		{
		    return getUpBack::Behavior();
		}
		else if(ang[1] > FALLEN_ANG || ang[0] > FALLEN_ANG)
		{
		    return getUpFront::Behavior();
		}
		else if (ABS(ang[0]) > FALLING_ANG || ABS(ang[1]) > FALLING_ANG)
		{
		    return deadStiffness::Behavior();
		}
		else if (filtered_fsr_sum < MIN_STANDING_WEIGHT)
		{
		    return refPickup::Behavior();
		}
		else return NULL;
	}

    ActionCommand::Body GetUpFront()
	{
        ActionCommand::Body body;

        body.actionType = ActionCommand::Body::GETUP_FRONT;
        body.power = 1.0;
        body.bend = 15.0;
        body.forward = 0.0;
        body.left = 0.0;
        body.turn = 0.0;

        return body;
	}

    ActionCommand::Body GetUpBack()
	{

        ActionCommand::Body body;

        body.actionType = ActionCommand::Body::GETUP_BACK;
        body.power = 1.0;
        body.bend = 15.0;
        body.forward = 0.0;
        body.left = 0.0;
        body.turn = 0.0;

        return body;
	}
    ActionCommand::Body DeadStiffness()
	{

        ActionCommand::Body body;

        body.actionType = ActionCommand::Body::DEAD;
        body.power = 1.0;
        body.bend = 15.0;
        body.forward = 0.0;
        body.left = 0.0;
        body.turn = 0.0;

        return body;

	}

    ActionCommand::Body RefPickup()
	{

        ActionCommand::Body body;

        body.actionType = ActionCommand::Body::REF_PICKUP;
        body.power = 1.0;
        body.bend = 15.0;
        body.forward = 0.0;
        body.left = 0.0;
        body.turn = 0.0;

        return body;

	}



	void SendCommand(const Command& command)
	{
	    CommandIndex index = indexBoard.load();
	    index.index = (index.index + 1) % 2;
	    if(index.index)
	    {
	        Command1 command1;
	        command1 = command;
	        CommandBoard1.save(command1);
	        indexBoard.save(index);
	    }
	    else
	    {
	        Command0 command0;
	        command0 = command;
	        CommandBoard0.save(command0);
	        indexBoard.save(index);
	    }
	}
};
