#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <map>
#include <unordered_map>

#include "MovieRating.h"
#include "SortowanieSzybkie.h"
#include "SortowaniePrzezScalanie.h"
#include "SortowanieKubelkowe.h"

int main() {
    const int MAX_LIMIT = 30000;
    std::vector<int> progi = { 1000, 5000, 10000, 20000, 30000 };

    // 1. Budowanie slownika tytulow (basics.tsv)
    //unordered_map<..> slownikTytulow to hashmap - struktura dzia³aj¹ca z wydajnoœci¹ O(1) 
    std::unordered_map<std::string, std::string> slownikTytulow;
    std::ifstream fileBasics("basics.tsv");

    std::cout << "Etap 1: Budowanie slownika tytulow...\n";
    auto startBasics = std::chrono::high_resolution_clock::now();

    if (fileBasics.is_open()) {
        std::string line;
        std::getline(fileBasics, line); // Naglowek
        while (std::getline(fileBasics, line) && slownikTytulow.size() < MAX_LIMIT * 4) {
            std::stringstream ss(line);
            std::string tconst, type, pTitle;
            std::getline(ss, tconst, '\t');
            std::getline(ss, type, '\t');
            std::getline(ss, pTitle, '\t');
            slownikTytulow[tconst] = pTitle;
        }
        fileBasics.close();
    }

    auto endBasics = std::chrono::high_resolution_clock::now();
    std::cout << "Czas budowania struktury tytulow: "
        << std::chrono::duration<double>(endBasics - startBasics).count() << " s\n\n";

    // 2. Wczytywanie ocen (ratings.tsv)
    std::vector<MovieRating> wszystkieFilmy;
    std::ifstream fileRatings("ratings.tsv");
    if (!fileRatings.is_open()) {
        std::cerr << "Blad: Brak pliku ratings.tsv!\n";
        return 1;
    }

    std::cout << "Etap 2: Wczytywanie " << MAX_LIMIT << " ocen...\n";
    std::string line;
    std::getline(fileRatings, line);
    while (std::getline(fileRatings, line) && (int)wszystkieFilmy.size() < MAX_LIMIT) {
        std::stringstream ss(line);
        std::string tconst, ratingStr;
        std::getline(ss, tconst, '\t');
        std::getline(ss, ratingStr, '\t');

        if (!ratingStr.empty() && ratingStr != "\\N") {
            //std::stod konwertuje string na double
            wszystkieFilmy.push_back({ tconst, std::stod(ratingStr) });
        }
    }
    fileRatings.close();
    std::cout << "Wczytano " << wszystkieFilmy.size() << " rekordow.\n\n";

    // 3. Analiza efektywnosci dla roznych progow (Zadanie 3 i 4)
    std::cout << std::fixed << std::setprecision(5);

    for (int N : progi) {
        if (N > (int)wszystkieFilmy.size()) continue;

        std::cout << "=== ANALIZA DLA N = " << N << " ===\n";

        // --- Quicksort ---
        MovieRating* tab1 = new MovieRating[N];
        for (int i = 0; i < N; ++i) tab1[i] = wszystkieFilmy[i];
        auto s1 = std::chrono::high_resolution_clock::now();
        SortowanieSzybkie::Sortuj_szybko(tab1, N, 0, N - 1);
        auto e1 = std::chrono::high_resolution_clock::now();
        std::cout << " [QS] Czas: " << std::chrono::duration<double>(e1 - s1).count() << " s\n";

        // --- MergeSort ---
        MovieRating* tab2 = new MovieRating[N];
        MovieRating* pom = new MovieRating[N];
        for (int i = 0; i < N; ++i) tab2[i] = wszystkieFilmy[i];
        auto s2 = std::chrono::high_resolution_clock::now();
        SortowaniePrzezScalanie::Sortuj(tab2, pom, N, 0, N - 1);
        auto e2 = std::chrono::high_resolution_clock::now();
        std::cout << " [MS] Czas: " << std::chrono::duration<double>(e2 - s2).count() << " s\n";

        // --- BucketSort ---
        MovieRating* tab3 = new MovieRating[N];
        for (int i = 0; i < N; ++i) tab3[i] = wszystkieFilmy[i];
        auto s3 = std::chrono::high_resolution_clock::now();
        SortowanieKubelkowe::Sortuj(tab3, N);
        auto e3 = std::chrono::high_resolution_clock::now();
        std::cout << " [BS] Czas: " << std::chrono::duration<double>(e3 - s3).count() << " s\n";

        // Statystyki (Zadanie 4)
        double suma = 0;
        for (int i = 0; i < N; ++i) suma += tab3[i].rating;
        double srednia = suma / N;
        double mediana = (N % 2 == 0) ? (tab3[N / 2 - 1].rating + tab3[N / 2].rating) / 2.0 : tab3[N / 2].rating;

        std::cout << " -> Srednia: " << std::setprecision(2) << srednia
            << " | Mediana: " << mediana << "\n\n";

        delete[] tab1; delete[] tab2; delete[] pom; delete[] tab3;
    }

    // 4. Zapis do pliku (Zadanie 5) (multimap - drzewo czerwono czarne )
    std::cout << "Etap 3: Budowanie drzewa binarnego dla zestawu " << MAX_LIMIT << " i zapis...\n";
    std::multimap<double, std::string> drzewo;

    for (int i = 0; i < MAX_LIMIT; ++i) {
        std::string tytul = (slownikTytulow.count(wszystkieFilmy[i].tconst))
            ? slownikTytulow[wszystkieFilmy[i].tconst]
            : wszystkieFilmy[i].tconst;
        drzewo.insert({ wszystkieFilmy[i].rating, tytul });
    }

    std::ofstream wyjscie("posortowane_filmy.txt");
    if (wyjscie.is_open()) {
        for (const auto& w : drzewo) {
            wyjscie << std::fixed << std::setprecision(1) << w.first << "\t" << w.second << "\n";
        }
        wyjscie.close();
        std::cout << "Plik posortowane_filmy.txt zostal wygenerowany pomyslnie.\n";
    }

    return 0;
}