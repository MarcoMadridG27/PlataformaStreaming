#include "../include/MovieTree.h"
#include "../include/Utils.h"
#include <iostream>
#include <string>
#include <limits>

int main() {

    stopWords = Utils::loadStopWords("C:/Users/ASUS/CLionProjects/pruebas/stop_word.txt");

    MovieTree movieTree;

    try {
        movieTree.loadFromCSV("C:/Users/ASUS/CLionProjects/pruebas/data_corregida1.csv");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    int option;
    do {
        std::cout << "\n--- Menu ---\n";
        std::cout << "1. Buscar pelicula\n";
        std::cout << "2. Buscar por tag\n";
        std::cout << "3. Ver sinopsis\n";
        std::cout << "4. Marcar pelicula como 'Me gusta'\n";
        std::cout << "5. Ver 'Me gusta'\n";
        std::cout << "6. Agregar a 'Ver mas tarde'\n";
        std::cout << "7. Ver 'Ver mas tarde'\n";
        std::cout << "8. Salir\n";
        std::cout << "Selecciona: ";

        if (!(std::cin >> option)) {
            std::cerr << "Entrada invalida. Por favor, ingresa un numero.\n";
            std::cin.clear(); // Limpia el estado de error de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (option) {
            case 1: {
                std::string keyword;
                std::cout << "Ingresa una palabra clave: ";
                std::getline(std::cin, keyword);

                auto results = movieTree.searchWithRelevance(Utils::filterText(keyword));
                if (results.empty()) {
                    std::cout << "No se encontraron peliculas.\n";
                } else {
                    for (size_t i = 0; i < results.size() && i < 5; ++i) {
                        const auto& [id, relevance] = results[i];
                        const auto& movie = movieTree.getMovie(id);
                        std::cout << i + 1 << ". " << movie.title << " (ID: " << id << ", Relevancia: " << relevance << ")\n";
                    }
                }
                break;
            }
            case 2: {
                std::string tag;
                std::cout << "Ingresa el tag: ";
                std::getline(std::cin, tag);

                auto results = movieTree.searchByTag(tag);
                if (results.empty()) {
                    std::cout << "No se encontraron peliculas con ese tag.\n";
                } else {
                    for (size_t i = 0; i < results.size() && i < 5; ++i) {
                        const auto& [id, relevance] = results[i];
                        const auto& movie = movieTree.getMovie(id);
                        std::cout << i + 1 << ". " << movie.title << " (ID: " << id << ", Relevancia: " << relevance << ")\n";
                    }
                }
                break;
            }
            case 3: {
                std::string movieID;
                std::cout << "Ingresa el ID de la pelicula: ";
                std::getline(std::cin, movieID);

                try {
                    std::string synopsis = movieTree.getSynopsis(movieID);
                    std::cout << "\nSinopsis:\n" << synopsis << "\n";
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 4: {
                std::string movieID;
                std::cout << "Ingresa el ID de la pelicula: ";
                std::getline(std::cin, movieID);

                try {
                    movieTree.addToLiked(movieID);
                    std::cout << "Pelicula marcada como 'Me gusta'.\n";
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 5: {
                const auto& likedMovies = movieTree.getLikedMovies();
                if (likedMovies.empty()) {
                    std::cout << "No hay peliculas marcadas como 'Me gusta'.\n";
                } else {
                    std::cout << "\nPeliculas marcadas como 'Me gusta':\n";
                    for (const auto& movieID : likedMovies) {
                        try {
                            const auto& movie = movieTree.getMovie(movieID);
                            std::cout << "- " << movie.title << " (ID: " << movieID << ")\n";
                        } catch (const std::exception& e) {
                            std::cerr << "Error: " << e.what() << "\n";
                        }
                    }
                }
                break;
            }
            case 6: {
                std::string movieID;
                std::cout << "Ingresa el ID de la pelicula: ";
                std::getline(std::cin, movieID);

                try {
                    movieTree.addToWatchLater(movieID);
                    std::cout << "Pelicula agregada a 'Ver mas tarde'.\n";
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 7: {
                const auto& watchLater = movieTree.getWatchLaterMovies();
                if (watchLater.empty()) {
                    std::cout << "No hay peliculas en 'Ver mas tarde'.\n";
                } else {
                    std::cout << "\nPeliculas en 'Ver mas tarde':\n";
                    for (const auto& movieID : watchLater) {
                        try {
                            const auto& movie = movieTree.getMovie(movieID);
                            std::cout << "- " << movie.title << " (ID: " << movieID << ")\n";
                        } catch (const std::exception& e) {
                            std::cerr << "Error: " << e.what() << "\n";
                        }
                    }
                }
                break;
            }
            case 8:
                std::cout << "Saliendo...\n";
                break;
            default:
                std::cout << "Opcion invalida. Intenta de nuevo.\n";
        }
    } while (option != 8);

    return 0;
}
