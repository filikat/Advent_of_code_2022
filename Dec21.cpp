#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>


class Monkey{
    public:
    std::string monkey_name{};
    long long number{-1};   //NUMBER=-1 MEANS IT HASN'T BEEN SET YET
    bool original_number{false};
    std::string name_1{};
    std::string name_2{};
    char operation{};
    bool depend_on_humn{false};

    long long getNumber(std::map<std::string,Monkey>&);
    bool check_dependance(std::map<std::string,Monkey>&);
    void backtrack(std::map<std::string,Monkey>&,long long);
    void backtrack_root(std::map<std::string,Monkey>&);

};

long long Monkey::getNumber(std::map<std::string,Monkey>& monkeys){
    //RECURSIVELY COMPUTE THE NUMBER OF THE MONKEYS

    if (original_number) return number;

    if (!depend_on_humn && number!=-1) return number;

    long long num_1 = monkeys.find(name_1)->second.getNumber(monkeys);
    long long num_2 = monkeys.find(name_2)->second.getNumber(monkeys);

    switch (operation){
        case '+': number = num_1 + num_2;break;
        case '-': number = num_1 - num_2;break;
        case '*': number = num_1 * num_2;break;
        case '/': number = num_1 / num_2;break;
        default:
            std::cerr<<"Wrong operation"<<'\n';
            break;
    }

    return number;
}

bool Monkey::check_dependance(std::map<std::string,Monkey>& monkeys){
    //RECURSIVELY CHECK WHICH MONKEYS DEPEND ON THE VALUE OF HUMN

    if (number!=-1) depend_on_humn = false;

    else if (name_1=="humn" || name_2=="humn") depend_on_humn = true;

    else if(
        monkeys.find(name_1)->second.check_dependance(monkeys) ||
        monkeys.find(name_2)->second.check_dependance(monkeys)
    ){
        depend_on_humn = true;
    }

    return depend_on_humn;
}

void Monkey::backtrack_root(std::map<std::string,Monkey>& monkeys){
    //START BACK-TRACKING AT ROOT

    std::cout<<"\nStart at root\n";

    std::string monkey_to_find = name_2;
    std::string monkey_to_compute = name_1;

    if(! monkeys.find(name_1)->second.depend_on_humn){
        std::string monkey_to_find = name_1;
        std::string monkey_to_compute = name_2;
    }

    long long num_to_get = monkeys.find(monkey_to_find)->second.getNumber(monkeys);
    monkeys.find(monkey_to_compute)->second.backtrack(monkeys,num_to_get);

}

void Monkey::backtrack(std::map<std::string,Monkey>& monkeys,long long num_to_get){
    //RECURSIVELY BACK-TRACK UNTIL REACHING HUMN

    std::cout<<"Backtrack to "<<monkey_name<<'\n';

    if (monkey_name=="humn") std::cout<<'\n'<<num_to_get<<'\n';

    else{

        bool dependance_1 = monkeys.find(name_1)->second.depend_on_humn;

        if (dependance_1){

            long long num = monkeys.find(name_2)->second.getNumber(monkeys);
            switch (operation){
                case '+':
                    monkeys.find(name_1)->second.backtrack(monkeys,num_to_get-num);
                    break;
                case '-':
                    monkeys.find(name_1)->second.backtrack(monkeys,num_to_get+num);
                    break;
                case '*':
                    monkeys.find(name_1)->second.backtrack(monkeys,num_to_get/num);
                    break;
                case '/':
                    monkeys.find(name_1)->second.backtrack(monkeys,num_to_get*num);
                    break;
            }

        }else{

            long long num = monkeys.find(name_1)->second.getNumber(monkeys);
            switch (operation){
                case '+':
                    monkeys.find(name_2)->second.backtrack(monkeys,num_to_get-num);
                    break;
                case '-':
                    monkeys.find(name_2)->second.backtrack(monkeys,num-num_to_get);
                    break;
                case '*':
                    monkeys.find(name_2)->second.backtrack(monkeys,num_to_get/num);
                    break;
                case '/':
                    monkeys.find(name_2)->second.backtrack(monkeys,num/num_to_get);
                    break;
            }

        }

    }

}

std::map<std::string,Monkey> read_data(std::ifstream& file){
    std::string str{};
    
    std::map<std::string,Monkey> monkeys{};

    std::string name_1{};
    std::string name_2{};
    char c{};
    char operation{};
    std::string s{};

    while (getline(file,str)){
        std::stringstream ss{str};
        std::string name{};

        for (int i=0;i<4;++i){
            ss>>c;
            name.push_back(c);
        }

        Monkey monkey{};
        monkey.monkey_name = name;

        ss>>c>>c;
        ss.seekg(-1,std::ios_base::cur);
        if (c-'0'<10){
            ss>>s;

            monkey.number = stoi(s);
            monkey.original_number = true;

        }else{
            ss>>name_1>>operation>>name_2;

            monkey.name_1 = name_1;
            monkey.name_2 = name_2;
            monkey.operation = operation;
        }
        monkeys.insert({name,monkey});
    }

    return monkeys;
}

int main(){

    bool part_one{false};

    std::ifstream file("data21.txt");
    
    std::map<std::string,Monkey> monkeys{read_data(file)};

    if (part_one){

        std::cout<<monkeys.find("root")->second.getNumber(monkeys)<<'\n';

    }else{

        monkeys.find("root")->second.check_dependance(monkeys);
        monkeys.find("humn")->second.depend_on_humn = true;
        monkeys.find("root")->second.backtrack_root(monkeys);

    }


    return 0;
}



