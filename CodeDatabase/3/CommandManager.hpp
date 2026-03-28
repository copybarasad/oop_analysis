#include "GameClass/GameClass.hpp"
#include "vector"

template<typename InputHandler>
class CommandManager {
public:
    template<typename ... Args>
    CommandManager(GameClass& game, Args&& ... args)
        : game(game), inputHandler(std::forward<Args>(args)...) {}

    void handle() {
        Motion motion = translateCommand(inputHandler.getCommand(), game.getState());
        std::vector<Motion> allowedMotions = game.getAllowedMotions();
        
        if (std::find(allowedMotions.begin(), allowedMotions.end(), motion) != allowedMotions.end()) {
            game.gameLoop(motion);
        }
    }

    bool isAlive() {
        return inputHandler.isAlive();
    }

private:
    GameClass& game;
    InputHandler inputHandler;

    Motion translateCommand(Command cmd, GameState state) {
        switch (cmd)
        {
        case Command::Cancel:
            return Motion::CancelCast;
        case Command::Confirm:
            return Motion::ConfirmCast;
        case Command::New:
            return Motion::NewGame;
        case Command::Load:
            return Motion::LoadSave;
        case Command::Finish:
            return Motion::FinishLevel;
        case Command::Save:
            return Motion::SaveGame;
        case Command::Change:
            return Motion::ChangeForm;

        case Command::One:
            return state == GameState::Improve ? Motion::UpgradeAreaS : Motion::StartCast1;
        case Command::Two:
            return state == GameState::Improve ? Motion::UpgradeTargetS : Motion::StartCast2;
        case Command::Three:
            return state == GameState::Improve ? Motion::UpgradeTrapS : Motion::StartCast3;
        case Command::Four:
            return state == GameState::Improve ? Motion::UpgradeDamage : Motion::StartCast4;
        case Command::Five:
            return state == GameState::Improve ? Motion::UpgradeHp : Motion::StartCast5;
        
        case Command::Up:
            return state == GameState::OnCast ? Motion::MoveUpCast : Motion::MoveUp;
        case Command::Down:
            return state == GameState::OnCast ? Motion::MoveDownCast : Motion::MoveDown;
        case Command::Left:
            return state == GameState::OnCast ? Motion::MoveLeftCast : Motion::MoveLeft;
        case Command::Right:
            return state == GameState::OnCast ? Motion::MoveRightCast : Motion::MoveRight;
        
        default:
            return Motion::None;
        }
    }
};
