#ifndef SAVE_S_H
#define SAVE_S_H

#include "../../SaveLoadException/SaveLoadException/SaveLoadException.hpp"
#include "modules/Services/SaveLoadServices/Serializable/BaseSerializable/Serializable.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/CellSerializer/CellSerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/EntityManagerSerializer/EntityManagerSerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/EntitySerializer/EntitySerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/FieldSerializer/FieldSerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/GameStateSerializer/GameStateSerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/ItemSerializer/ItemSerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/PlayerSerializer/PlayerSerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/SpellHandSerializer/SpellHandSerializer.hpp"
#include "modules/Services/SaveLoadServices/Serializable/GameStateSerializers/SpellStoreSerializer/SpellStoreSerializer.hpp"

class SaveService {
    public:
    virtual ~SaveService () = default;

    virtual void save (const std::string& saveName) = 0;
};

#endif