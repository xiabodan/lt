#include <iostream>
#include <tr1/functional>
// #include <tr1/memory>

class GameCh;
int defaultCalcFunc(const GameCh& gc) {
    std::cout << "defaultCalcFunc" << std::endl;
    return 0;
}

struct HealthCalculator {
    int operator() (const GameCh& gc) const {
        return 1;
    }
};

class GameLevel {
public:
    float health(const GameCh&) const {

    }
};

class GameCh{
public:
    typedef std::tr1::function<int (const GameCh&)> HealthCalcFunc;
    // typedef int (*HealthCalcFunc)(const GameCh&);

    explicit GameCh(HealthCalcFunc hcf = defaultCalcFunc)
    :healthFunc(hcf){

    }

    int getHealth(){
        return healthFunc(*this);
    }
    void setCalcFunc(HealthCalcFunc hcf){
        healthFunc = hcf;
    }
    virtual void draw(int a = 1){
        std::cout << "GameCh" << std::endl; 
    }
private:
    HealthCalcFunc healthFunc;
};


int loseHighCalcFunc(const GameCh& gc){
    std::cout << "loseHighCalcFunc" << std::endl;
    return 1;
}
int loselittleCalcFunc(const GameCh& gc){
    std::cout << "loselittleCalcFunc" << std::endl;
    return 2;
}
int loseMudiumCalcFunc(const GameCh& gc){
    std::cout << "loseMudiumCalcFunc" << std::endl;
    return 3;
}

class Timor: public GameCh{
public:
    explicit Timor(HealthCalcFunc hcf = defaultCalcFunc)
    :GameCh(hcf){

    }

    void setHealthCalcFunc(HealthCalcFunc hcf){
        setCalcFunc(hcf);
    }

    virtual void draw(int a){
        std::cout << "Timor" << std::endl; 
    }
};


class EyeCandyCh : public GameCh {
public:
    explicit EyeCandyCh(HealthCalcFunc hcf = defaultCalcFunc)
    :GameCh(hcf) {
        std::cout << "EyeCandyCh " << hcf << std::endl;
    }
};

class Evil : public GameCh {
public:
    explicit EyeCandyCh(HealthCalcFunc hcf = defaultCalcFunc)
    :GameCh(hcf) {

    }
};

int main(){
    Timor bo;
    Timor hubert(loseHighCalcFunc);
    Timor xia(loselittleCalcFunc);
    bo.getHealth();
    hubert.getHealth();
    xia.getHealth();

    bo.setHealthCalcFunc(loseMudiumCalcFunc);
    bo.getHealth();
    hubert.getHealth();
    xia.getHealth();

    GameCh* h = new Timor;
    h->draw();

    // EyeCandyCh ebg1(HealthCalculator());

    GameLevel currentLevel;
    Evil ecc1(std::tr1::bind(&GameLevel::health, currentLevel, _1));

    return 0;
}
