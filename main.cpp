#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include <array>
#include "history.h"

using namespace std;

const char* gameName[] = {"Lotto", "Power ball"};
enum class enGame:uint8_t
{
    LOTTO = 1,
    LOTTOP1,
    LOTTOP2,
    POWERB,
    POWERBP
};

typedef struct _machine
{
    _machine(uint8_t max_num, bool seed=true): _max(max_num)
    {
        if(seed)
            random_engine.seed(std::time(nullptr));
    }

    uint8_t getNumber()
    {
        std::uniform_int_distribution<> uid(1, _max);
        return static_cast<uint8_t>(uid(random_engine));
    }
    void seedRE(unsigned int value)
    {
        random_engine.seed(value);
    }
private:
    uint8_t _max;
    std::default_random_engine random_engine;

}machine;

void generateLNumbers(int lines, enGame gm)
{
    uint8_t _max = 52, pb = Powerb[5], numInLine = 6;
    std::vector<uint8_t> numbers, recentNumbers;
    std::string prompt;

    switch(gm)
    {
    case enGame::LOTTO:
        prompt = "Generating Lotto numbers, please be patient";
        recentNumbers = Lotto;
        break;
    case enGame::LOTTOP1:
        prompt = "Generating Lottop1 numbers, please be patient";
        recentNumbers = Lottop1;
        break;
    case enGame::LOTTOP2:
        prompt = "Generating Lottop2 numbers, please be patient";
        recentNumbers = Lottop2;
        break;
    case enGame::POWERB:
        prompt = "Generating PowerBall numbers, please be patient";
        _max = 50;
        numInLine = 5;
        recentNumbers = Powerb;
        recentNumbers.resize(5);
        break;
    case enGame::POWERBP:
        prompt = "Generating PowerBallp numbers, please be patient";
        _max = 50;
        numInLine = 5;
        recentNumbers = Powerbp;
        recentNumbers.resize(5);
        pb = Powerbp[5];
        break;
    }
    cout << prompt << endl;

    machine Kleinbos(_max, false);
    machine Powerball(20, false);
    //machine Kleinbos(_max, false);
    std::array<bool, 52> selected;
    uint8_t minutes = 0, hours = 0;
    std::time_t timer = std::time(nullptr) + 60;

    do
    {
        if(std::time(nullptr) > timer)
        {
            cout << ".";
            timer = std::time(nullptr) + 60;
            if(++minutes >= 120) //Seed every 2 hour
            {
                minutes = 0;
                Kleinbos.seedRE(timer);
                cout << (int)(++hours * 2) << endl;
            }
        }

        numbers.clear();
        selected.fill(false);
        for(int i = 0; i < numInLine; i++)
        {
            uint8_t rnum = Kleinbos.getNumber();
            while(selected[rnum] == true)
                rnum = Kleinbos.getNumber();
            numbers.push_back(rnum);
            selected[rnum] = true;
        }

    }while(numbers != recentNumbers);

    if(_max == 50)
    {
        uint8_t rnum;
        do
        {
            rnum = Powerball.getNumber();
            if(std::time(nullptr) > timer)
            {
                cout << ".";
                timer = std::time(nullptr) + 60;
            }

        }while(pb != rnum);
    }


    cout << "\n\nYour numbers are:\n***********************\n***********************\n";
    for(int line = 1; line <= lines; line++)
    {
        uint8_t rnum;
        numbers.clear();
        selected.fill(false);
        for(int num = 0; num < numInLine; num++)
        {
            rnum = Kleinbos.getNumber();
            while(selected[rnum] == true)
                rnum = Kleinbos.getNumber();
            numbers.push_back(rnum);
            selected[rnum] = true;
        }
        std::sort(numbers.begin(), numbers.end());

        if(_max == 50)
            numbers.push_back(Powerball.getNumber());

        cout << "** ";
        for(auto n : numbers)
            cout << std::setfill('0') << std::setw(2) << (int)n << " ";
        cout << "**";
        cout << endl;
    }
    cout << "***********************\n***********************\nGOOD LUCK!\n";
}

int main(int argc, char **argv)
{
    enGame game;
    int lines = 0;

    cout << "Welcome to SALottery" << endl;
    if(argc > 1)
    {
        game = (enGame)atoi(argv[1]);
        lines = atoi(argv[2]);
        generateLNumbers(lines, game);
    }
    else
    {
        cout << "Usage: " << argv[0] << " game lines\n";
    }
    return 0;
}
