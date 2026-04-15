#include "SortowanieKubelkowe.h"
#include <vector>

MovieRating* SortowanieKubelkowe::Sortuj(MovieRating tablicaNieposortowana[], int rozmiar) {
    if (rozmiar <= 0) return tablicaNieposortowana;

    int zakres = 101; // 0.0, 0.1 ... 10.0
    std::vector<MovieRating>* kubelki = new std::vector<MovieRating>[zakres];

    for (int i = 0; i < rozmiar; i++) {
        int indeks = static_cast<int>(tablicaNieposortowana[i].rating * 10.0 + 0.1);
        if (indeks >= 0 && indeks < zakres) {
            kubelki[indeks].push_back(tablicaNieposortowana[i]);
        }
    }

    int aktualnyIndeks = 0;
    for (int i = 0; i < zakres; i++) {
        for (size_t j = 0; j < kubelki[i].size(); j++) {
            tablicaNieposortowana[aktualnyIndeks++] = kubelki[i][j];
        }
    }

    delete[] kubelki;
    return tablicaNieposortowana;
}