#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


long long convert(char c){
    switch (c){
        case '2': return 2;
        case '1': return 1;
        case '0': return 0;
        case '-': return -1;
        case '=': return -2;
        default:
            std::cerr<<"Error in conversion of char\n";
            return 0;
    }
}

char convert(long long i){
    switch (i){
        case 2: return '2';
        case 1: return '1';
        case 0: return '0';
        case -1: return '-';
        case -2: return '=';
        default:
            std::cerr<<"Error in conversion of int\n";
            return 0;
    }
}

long long mod5(long long x){
    long long result = (x%5 + 5) % 5;
    if (result==4) return -1;
    if (result==3) return -2;
    return result;
}

std::pair<long long,long long> sum_char(long long num1,long long num2,long long carry){
    //RETURNS RESULT AND CARRY OF SUMMING NUM1 AND NUM2 AND CARRY
    long long sum = num1 + num2 + carry;

    long long res = mod5(sum);
    long long res_carry{};

    if (std::abs(sum)<=2) res_carry = 0;
    else if (sum>0){
        res_carry = (sum-3)/5 + 1;
    }else{
        res_carry = - ((-sum-3)/5 + 1);
    }
    return {res,res_carry};
}

long long str2num(const std::string& str){
    long long factor = 1;
    long long number = 0;
    for (auto it=str.rbegin();it!=str.rend();++it){
        number += convert(*it) * factor;
        factor *= 5;
    }
    return number;
}

int main(){

    std::ifstream file("data25.txt");

    std::string str1{};
    std::string str2{};
    long long carry{};

    getline(file,str1);

    while (getline(file,str2)){
        
        //GET STRINGS OF SAME LENGTH
        while (str1.size()<str2.size()){
            str1.insert(str1.begin(),'0');
        }
        while (str2.size()<str1.size()){
            str2.insert(str2.begin(),'0');
        }
        
        std::string result{};
        for (long long i=str1.size()-1;i>=0;--i){
            long long num1{convert(str1[i])};
            long long num2{convert(str2[i])};
            std::pair<long long,long long> sum{sum_char(num1,num2,carry)};
            carry = sum.second;
            result.insert(result.begin(),convert(sum.first));
        }

        if (carry>0){
            result.insert(result.begin(),convert(carry));
        }

        str1 = result;
    }

    std::cout<<str1<<'\n';
    std::cout<<str2num(str1)<<'\n';
   

    return 0;
}



