#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>


class Monkey{
    public:
    std::queue<long long> items{};
    char operation{};   //'+' '*' '2'
    int opValue{};
    int divisibleBy{};
    int ifTrue{};
    int ifFalse{};

    int inspected{};
};



int main(){

    std::vector<Monkey> monkeys;
    bool part_one{false};
    
    //MONKEY 0
    Monkey monkey0{};
    monkey0.items = std::queue<long long> ({74,73,57,77,74});
    monkey0.operation = '*';
    monkey0.opValue = 11;
    monkey0.divisibleBy = 19;
    monkey0.ifTrue = 6;
    monkey0.ifFalse = 7;
    monkeys.push_back(monkey0);

    //MONKEY 1
    Monkey monkey1{};
    monkey1.items = std::queue<long long> ({99,77,79});
    monkey1.operation = '+';
    monkey1.opValue = 8;
    monkey1.divisibleBy = 2;
    monkey1.ifTrue = 6;
    monkey1.ifFalse = 0;
    monkeys.push_back(monkey1);

    //MONKEY 2
    Monkey monkey2{};
    monkey2.items = std::queue<long long> ({64,67,50,96,89,82,82});
    monkey2.operation = '+';
    monkey2.opValue = 1;
    monkey2.divisibleBy = 3;
    monkey2.ifTrue = 5;
    monkey2.ifFalse = 3;
    monkeys.push_back(monkey2);

    //MONKEY 3
    Monkey monkey3{};
    monkey3.items = std::queue<long long> ({88});
    monkey3.operation = '*';
    monkey3.opValue = 7;
    monkey3.divisibleBy = 17;
    monkey3.ifTrue = 5;
    monkey3.ifFalse = 4;
    monkeys.push_back(monkey3);

    //MONKEY 4
    Monkey monkey4{};
    monkey4.items = std::queue<long long> ({80,66,98,83,70,63,57,66});
    monkey4.operation = '+';
    monkey4.opValue = 4;
    monkey4.divisibleBy = 13;
    monkey4.ifTrue = 0;
    monkey4.ifFalse = 1;
    monkeys.push_back(monkey4);

    //MONKEY 5
    Monkey monkey5{};
    monkey5.items = std::queue<long long> ({81,93,90,61,62,64});
    monkey5.operation = '+';
    monkey5.opValue = 7;
    monkey5.divisibleBy = 7;
    monkey5.ifTrue = 1;
    monkey5.ifFalse = 4;
    monkeys.push_back(monkey5);

    //MONKEY 6
    Monkey monkey6{};
    monkey6.items = std::queue<long long> ({69,97,88,93});
    monkey6.operation = '2';
    monkey6.opValue = 0;
    monkey6.divisibleBy = 5;
    monkey6.ifTrue = 7;
    monkey6.ifFalse = 2;
    monkeys.push_back(monkey6);

    //MONKEY 7
    Monkey monkey7{};
    monkey7.items = std::queue<long long> ({59,80});
    monkey7.operation = '+';
    monkey7.opValue = 6;
    monkey7.divisibleBy = 11;
    monkey7.ifTrue = 2;
    monkey7.ifFalse = 3;
    monkeys.push_back(monkey7);



    int rounds{};
    if (part_one) rounds = 20;
    else rounds = 10000;

    for (int round=0;round<rounds;++round){
        for (int i=0;i<monkeys.size();++i){
            while (!monkeys[i].items.empty()){
                long long level{monkeys[i].items.front()};

                switch (monkeys[i].operation){
                    case '+':
                        level += monkeys[i].opValue;
                        break;
                    case '*':
                        level *= monkeys[i].opValue;
                        break;
                    case '2':
                        level *= level;
                        break;
                    default:
                        std::cout<<"error\n";
                        break;
                }

                if (part_one) level = level/3;
                else level = level%(19*2*3*17*13*7*5*11);

                if (level%monkeys[i].divisibleBy==0){
                    monkeys[monkeys[i].ifTrue].items.push(level);
                    monkeys[i].items.pop();
                }else{
                    monkeys[monkeys[i].ifFalse].items.push(level);
                    monkeys[i].items.pop();
                }

                ++monkeys[i].inspected;
            }
        }
    }

    std::vector<long long> inspected{};
    for (Monkey m: monkeys){
        inspected.push_back(m.inspected);
    }
    std::sort(inspected.begin(),inspected.end());
    std::cout<<inspected.back() * *(inspected.end()-2)<<'\n';


    
    return 0;
}



