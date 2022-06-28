#include <iostream>
#include "arbreavl.h"

int main(int argc, char **argv){
   ArbreAVL<int> a;
        a.inserer(1);
        a.inserer(3);
        a.inserer(4);
        a.inserer(6);
        a.inserer(8);
        a.inserer(2);
        a.inserer(24);
        std::cout << "HAUTEUR : " << a.hauteur() << std::endl;
        std::cout << "TAILLE : " << a.taille() << std::endl;
        a.afficher();
        
}
