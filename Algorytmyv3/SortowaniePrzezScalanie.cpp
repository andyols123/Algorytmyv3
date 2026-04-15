#include "SortowaniePrzezScalanie.h"

MovieRating* SortowaniePrzezScalanie::Sortuj(MovieRating* d, MovieRating* p, int rozmiar, int ip, int ik) {
    int is = (ip + ik + 1) / 2;
    if ((is - ip) > 1) { Sortuj(d, p, rozmiar, ip, is - 1); }
    if ((ik - is) > 0) { Sortuj(d, p, rozmiar, is, ik); }
    Scalaj(d, p, rozmiar, ip, is, ik);
    return d;
}

MovieRating* SortowaniePrzezScalanie::Scalaj(MovieRating* d, MovieRating* p, int rozmiar, int ip, int is, int ik) {
    int i1 = ip;
    int i2 = is;

    for (int i = ip; i <= ik; i++) {
        if ((i1 == is) || ((i2 <= ik) && (d[i1].rating > d[i2].rating))) {
            p[i] = d[i2];
            i2++;
        }
        else {
            p[i] = d[i1];
            i1++;
        }
    }
    for (int i = ip; i <= ik; i++) { d[i] = p[i]; }
    return d;
}