/* Squelette minimal pour classe générique ArbreAVL<T> du TP3.
 * TP3 -- Arbres AVL
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/tp3/
 *
 * Suggestions:
 *  - utilisez votre solution des Lab6 et Lab7;
 *  - ajoutez-y les fonctions demandées ci-dessous.
 *
 * Travail presente par Guillaume Ricard - RICG05089405
 */

#include <assert.h>
#if !defined(_ARBREAVL___H_)
#define _ARBREAVL___H_

// Déclare l'existance de la classe template ArbreAVL (pour déclarer unionn et inter).
template <class T>
class ArbreAVL;

// Déclare l'existance des fonctions unionn et inter (pour les déclarer friend dans ArbreAVL).
template <class T>
ArbreAVL<T> unionn(const ArbreAVL<T> &a, const ArbreAVL<T> &b);
template <class T>
ArbreAVL<T> inter(const ArbreAVL<T> &a, const ArbreAVL<T> &b); // intersection

template <class T>
class ArbreAVL
{
private:
class Noeud;
public:
    // Constructeurs
    ArbreAVL();
    ArbreAVL(const ArbreAVL &);

    // Fonctions de modifications
    bool vide() const;
    bool contient(const T &) const;
    void inserer(const T &);
    void vider();
    void vider(Noeud *n);

    //Utilitaires
    void afficher(const Noeud *racine);
    void afficher();
    int hauteur() const;
    int hauteur(const Noeud *n) const;
    int taille() const;
    int taille(const Noeud *n) const;

    // Nouvelles fonctionnalités du TP3:
    ArbreAVL<T> &operator=(const ArbreAVL<T> &autre);
    bool operator==(const ArbreAVL<T> &autre) const;

    friend ArbreAVL<T> unionn<T>(const ArbreAVL<T> &a, const ArbreAVL<T> &b);
    friend ArbreAVL<T> inter<T>(const ArbreAVL<T> &a, const ArbreAVL<T> &b); // intersection

private:
    class Noeud
    {
    public:
        Noeud(const T &element)
        {
            Noeud *n = new Noeud();
            contenu = element;
        };
        Noeud()
        {
        }
        T contenu;
        Noeud *gauche;
        Noeud *droite;
        int equilibre;
    };

    Noeud *racine;

    bool inserer(Noeud *&n, const T &element);
    void rotationGaucheDroite(Noeud *&racineSousArbre);
    void rotationDroiteGauche(Noeud *&racineSousArbre);
};

//-----------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::ArbreAVL()
{
    racine = nullptr;
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T> &autre)
{
    // À compléter.
}

template <class T>
bool ArbreAVL<T>::contient(const T &element) const
{
    // À compléter.
    return false;
}

template <class T>
void ArbreAVL<T>::inserer(const T &element)
{
    inserer(racine, element);
}

template <class T>
bool ArbreAVL<T>::inserer(Noeud *&n, const T &element)
{
    if (n == nullptr)
    {
        n = new Noeud(element);
        return true;
    }
    if (element < n->contenu)
    {
        if (inserer(n->gauche, element))
        {
            n->equilibre++;
            if (n->equilibre == 0)
            {
                return false;
            }
            if (n->equilibre == 1)
            {
                return true;
            }
            assert(n->equilibre == 2);
            if (n->gauche->equilibre == -1)
            {
                rotationDroiteGauche(n->gauche);
            }
            rotationGaucheDroite(n);
        }
        return false;
    }
    else if (n->contenu < element)
    {
        if (inserer(n->droite, element))
        {
            n->equilibre--;
            //std::cout << n->equilibre << std::endl;
            if (n->equilibre == 0)
            {
                return false;
            }
            if (n->equilibre == -1)
            {
                return true;
            }
            assert(n->equilibre == -2);
            if (n->droite->equilibre == 1)
            {
                rotationGaucheDroite(n->droite);
            }
            rotationDroiteGauche(n);
        }
        return false;
    }
    else
    {
        n->contenu = element;
        return false;
    }
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud *&racineSousArbre)
{
    Noeud *temp = racineSousArbre->gauche; 
    
    int ea = temp->equilibre; 
    int eb = racineSousArbre->equilibre; 

    int neb = -(ea > 0 ? ea : 0) - 1 + eb;
    int nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racineSousArbre->equilibre = neb;

    racineSousArbre->gauche = temp->droite;
    temp->droite = racineSousArbre;
    racineSousArbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud *&racineSousArbre)
{
    Noeud *temp = racineSousArbre->droite;

    int eb = temp->equilibre;
    int ea = racineSousArbre->equilibre;

    int nea = -(eb < 0 ? eb : 0) + 1 + ea;
    int neb = (nea> 0 ? nea : 0) + 1 + eb;

    temp->equilibre = neb;
    racineSousArbre->equilibre = nea;

    racineSousArbre->droite = temp->gauche;
    temp->gauche = racineSousArbre;
    racineSousArbre = temp;
}

template <class T>
bool ArbreAVL<T>::vide() const
{
    // À compléter : doit retourner vrai si et seulement si l'arbre est vide.
    return racine == nullptr;
}

template <class T>
void ArbreAVL<T>::vider()
{
    vider(racine);
    racine = nullptr;
}

template <class T>
void ArbreAVL<T>::vider(Noeud *n){
if(n == nullptr) return;
vider(n->gauche);
vider(n->droite);
}

template <class T>
ArbreAVL<T> &ArbreAVL<T>::operator=(const ArbreAVL &autre)
{
    // À compléter.
    return *this;
}

template <class T>
bool ArbreAVL<T>::operator==(const ArbreAVL<T> &autre) const
{
    // À compléter.
    return false;
}

template <class T>
ArbreAVL<T> unionn(const ArbreAVL<T> &a, const ArbreAVL<T> &b)
{
    ArbreAVL<T> resultat;
    return resultat;
}

template <class T>
ArbreAVL<T> inter(const ArbreAVL<T> &a, const ArbreAVL<T> &b)
{
    ArbreAVL<T> resultat;
    return resultat;
}

//----- Fonctions utilitaires non demandees dans le cadre du TP
template<class T>
int ArbreAVL<T>::taille() const{
  return taille(racine);
}

template<class T>
int ArbreAVL<T>::taille(const Noeud *n) const{
    if(n == nullptr) return 0;
    return 1+taille(n->gauche)+taille(n->droite);
}

template <class T>
int ArbreAVL<T>::hauteur() const{
    return hauteur(racine);
}

template<class T>
int ArbreAVL<T>::hauteur(const Noeud *n) const{
    if (n == nullptr) return 0;
    return std::max(hauteur(n->droite),hauteur(n->gauche)) + 1;
}

template<class T>
void ArbreAVL<T>::afficher(){
    afficher(racine);
    std::cout<<std::endl;
}

template <class T>
void ArbreAVL<T>::afficher(const Noeud *n){
    if(n == nullptr) return;
    afficher(n->gauche);
    std::cout<< n->contenu << " ";
    afficher(n->droite);
}

#endif
