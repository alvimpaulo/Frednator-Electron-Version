#pragma once
#include <unBoard.hpp>
#include <motionData.hpp>
#include <types/ActionCommand.hpp>
#include <motionAuxiliarFunctions.hpp>

namespace LEDAux
{
    static float filtered_fsr_sum = 0;

	ActionCommand::LED changeColor (int Chest, int REye , int LEye);
}
