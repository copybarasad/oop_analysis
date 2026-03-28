#include "SwitchAction.h"
#include "../LogSystem/Notifier.h"
#include "../LogSystem/SwitchEvent.h"

SwitchAction::SwitchAction(GameVisualizer<ConsoleRenderer>& view, Player& player): 
    view(view), player(player) {}

bool SwitchAction::execute() {
        if (this->player.getDMGMode() == DamageMode::Melee) {
            this->view.renderMessage("You switched from melee to ranged combat");
        }
        else {
            this->view.renderMessage("You switched from ranged to melee combat");
        }
        this->player.switchDMGMode();

        SwitchEvent event = SwitchEvent(this->player.getDMGMode());
        Notifier::getInstance().notify(event);
        return true;
    }
    
std::string SwitchAction::getName() const {
    return "Switch weapon";
}