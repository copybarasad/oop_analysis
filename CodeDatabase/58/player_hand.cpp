#include "player_hand.hpp"

PlayerHand::PlayerHand(int size_hand){
    if (size_hand > 1000 || size_hand < 1){
        size_hand = BASE_SIZE_HAND;
    }
    this->size_hand = size_hand;

    add_random_card();
}


PlayerHand::PlayerHand(const PlayerHand& other) : size_hand(other.size_hand){
    for(auto card : other.cards){
        if(auto waterball = dynamic_cast<WaterBall*>(card)){
            cards.push_back(new WaterBall(*waterball));
        } else if(auto banana_trap = dynamic_cast<BananaTrap*>(card)){
            cards.push_back(new BananaTrap(*banana_trap));
        } else if(auto poison_cloud = dynamic_cast<PoisonCloud*>(card)){
            cards.push_back(new PoisonCloud(*poison_cloud));
        }
    }
}

int PlayerHand::random(int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, max - 1);

    return distrib(gen);
}

PlayerHand::~PlayerHand(){
    for (auto card : cards){
        delete card;
    }
    cards.clear();
}


void PlayerHand::add_random_card(){
    int c = random(30);
    if (c < 10) cards.push_back(new WaterBall(3));
    else if (c < 20) cards.push_back(new PoisonCloud());
    else if (c < 30) cards.push_back(new BananaTrap());
}
void PlayerHand::set_cards(std::vector<ISpellCard*>& cards){
    for(auto card : this->cards){
        delete card;
    }
    this->cards.clear();

    for(auto card : cards){
        if(card){
            this->cards.push_back(card);
        }
    }
    cards.clear();
}

void PlayerHand::remove_card(int index){
    delete cards[index];
    cards.erase(cards.begin() + index);
}
void PlayerHand::random_remove_card(){
    if (cards.empty()) return;

    int cnt_delete = get_cur_size_hand() / 2;
    if (cnt_delete == 0) cnt_delete = 1;
    int deleted = 0;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (deleted < cnt_delete && !cards.empty()) {
        std::uniform_int_distribution<> distrib(0, cards.size() - 1);
        int random_index = distrib(gen);
        
        std::uniform_int_distribution<> percent_distrib(0, 99);
        if (percent_distrib(gen) < 50) {
            remove_card(random_index);
            deleted++;
        }
    }
}

void PlayerHand::set_size_hand(int size_hand) {this->size_hand = size_hand;};

int PlayerHand::get_max_size_hand() {return size_hand;};

int PlayerHand::get_cur_size_hand() {return cards.size();};

int PlayerHand::use_card(World* world){
    std::cout << "enter num of card: ";

    int num_card;
        if(!(std::cin >> num_card)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "error enter!" << std::endl;
        return 1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (num_card < 0 || num_card >= (int)cards.size()){
        std::cout << "num of card out of range" << std::endl;
        return 1;
    }

    if(!cards[num_card]->cast(world)) return 1;

    delete cards[num_card];
    cards.erase(cards.begin() + num_card);
    return 0;
}


std::vector<ISpellCard*> PlayerHand::get_cards() {return cards;};