#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


/*
PART ONE

A = ROCK
B = PAPER
C = SCISSORS

X = ROCK        1
Y = PAPER       2   
Z = SCISSORS    3

WIN             6
DRAW            3   
LOSS            0

IF ROUND IS U vs V (OPPONENT vs PLAYER)
CONVERT ACCORDING TO ROCK=0,PAPER=1,SCISSOR=2
THEN
WIN IF  (V-U)mod 3 = 1 OR -2
DRAW IF (V-U)mod 3 = 0 
LOSS IF (V-U)mod 3 = 2 OR -1
*/

/*
PART TWO

A = ROCK        1
B = PAPER       2
C = SCISSORS    3

X = LOSS        2
Y = DRAW        0
Z = WIN         1

WIN             6
DRAW            3   
LOSS            0

IF OPPONENT PLAYS U AND EXPECTED OUTCOME IS V
CONVERT V ACCORDING TO WIN=1, DRAW=0, LOSS=2
PLAYER NEEDS TO PLAY (U+V)mod 3

*/

int convert(char player){
    switch (player){

        case 'A':
        case 'X':
            return 0;

        case 'B':
        case 'Y':
            return 1;

        case 'C':
        case 'Z':
            return 2;

        default:
            std::cerr<<"error\n";
            return -1;

    }
}

int convert2(char player){
    switch (player){

        case 'Y':
            return 0;

        case 'A':
        case 'Z':
            return 1;

        case 'B':
        case 'X':
            return 2;

        case 'C':
            return 3;

        default:
            std::cerr<<"error\n";
            return -1;

    }
}

int playRound(char opponent,char player){
    int opValue{convert(opponent)};
    int plValue{convert(player)};
    int score{};

    int roundValue{(plValue-opValue)%3};
    if (roundValue==1 || roundValue==-2) score += 6;
    else if (roundValue==0) score += 3;

    score += (plValue +1);

    return score;
}

int playRound2(char opponent,char expected){
    int opValue{convert2(opponent)};
    int exValue{convert2(expected)};

    int plValue{(opValue+exValue)%3};
    if (plValue==0) plValue=3;

    int score{};
    if (exValue==1) score += 6;
    else if (exValue==0) score += 3;

    score += plValue;
    return score;
}

int main(){

    std::ifstream file("data2.txt");
    std::string str{};
    char opponent{};
    char player{};
    int score_1{};
    int score_2{};

    while(getline(file,str)){
        std::stringstream ss{str};
        ss>>opponent>>player;
        
        //PART ONE
        score_1 += playRound(opponent,player);

        //PART TWO
        score_2 += playRound2(opponent,player);

    }
    std::cout<<score_1<<'\n';
    std::cout<<score_2<<'\n';

    return 0;
}



