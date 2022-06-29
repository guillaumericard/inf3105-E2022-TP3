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
#include "pile.h"

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
    // Constructeurs et classes
    ArbreAVL();
    ~ArbreAVL();
    ArbreAVL(const ArbreAVL &);
    class Iterateur;
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T &element) const;

    // Fonctions de modifications
    bool vide() const;              // ok
    bool contient(const T &) const; // ok
    void inserer(const T &);        // ok
    void vider();                   // ok

    // Utilitaires
    void afficher(const Noeud *racine);
    void afficher();
    int hauteur() const;
    int hauteur(const Noeud *n) const;
    int taille() const;
    int taille(const Noeud *n) const;

    // Nouvelles fonctionnalités du TP3:
    ArbreAVL<T> &operator=(const ArbreAVL<T> &autre); //ok
    bool operator==(const ArbreAVL<T> &autre) const; //ok

    friend ArbreAVL<T> unionn<T>(const ArbreAVL<T> &a, const ArbreAVL<T> &b); //ok
    friend ArbreAVL<T> inter<T>(const ArbreAVL<T> &a, const ArbreAVL<T> &b); // ok

private:
    class Noeud
    {
    public:
        Noeud(const T &element)
        {
            contenu = element;
            equilibre = 0;
            gauche = nullptr;
            droite = nullptr;
        };
        T contenu;
        Noeud *gauche;
        Noeud *droite;
        int equilibre;
    };

    Noeud *racine = nullptr;

    bool inserer(Noeud *&n, const T &element);
    void vider(Noeud *n);
    void rotationGaucheDroite(Noeud *&racineSousArbre);
    void rotationDroiteGauche(Noeud *&racineSousArbre);
    const T *recherche(Noeud *n, const T &element) const;

    // Nouvelle section publique pour Iterateur (mieux decoupé)
public:
    class Iterateur
    {
    public:
        Iterateur(const ArbreAVL &a);
        Iterateur(const Iterateur &a);
        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur &) const;
        bool operator!=(const Iterateur &) const;
        const T &operator*() const;
        Iterateur &operator++();   // pre-increment
        Iterateur operator++(int); // post-increment
        Iterateur &operator=(const Iterateur &);

    private:
        const ArbreAVL &arbre_associe;
        Noeud *courant;
        Pile<Noeud *> chemin;
        friend class ArbreAVL;
    };
};

//-----Fonctions pour ArbreAVL------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::ArbreAVL()
{
    racine = nullptr;
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider();
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T> &autre)
{
    if (!vide())
        vider();
    for (Iterateur i = autre.debut(); i; ++i)
    {
        inserer(*i);
    }
}

template <class T>
bool ArbreAVL<T>::contient(const T &element) const
{
    return recherche(racine, element) != nullptr;
}

template <class T>
const T *ArbreAVL<T>::recherche(Noeud *n, const T &element) const
{
    if (n == nullptr)
        return nullptr;
    if (element == n->contenu)
        return &(n->contenu);
    if (element < n->contenu)
        return recherche(n->gauche, element);
    else
        return recherche(n->droite, element);
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
            // std::cout << n->equilibre << std::endl;
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
    int neb = (nea > 0 ? nea : 0) + 1 + eb;

    temp->equilibre = neb;
    racineSousArbre->equilibre = nea;

    racineSousArbre->droite = temp->gauche;
    temp->gauche = racineSousArbre;
    racineSousArbre = temp;
}

template <class T>
bool ArbreAVL<T>::vide() const
{
    return racine == nullptr;
}

template <class T>
void ArbreAVL<T>::vider()
{
    vider(racine);
    racine = nullptr;
}

template <class T>
void ArbreAVL<T>::vider(Noeud *n)
{
    if (n == nullptr)
    {
        return;
    }
    vider(n->gauche);
    vider(n->droite);
    delete n;
}

template <class T>
ArbreAVL<T> &ArbreAVL<T>::operator=(const ArbreAVL &autre)
{
    if (!vide())
        vider();
    for (Iterateur i = autre.debut(); i; ++i)
    {
        inserer(*i);
    }
    return *this;
}

template <class T>
bool ArbreAVL<T>::operator==(const ArbreAVL<T> &autre) const
{
    if (vide() && autre.vide())
        return true;
    if (taille() != autre.taille())
        return false;
    Iterateur i = debut();
    Iterateur i_autre = autre.debut();
    while (i)
    {
        if (*i != *i_autre)
            return false;
        ++i;
        ++i_autre;
    }
    return true;
}

template <class T>
ArbreAVL<T> unionn(const ArbreAVL<T> &a, const ArbreAVL<T> &b)
{
    ArbreAVL<T> resultat;
    
    //union d'un ensemble avec ensemble vide = ensemble
    if(a.vide()) return b;
    if(b.vide())  return a;

    typename ArbreAVL<T>::Iterateur i = a.debut();
    typename ArbreAVL<T>::Iterateur y = b.debut();
    while (i || y)
    {
        if (i)
        {
            resultat.inserer(*i);
            ++i;
        }
        if (y)
        {
            resultat.inserer(*y);
            ++y;
        }
    }
    return resultat;
}

template <class T>
ArbreAVL<T> inter(const ArbreAVL<T> &a, const ArbreAVL<T> &b)
{
    ArbreAVL<T> resultat;

    // intersection d'un ensemble avec un ensemble vide = ensemble vide
    if(b.vide()) return b; 
    if(a.vide()) return a;

    if (a.taille() < b.taille())
    {
        typename ArbreAVL<T>::Iterateur i = a.debut();
        while (i)
        {
            if (b.contient(*i))
            {
                resultat.inserer(*i);
            }
            ++i;
        }
    }
    else
    {
        typename ArbreAVL<T>::Iterateur i = b.debut();
        while (i)
        {
            if (a.contient(*i))
            {
                resultat.inserer(*i);
            }
            ++i;
        }
    }
    return resultat;
}

//-----Fonctions pour Iterateur------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL &a)
    : arbre_associe(a), courant(nullptr)
{
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur i(*this);
    i.courant = racine;
    if (i.courant != nullptr)
    {
        while (i.courant->gauche != nullptr)
        {
            i.chemin.empiler(i.courant);
            i.courant = i.courant->gauche;
        }
    }
    return i;
}

template <class T>
typename ArbreAVL<T>::Iterateur &ArbreAVL<T>::Iterateur::operator++()
{
    assert(courant);
    Noeud *suivant = courant->droite;
    while (suivant)
    {
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if (!chemin.vide())
    {
        courant = chemin.depiler();
    }
    else
    {
        courant = nullptr;
    }
    return *this;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant != nullptr;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const
{
    return courant == nullptr;
}

template <class T>
const T &ArbreAVL<T>::Iterateur::operator*() const
{
    return courant->contenu;
}

//----- Fonctions utilitaires non demandees dans le cadre du TP
template <class T>
int ArbreAVL<T>::taille() const
{
    return taille(racine);
}

template <class T>
int ArbreAVL<T>::taille(const Noeud *n) const
{
    if (n == nullptr)
        return 0;
    return 1 + taille(n->gauche) + taille(n->droite);
}

template <class T>
int ArbreAVL<T>::hauteur() const
{
    return hauteur(racine);
}

template <class T>
int ArbreAVL<T>::hauteur(const Noeud *n) const
{
    if (n == nullptr)
        return 0;
    return std::max(hauteur(n->droite), hauteur(n->gauche)) + 1;
}

template <class T>
void ArbreAVL<T>::afficher()
{
    afficher(racine);
    std::cout << std::endl;
}

template <class T>
void ArbreAVL<T>::afficher(const Noeud *n)
{
    for (Iterateur i = debut(); i; ++i)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

#endif
