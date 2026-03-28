#pragma once

class Wallet {
private:
    int balance;
public:
    Wallet(int start_balance = 0): balance(start_balance) {}
    ~Wallet() = default;

    const int getBalance() const { return balance; }

    void add(int x) {
        balance += x;
    }

    bool spend(int x) {
        if(balance < x) {
            return false;
        }
        balance-=x;
        return true;
    }
    
    void setBalance(int newBalance) {
        balance = newBalance;
    }
};