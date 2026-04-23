#pragma once
#include <string>

enum Kolor { CZERWONY, CZARNY };

struct Wezel {
    std::string tconst; // Klucz: ID filmu
    std::string tytul;  // Wartoœæ: Tytu³ filmu
    Kolor kolor;
    Wezel* lewy, * prawy, * rodzic;

    Wezel(std::string id, std::string t);
};

class DrzewoCzerwonoCzarne {
private:
    Wezel* korzen;
    Wezel* NIL;

    void rotacjaLewo(Wezel* x);
    void rotacjaPrawo(Wezel* x);
    void naprawWstawianie(Wezel* k);
    void usunDrzewo(Wezel* w);

public:
    DrzewoCzerwonoCzarne();
    ~DrzewoCzerwonoCzarne();

    void insert(std::string id, std::string tytul);
    std::string szukaj(std::string id); // Metoda do pobierania tytu³u po ID
};