#pragma once

class Cooldown 
{
protected:
    int SpeedCD;
    int ATKCD;
public:
    Cooldown(int SpeedCD = 0, int ATKCD = 0):SpeedCD(SpeedCD),ATKCD(ATKCD){}

    void setSpeedCD(int duration) { this->SpeedCD = this->SpeedCD+duration; }
    void setATKCD(int duration) { this->ATKCD = this->ATKCD+duration; }
    void resetSpeedCD(int duration) { this->SpeedCD = duration; }
    void resetATKCD(int duration) { this->ATKCD = duration; }
    int checkSpeedCD() const { return this->SpeedCD; }
    int checkATKCD() const { return this->ATKCD; }
};