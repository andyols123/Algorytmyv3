#pragma once
#include <string>
#include <utility>

enum Kolor { CZERWONY, CZARNY };

struct Wezel {
    double ocena;
    std::string tytul;
    Kolor kolor;
    Wezel* lewy, * prawy, * rodzic;

    Wezel(double o, std::string t);
};

class DrzewoCzerwonoCzarne {
private:
    Wezel* korzen;
    Wezel* NIL; // Wartownik zastêpuj¹cy nullptr

    void rotacjaLewo(Wezel* x);
    void rotacjaPrawo(Wezel* x);
    void naprawWstawianie(Wezel* k);
    void usunDrzewo(Wezel* w);

public:
    DrzewoCzerwonoCzarne();
    ~DrzewoCzerwonoCzarne();

    void insert(std::pair<double, std::string> dane);

    class Iterator {
        Wezel* biezacy;
        Wezel* nil_ref;
    public:
        Iterator(Wezel* start, Wezel* n) : biezacy(start), nil_ref(n) {}
        bool operator!=(const Iterator& inny) const { return biezacy != inny.biezacy; }
        std::pair<double, std::string> operator*() const { return { biezacy->ocena, biezacy->tytul }; }
        Iterator& operator++();
    };

    Iterator begin();
    Iterator end() { return Iterator(NIL, NIL); }
};