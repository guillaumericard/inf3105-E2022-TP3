#include <iostream>
#include "arbreavl.h"

int main(int argc, char **argv){
        ArbreAVL<int> a;
        a.inserer(1);
        a.inserer(2);
        a.inserer(3);
        ArbreAVL<int> b = a;
        b.inserer(4);
        ArbreAVL<int> c = inter(a,b);
        std::cout << (c == a ? "TRUE" : "FALSE") << std::endl;
        
}
