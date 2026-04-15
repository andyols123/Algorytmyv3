#pragma once
#include "MovieRating.h"

class SortowaniePrzezScalanie {
public:
    static MovieRating* Sortuj(MovieRating* d, MovieRating* p, int rozmiar, int ip, int ik);
private:
    static MovieRating* Scalaj(MovieRating* d, MovieRating* p, int rozmiar, int ip, int is, int ik);
};