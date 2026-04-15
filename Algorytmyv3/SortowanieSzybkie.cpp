#include "SortowanieSzybkie.h"
#include <algorithm>

MovieRating* SortowanieSzybkie::Sortuj_szybko(MovieRating* d, int rozmiar, int lewy, int prawy) {
    MovieRating piwot;
    int i = (lewy + prawy) / 2;
    int j = lewy;

    piwot = d[i];
    d[i] = d[prawy];

    for (int k = lewy; k <= prawy - 1; k++) {
        if (d[k].rating < piwot.rating) {
            std::swap(d[k], d[j]);
            j++;
        }
    }

    d[prawy] = d[j];
    d[j] = piwot;

    if (lewy < (j - 1)) { Sortuj_szybko(d, rozmiar, lewy, j - 1); }
    if ((j + 1) < prawy) { Sortuj_szybko(d, rozmiar, j + 1, prawy); }

    return d;
}