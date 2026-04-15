#pragma once
#include "MovieRating.h"

class SortowanieSzybkie {
public:
    static MovieRating* Sortuj_szybko(MovieRating* d, int rozmiar, int lewy, int prawy);
};