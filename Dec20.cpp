#include <iostream>
#include <fstream>
#include <string>
#include <vector>

constexpr long long key{811589153};

void shiftPositions(std::vector<int>& positions,size_t j){
    //PUT THE CURRENT NUMBER AT POSITION 0
    long long cur_pos = positions[j];
    for (size_t i=0;i<positions.size();++i){
        positions[i] -= cur_pos;
        while (positions[i]<0){
            positions[i] += positions.size();
        }
    }
}

void changePositions(std::vector<int>& positions,int number){
    //CHANGE THE POSITIONS BASED ON THE NUMBER
    for (size_t i=0;i<positions.size();++i){
        if (positions[i]==0) positions[i] = number;
        else if (positions[i]<=number) --positions[i];
    }
}

void mixing(const std::vector<int>& numbers,std::vector<int>& positions){
    for (size_t i=0;i<numbers.size();++i){
        shiftPositions(positions,i);
        changePositions(positions,numbers[i]);
    }
}


int main(){

    bool part_one{false};

    std::ifstream file("data20.txt");
    std::string str{};
    
    std::vector<long long> numbers{};
    std::vector<int> positions{};
    std::vector<int> reduced{};
    int index{};

    long long factor{1};
    if (!part_one) factor = key;

    //READ DATA
    while(getline(file,str)){
        positions.push_back(index);
        ++index;
        numbers.push_back(factor*stoi(str));
    }

    //REDUCE THE NUMBER BETWEEN 0 AND THE SIZE OF THE CIRCLE
    for (size_t i=0;i<numbers.size();++i){

        long long number = numbers[i];
        if (number>0){
            number = number % static_cast<long long>(positions.size()-1);
        }

        if (number<0){
            number = number % static_cast<long long>(positions.size()-1);
            if (number<0) number += (positions.size()-1);
        }

        reduced.push_back(number);

    }

    int iterations{1};
    if (!part_one) iterations = 10; 

    //PERFORM ITERATIONS OF MIXING
    for (size_t i=0;i<iterations;++i){
        mixing(reduced,positions);
        std::cout<<"Finished "<<i+1<<'\n';
    }

    //FIND POSITION OF ZERO
    int pos0{};
    for (size_t i=0;i<numbers.size();++i){
        if (numbers[i] == 0){
            pos0 = positions[i];
            break;
        }
    }

    int pos1000 = (1000%numbers.size() + pos0) % numbers.size();
    int pos2000 = (2000%numbers.size() + pos0) % numbers.size();
    int pos3000 = (3000%numbers.size() + pos0) % numbers.size();

    long long num1000{};
    long long num2000{};
    long long num3000{};

    //FIND 1000,2000,3000TH NUMBER
    for (size_t i=0;i<numbers.size();++i){
        if (positions[i]==pos1000) num1000 = numbers[i];
        if (positions[i]==pos2000) num2000 = numbers[i];
        if (positions[i]==pos3000) num3000 = numbers[i];
    }

    std::cout<<num1000<<' '<<num2000<<' '<<num3000<<'\n';
    std::cout<<num1000+num2000+num3000<<'\n';
    
    return 0;
}



