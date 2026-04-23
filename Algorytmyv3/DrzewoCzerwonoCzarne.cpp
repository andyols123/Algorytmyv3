#include "DrzewoCzerwonoCzarne.h"

Wezel::Wezel(std::string id, std::string t)
    : tconst(id), tytul(t), kolor(CZERWONY), lewy(nullptr), prawy(nullptr), rodzic(nullptr) {
}

DrzewoCzerwonoCzarne::DrzewoCzerwonoCzarne() {
    NIL = new Wezel("", "");
    NIL->kolor = CZARNY;
    NIL->lewy = NIL->prawy = NIL;
    korzen = NIL;
}

DrzewoCzerwonoCzarne::~DrzewoCzerwonoCzarne() {
    usunDrzewo(korzen);
    delete NIL;
}

void DrzewoCzerwonoCzarne::usunDrzewo(Wezel* w) {
    if (w != NIL && w != nullptr) {
        usunDrzewo(w->lewy);
        usunDrzewo(w->prawy);
        delete w;
    }
}

void DrzewoCzerwonoCzarne::insert(std::string id, std::string tytul) {
    Wezel* wezel = new Wezel(id, tytul);
    wezel->lewy = wezel->prawy = NIL;
    Wezel* r_temp = nullptr;
    Wezel* biezacy = korzen;

    while (biezacy != NIL) {
        r_temp = biezacy;
        if (wezel->tconst < biezacy->tconst) biezacy = biezacy->lewy;
        else biezacy = biezacy->prawy;
    }
    wezel->rodzic = r_temp;
    if (r_temp == nullptr) korzen = wezel;
    else if (wezel->tconst < r_temp->tconst) r_temp->lewy = wezel;
    else r_temp->prawy = wezel;

    if (wezel->rodzic == nullptr) { wezel->kolor = CZARNY; return; }
    if (wezel->rodzic->rodzic == nullptr) return;
    naprawWstawianie(wezel);
}

std::string DrzewoCzerwonoCzarne::szukaj(std::string id) {
    Wezel* b = korzen;
    while (b != NIL) {
        if (id == b->tconst) return b->tytul;
        if (id < b->tconst) b = b->lewy;
        else b = b->prawy;
    }
    return "";
}

void DrzewoCzerwonoCzarne::rotacjaLewo(Wezel* x) {
    Wezel* y = x->prawy;
    x->prawy = y->lewy;
    if (y->lewy != NIL) y->lewy->rodzic = x;
    y->rodzic = x->rodzic;
    if (x->rodzic == nullptr) korzen = y;
    else if (x == x->rodzic->lewy) x->rodzic->lewy = y;
    else x->rodzic->prawy = y;
    y->lewy = x;
    x->rodzic = y;
}

void DrzewoCzerwonoCzarne::rotacjaPrawo(Wezel* x) {
    Wezel* y = x->lewy;
    x->lewy = y->prawy;
    if (y->prawy != NIL) y->prawy->rodzic = x;
    y->rodzic = x->rodzic;
    if (x->rodzic == nullptr) korzen = y;
    else if (x == x->rodzic->prawy) x->rodzic->prawy = y;
    else x->rodzic->lewy = y;
    y->prawy = x;
    x->rodzic = y;
}

void DrzewoCzerwonoCzarne::naprawWstawianie(Wezel* k) {
    while (k->rodzic->kolor == CZERWONY) {
        if (k->rodzic == k->rodzic->rodzic->lewy) {
            Wezel* u = k->rodzic->rodzic->prawy;
            if (u->kolor == CZERWONY) {
                u->kolor = CZARNY; k->rodzic->kolor = CZARNY;
                k->rodzic->rodzic->kolor = CZERWONY; k = k->rodzic->rodzic;
            }
            else {
                if (k == k->rodzic->prawy) { k = k->rodzic; rotacjaLewo(k); }
                k->rodzic->kolor = CZARNY; k->rodzic->rodzic->kolor = CZERWONY;
                rotacjaPrawo(k->rodzic->rodzic);
            }
        }
        else {
            Wezel* u = k->rodzic->rodzic->lewy;
            if (u->kolor == CZERWONY) {
                u->kolor = CZARNY; k->rodzic->kolor = CZARNY;
                k->rodzic->rodzic->kolor = CZERWONY; k = k->rodzic->rodzic;
            }
            else {
                if (k == k->rodzic->lewy) { k = k->rodzic; rotacjaPrawo(k); }
                k->rodzic->kolor = CZARNY; k->rodzic->rodzic->kolor = CZERWONY;
                rotacjaLewo(k->rodzic->rodzic);
            }
        }
        if (k == korzen) break;
    }
    korzen->kolor = CZARNY;
}