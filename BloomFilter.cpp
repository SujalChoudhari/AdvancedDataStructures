// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>


int hash_func(int init_value,int bias, std::string value){
    int hash = init_value;
    for(char ch: value ) {
        hash = hash * bias  % 512 & ch ^ bias;
    }
    
    return hash % 512;
}

// prime nums 
#define H1(x) hash_func(1543,4007,x) 
 
#define H2(x) hash_func(1429,3989,x)

#define H3(x) hash_func(4339,7079,x)

int main() {
    std::vector<int> bitsArray(511,0); 
    std::cout << "Insert Strings: Seperate by space number and strings: 5 Sujal ...\n>>> ";
    int count = 0;
    std::cin >> count;
    
    // add to filter
    for(int i = 0; i < count; i++){
        std::string str;
        std::cin >> str;
        std::cout << str << std::endl;
        
        int h1 = H1(str);
        int h2 = H2(str);
        int h3 = H3(str);
        
        std:: cout << "Hash values: "<< h1 << " " << h2 << " " << h3 << " " << std::endl;
        
        bitsArray[h1] = 1;
        bitsArray[h2] = 1;
        bitsArray[h3] = 1;
    }
    
    // is present
    while(true){
        std::cout << "Check for a word if it is present: exit to return \n>>> " ;
        std::string srch;
        std::cin >> srch;
        
        if(srch == "exit") {
            return 0;
        }
        
        int sh1 = H1(srch);
        int sh2 = H2(srch);
        int sh3 = H3(srch);
        
        std:: cout << sh1 << " " << sh2 << " " << sh3 << " " << std::endl;
        
        if(bitsArray[sh1] && bitsArray[sh2]&& bitsArray[sh3]){
            std::cout << "Yes we have that one!" << std::endl;
        }else {
            std::cout << "Nope!" << std::endl;
        }
        
   }
    
    return 0;
}
