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
#include "DrzewoCzerwonoCzarne.h"

int main() {
    auto sProgram = std::chrono::high_resolution_clock::now();
    std::vector<int> progi = { 10000, 100000, 500000, 1000000 };

    //ETAP 1: Wczytywanie wszystkich tytu³ów
    std::unordered_map<std::string, std::string> slownikTytulow;
    std::ifstream fileBasics("basics.tsv");

    std::cout << "Etap 1: Budowanie pelnego slownika tytulow...\n";
    auto sBasics = std::chrono::high_resolution_clock::now();

    if (fileBasics.is_open()) {
        std::string line;
        std::getline(fileBasics, line); // Pominiecie naglowka
        while (std::getline(fileBasics, line)) {
            std::stringstream ss(line);
            std::string tconst, type, pTitle;
            std::getline(ss, tconst, '\t');
            std::getline(ss, type, '\t');
            std::getline(ss, pTitle, '\t');
            slownikTytulow[tconst] = pTitle;
        }
        fileBasics.close();
    }
    auto eBasics = std::chrono::high_resolution_clock::now();
    std::cout << "Wczytano slownik. Czas: " << std::chrono::duration<double>(eBasics - sBasics).count() << " s\n";
    std::cout << "Liczba tytulow w slowniku tytulow: " << slownikTytulow.size() << "\n\n";

    //ETAP 2: Wczytywanie wszystkich ocen (Punkt 1 i 2 zadania)
    std::vector<MovieRating> wszystkieFilmy;
    // Wstêpna rezerwacja pamiêci
    wszystkieFilmy.reserve(1300000);

    std::ifstream fileRatings("ratings.tsv");
    if (!fileRatings.is_open()) {
        std::cerr << "Blad: Brak pliku ratings.tsv!\n";
        return 1;
    }

    std::cout << "Etap 2: Wczytywanie danych z pliku ratings.tsv...\n";
    std::string line;
    std::getline(fileRatings, line); // Naglowek

    while (std::getline(fileRatings, line)) {
        std::stringstream ss(line);
        std::string tconst, ratingStr;
        std::getline(ss, tconst, '\t');
        std::getline(ss, ratingStr, '\t');

        //Filtrowanie pustych wpisow
        if (!ratingStr.empty() && ratingStr != "\\N") {
            try {
                wszystkieFilmy.push_back({ tconst, std::stod(ratingStr) });
            }
            catch (...) { continue; }
        }
    }
    fileRatings.close();
    std::cout << "Zaladowano lacznie: " << wszystkieFilmy.size() << " rekordow.\n\n";

    //ETAP 3: Analiza efektywnosci (Punkt 3 i 4 zadania)
    std::cout << std::fixed << std::setprecision(5);

    int maxData = wszystkieFilmy.size();
    progi.push_back(maxData);

    for (int N : progi) {

        std::cout << ">>> ANALIZA DLA N = " << N << " <<<\n";

        // Kopiowanie danych do tablic roboczych dla kazdego algorytmu
        MovieRating* tab1 = new MovieRating[N];
        MovieRating* tab2 = new MovieRating[N];
        MovieRating* tab3 = new MovieRating[N];
        MovieRating* pom = new MovieRating[N]; // Dla MergeSort

        for (int i = 0; i < N; ++i) {
            tab1[i] = tab2[i] = tab3[i] = wszystkieFilmy[i];
        }


        // 1. Quicksort
        auto s1 = std::chrono::high_resolution_clock::now();
        SortowanieSzybkie::Sortuj_szybko(tab1, N, 0, N - 1);
        auto e1 = std::chrono::high_resolution_clock::now();
        std::cout << " [QS] Czas: " << std::chrono::duration<double>(e1 - s1).count() << " s\n";


        // 2. MergeSort
        auto s2 = std::chrono::high_resolution_clock::now();
        SortowaniePrzezScalanie::Sortuj(tab2, pom, N, 0, N - 1);
        auto e2 = std::chrono::high_resolution_clock::now();
        std::cout << " [MS] Czas: " << std::chrono::duration<double>(e2 - s2).count() << " s\n";

        // 3. BucketSort
        auto s3 = std::chrono::high_resolution_clock::now();
        SortowanieKubelkowe::Sortuj(tab3, N);
        auto e3 = std::chrono::high_resolution_clock::now();
        std::cout << " [BS] Czas: " << std::chrono::duration<double>(e3 - s3).count() << " s\n";

        // Punkt 4: Statystyki
        double suma = 0;
        for (int i = 0; i < N; ++i) suma += tab3[i].rating;
        double srednia = suma / N;
        double mediana = (N % 2 == 0) ? (tab3[N / 2 - 1].rating + tab3[N / 2].rating) / 2.0 : tab3[N / 2].rating;

        std::cout << " -> Srednia: " << std::setprecision(2) << srednia
            << " | Mediana: " << mediana << "\n\n";

        delete[] tab1; delete[] tab2; delete[] tab3; delete[] pom;
    }

    // ETAP 4: Wykorzystanie w³asnej implementacji zwrównowa¿onego drzewa binarnego w postaci
    //drzewa czerwono - czarnego
    std::cout << "Etap 4: Budowanie drzewa binarnego dla wszystkich danych (" << maxData << ") i zapis...\n";
    DrzewoCzerwonoCzarne drzewo; // Zastêpuje std::multimap<double, std::string>

    auto s4 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < wszystkieFilmy.size(); ++i) {
        if (slownikTytulow.count(wszystkieFilmy[i].tconst)) {
            std::string tytul = slownikTytulow[wszystkieFilmy[i].tconst];
            drzewo.insert({ wszystkieFilmy[i].rating, tytul });
        }
    }
    auto e4 = std::chrono::high_resolution_clock::now();
    std::cout << " Tworzenie zrownowazonego drzewa binarnego " << std::chrono::duration<double>(e4 - s4).count() << " s\n";

    std::ofstream wyjscie("posortowane_filmy.txt");
    if (wyjscie.is_open()) {
        for (const auto& w : drzewo) {
            wyjscie << std::fixed << std::setprecision(1) << w.first << "\t" << w.second << "\n";
        }
        wyjscie.close();
        std::cout << "Plik wynikowy zostal wygenerowany.\n";
    }
    auto eProgram = std::chrono::high_resolution_clock::now();
    std::cout << "Calkowity czas dzialania programu: " << std::chrono::duration<double>(eProgram - sProgram).count() << " s\n";

    return 0;

}