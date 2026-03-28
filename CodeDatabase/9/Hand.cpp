#include "Hand.h"

Hand::Hand(int capacity){
    capacity_ = capacity;
    selected_ = 0;
}

Hand::~Hand(){
    for(size_t i = 0; i < _cards.size(); ++i){
        delete _cards[i];
    }
}

Hand::Hand(const Hand& other){
    capacity_ = other.capacity_;
    selected_ = other.selected_;
    for(size_t i = 0; i < other._cards.size(); ++i){
        _cards.push_back(other._cards[i]->CloneNew());
    }
}

Hand& Hand::operator=(const Hand& other){
    if(this == &other){
        return *this;
    }
    for(size_t i = 0; i < _cards.size(); ++i){
        delete _cards[i];
    }
    _cards.clear();
    capacity_ = other.capacity_;
    selected_ = other.selected_;
    for(size_t i = 0; i < other._cards.size(); ++i){
        _cards.push_back(other._cards[i]->CloneNew());
    }
    return *this;
}

int Hand::GetCapacity() const{
    return capacity_;
}

int Hand::GetCount() const{
    return (int)_cards.size();
}

int Hand::GetSelectedIndex() const{
    return selected_;
}

void Hand::SelectByIndex(int index){
    if(index >= 0 && index < (int)_cards.size()){
        selected_ = index;
    }
}

ISpell* Hand::GetSelected(){
    if(_cards.empty()){
        return 0;
    }
    return _cards[selected_];
}

bool Hand::AddCard(ISpell* card){
    if((int)_cards.size() >= capacity_){
        return false;
    }
    _cards.push_back(card);
    return true;
}

void Hand::GetCardNames(std::vector<const char*>& out){
    out.clear();
    for(size_t i = 0; i < _cards.size(); ++i){
        out.push_back(_cards[i]->GetName());
    }
}

bool Hand::RemoveSelected(){
    if(_cards.empty()){
        return false;
    }
    delete _cards[selected_];
    _cards.erase(_cards.begin() + selected_);
    if(selected_ >= (int)_cards.size()){
        selected_ = (int)_cards.size() - 1;
    }
    if(selected_ < 0){
        selected_ = 0;
    }
    return true;
}