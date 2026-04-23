#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

#include "MovieRating.h"
#include "SortowanieSzybkie.h"
#include "SortowaniePrzezScalanie.h"
#include "SortowanieKubelkowe.h"
#include "DrzewoCzerwonoCzarne.h"

int main() {
    auto sProgram = std::chrono::high_resolution_clock::now();

    // ETAP 1: Budowanie bazy tytulow w Drzewie (Zamiast Hashmapy)
    DrzewoCzerwonoCzarne bazaTytulow;
    std::ifstream fileBasics("basics.tsv");
    std::cout << "Etap 1: Budowanie bazy tytulow w drzewie...\n";

    auto sTreeBuild = std::chrono::high_resolution_clock::now();
    if (fileBasics.is_open()) {
        std::string line;
        std::getline(fileBasics, line); // Naglowek
        while (std::getline(fileBasics, line)) {
            std::stringstream ss(line);
            std::string tconst, type, pTitle;
            std::getline(ss, tconst, '\t');
            std::getline(ss, type, '\t');
            std::getline(ss, pTitle, '\t');
            if (!tconst.empty()) bazaTytulow.insert(tconst, pTitle);
        }
        fileBasics.close();
    }
    auto eTreeBuild = std::chrono::high_resolution_clock::now();
    std::cout << " Czas budowania drzewa: " << std::chrono::duration<double>(eTreeBuild - sTreeBuild).count() << " s\n";

    // ETAP 2: Wczytywanie ocen
    std::vector<MovieRating> wszystkieFilmy;
    wszystkieFilmy.reserve(1300000);
    std::ifstream fileRatings("ratings.tsv");
    std::cout << "Etap 2: Wczytywanie danych ratings.tsv...\n";

    if (fileRatings.is_open()) {
        std::string line;
        std::getline(fileRatings, line);
        while (std::getline(fileRatings, line)) {
            std::stringstream ss(line);
            std::string tconst, rStr;
            std::getline(ss, tconst, '\t');
            std::getline(ss, rStr, '\t');
            if (!rStr.empty() && rStr != "\\N") {
                wszystkieFilmy.push_back({ tconst, std::stod(rStr) });
            }
        }
        fileRatings.close();
    }

    // ETAP 3: Analiza porownawcza algorytmow dla roznych wielkosci danych
    std::vector<int> progi = { 10000, 100000, 500000, 1000000, (int)wszystkieFilmy.size() };

    for (int N : progi) {
        std::cout << "\n>>> ANALIZA DLA N = " << N << " <<<\n";
        MovieRating* tab1 = new MovieRating[N];
        MovieRating* tab2 = new MovieRating[N];
        MovieRating* tab3 = new MovieRating[N];
        //tablica pomocnicza dla mergestorta
        MovieRating* pom = new MovieRating[N];

        for (int i = 0; i < N; ++i) tab1[i] = tab2[i] = tab3[i] = wszystkieFilmy[i];

        // Quicksort
        auto sQS = std::chrono::high_resolution_clock::now();
        SortowanieSzybkie::Sortuj_szybko(tab1, N, 0, N - 1);
        auto eQS = std::chrono::high_resolution_clock::now();
        std::cout << " [Quicksort] Czas: " << std::chrono::duration<double>(eQS - sQS).count() << " s\n";

        // MergeSort
        auto sMS = std::chrono::high_resolution_clock::now();
        SortowaniePrzezScalanie::Sortuj(tab2, pom, N, 0, N - 1);
        auto eMS = std::chrono::high_resolution_clock::now();
        std::cout << " [MergeSort] Czas: " << std::chrono::duration<double>(eMS - sMS).count() << " s\n";

        // BucketSort
        auto sBS = std::chrono::high_resolution_clock::now();
        SortowanieKubelkowe::Sortuj(tab3, N);
        auto eBS = std::chrono::high_resolution_clock::now();
        std::cout << " [BucketSort] Czas: " << std::chrono::duration<double>(eBS - sBS).count() << " s\n";

        // Obliczanie statystyk (na posortowanej tablicy tab3)
        double suma = 0;
        for (int i = 0; i < N; ++i) suma += tab3[i].rating;
        double mediana = (N % 2 == 0) ? (tab3[N / 2 - 1].rating + tab3[N / 2].rating) / 2.0 : tab3[N / 2].rating;
        std::cout << " -> Srednia: " << (suma / N) << " | Mediana: " << mediana << "\n";

        // ETAP 4: Zapis i korzystanie z drzewa (tylko dla najwiekszego zbioru)
        if (N == (int)wszystkieFilmy.size()) {
            std::cout << "Etap 4: Mapowanie ID na tytuly za pomoca drzewa i zapis...\n";
            std::ofstream wyjscie("posortowane_filmy.txt");
            auto sLookup = std::chrono::high_resolution_clock::now();

            if (wyjscie.is_open()) {
                for (int i = 0; i < N; ++i) {
                    // wyci¹ganie tytu³u z drzewa dla kazdego posortowanego ratingu
                    std::string tytul = bazaTytulow.szukaj(tab3[i].tconst);
                    if (!tytul.empty()) {
                        wyjscie << std::fixed << std::setprecision(1) << tab3[i].rating << "\t" << tytul << "\n";
                    }
                }
                wyjscie.close();
            }
            auto eLookup = std::chrono::high_resolution_clock::now();
            std::cout << " Czas wyszukiwania w drzewie i zapisu: " << std::chrono::duration<double>(eLookup - sLookup).count() << " s\n";
        }

        delete[] tab1; delete[] tab2; delete[] tab3; delete[] pom;
    }

    auto eProgram = std::chrono::high_resolution_clock::now();
    std::cout << "\nCalkowity czas programu: " << std::chrono::duration<double>(eProgram - sProgram).count() << " s\n";
    return 0;
}