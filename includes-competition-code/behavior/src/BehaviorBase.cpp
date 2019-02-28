#include <BehaviorBase.hpp>


BehaviorBase* BehaviorBase::lastBehavior = NULL;

void BehaviorBase::writeCommand(ActionCommand::All action, void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    int index = readFrom(behavior, requestIndex);
    int actionIndex = readFrom(behavior, request[index].actions.actionIndex);
    actionIndex++;
    index = (index + 1) % 2;
    action.actionIndex = actionIndex;
    writeTo(behavior, request[index].actions, action);
    writeTo(behavior, requestIndex, index);
}

bool BehaviorBase::ButtonPressed(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    ButtonPresses button = readFrom(motion, button);
    if(button.pop(1))
    {
        //Chest Button pressed once
        writeTo(motion, button, button);
        return true;
    }
    writeTo(motion, button, button);
    return false;
}

ActionCommand::rgb BehaviorBase::getChestColor(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    int gameState = readFrom(behavior, gameState);
    ActionCommand::rgb chestButton;

    if(gameState == GAME_STATE::INITIAL || gameState == GAME_STATE::FINISHED)
    {
        chestButton.blue = 0;
        chestButton.red = 0;
        chestButton.green = 0;
    }
    else if(gameState == GAME_STATE::READY)
    {
        chestButton.blue = 1;
        chestButton.red = 0;
        chestButton.green = 0;
    }
    else if(gameState == GAME_STATE::SET)
    {
        chestButton.blue = 1;
        chestButton.red = 0;
        chestButton.green = 1;
    }
    else if(gameState == GAME_STATE::PLAYING)
    {
        chestButton.blue = 0;
        chestButton.red = 0;
        chestButton.green = 1;
    }

    return chestButton;
}

SonarClass* BehaviorBase::sonarStatic = new SonarClass("127.0.0.1");
