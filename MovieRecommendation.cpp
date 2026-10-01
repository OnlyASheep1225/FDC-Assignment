/*
 * =====================================================================
 *   Title   : Movie Recommendation Assistant
 *   Group   : FCI4_G10
 *   Project : Netflix and the Evolution of Online Streaming
 *             (LDCW6123 - Fundamentals of Digital Competence Part 2)
 *
 *
 *   Purpose : Netflix's core innovation was personalised content
 *             discovery. This program simulates a simplified version
 *             of that feature: the user picks a genre and a mood, and
 *             the program recommends a matching title, then lets the
 *             user build a personal "watchlist" (My List) with full
 *             management features (view, remove, browse catalog,
 *             and save a copy to a text file).
 * =====================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream> // Required for file saving/exporting

using namespace std;

// ---------------------------------------------------------------------
// Data structure representing one movie in our catalog
// ---------------------------------------------------------------------
struct Movie {
    string title;
    string genre;       // Action, Comedy, Drama, Sci-Fi, Documentary
    string mood;        // Light, Intense, Thoughtful
    string description;
    double rating;      // out of 10
};

// ---------------------------------------------------------------------
// Function prototypes
// ---------------------------------------------------------------------
void displayWelcome();
int getMenuChoice(int minOption, int maxOption, const string& prompt);
string genreNumberToName(int genreChoice);
string moodNumberToName(int moodChoice);
vector<Movie> buildCatalog();
Movie recommendMovie(const vector<Movie>& catalog, const string& genre, const string& mood, bool& found);
void showWatchlist(vector<Movie>& watchlist);
void showCatalog(const vector<Movie>& catalog);
void saveWatchlistToFile(const vector<Movie>& watchlist);
void clearInputError();

// ---------------------------------------------------------------------
// main() - controls overall program flow with an expanded menu loop
// ---------------------------------------------------------------------
int main() {
    vector<Movie> catalog = buildCatalog();
    vector<Movie> watchlist; // user's personal "My List"

    displayWelcome();

    bool running = true;
    while (running) {
        cout << "\n===================== MAIN MENU =====================\n";
        cout << "1. Get a movie recommendation\n";
        cout << "2. View my watchlist\n";
        cout << "3. Browse full movie catalog\n";
        cout << "4. Save watchlist copy to text file\n";
        cout << "5. Exit\n";
        cout << "======================================================\n";

        int mainChoice = getMenuChoice(1, 5, "Enter your choice (1-5): ");

        switch (mainChoice) {
            case 1: {
                // ---- Step 1: Ask for genre preference ----
                cout << "\nSelect a genre:\n";
                cout << "1. Action\n2. Comedy\n3. Drama\n4. Sci-Fi\n5. Documentary\n";
                int genreChoice = getMenuChoice(1, 5, "Enter genre number (1-5): ");
                string genre = genreNumberToName(genreChoice);

                // ---- Step 2: Ask for mood preference ----
                cout << "\nSelect a mood:\n";
                cout << "1. Light-hearted\n2. Intense\n3. Thoughtful\n";
                int moodChoice = getMenuChoice(1, 3, "Enter mood number (1-3): ");
                string mood = moodNumberToName(moodChoice);

                // ---- Step 3: Generate recommendation ----
                bool found = false;
                Movie result = recommendMovie(catalog, genre, mood, found);

                cout << "\n------------------------------------------------------\n";
                if (found) {
                    cout << "Recommended for you: " << result.title << "\n";
                    cout << "Genre: " << result.genre << " | Mood: " << result.mood
                         << " | Rating: " << result.rating << "/10\n";
                    cout << "Description: " << result.description << "\n";
                    cout << "------------------------------------------------------\n";

                    // ---- Offer to add to watchlist ----
                    cout << "Add this to your watchlist? (1 = Yes, 2 = No): ";
                    int addChoice = getMenuChoice(1, 2, "");
                    if (addChoice == 1) {
                        watchlist.push_back(result);
                        cout << "\"" << result.title << "\" added to your watchlist.\n";
                    }
                } else {
                    cout << "Sorry, no exact match was found for that combination.\n";
                    cout << "Try browsing the full catalog to see all available titles!\n";
                }
                break;
            }

            case 2:
                showWatchlist(watchlist);
                break;

            case 3:
                showCatalog(catalog);
                break;

            case 4:
                saveWatchlistToFile(watchlist);
                break;

            case 5:
                running = false;
                cout << "\nThanks for using Movie Recommendation Assistant. Enjoy streaming!\n";
                break;

            default:
                cout << "Invalid option.\n";
        }
    }

    return 0;
}
// ---------------------------------------------------------------------
// Prints a short welcome / intro banner
// ---------------------------------------------------------------------
void displayWelcome() {
    cout << "*******************************************************\n";
    cout << "    MOVIE RECOMMENDATION ASSISTANT (Netflix-Inspired)\n";
    cout << "    Tell us your genre and mood, we'll suggest a movie\n";
    cout << "*******************************************************\n";
}

// ---------------------------------------------------------------------
// Reads and validates an integer menu choice from the user.
// ---------------------------------------------------------------------
int getMenuChoice(int minOption, int maxOption, const string& prompt) {
    int choice;
    while (true) {
        if (!prompt.empty()) cout << prompt;
        cin >> choice;

        if (cin.fail()) {
            clearInputError();
            cout << "Invalid input. Please enter a number between "
                 << minOption << " and " << maxOption << ".\n";
            continue;
        }

        if (choice < minOption || choice > maxOption) {
            cout << "Out of range. Please enter a number between "
                 << minOption << " and " << maxOption << ".\n";
            continue;
        }

        return choice;
    }
}
// ---------------------------------------------------------------------
// Clears cin's error flags and discards bad input from the buffer
// ---------------------------------------------------------------------
void clearInputError() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ---------------------------------------------------------------------
// Converts a menu number to a genre name string
// ---------------------------------------------------------------------
string genreNumberToName(int genreChoice) {
    switch (genreChoice) {
        case 1: return "Action";
        case 2: return "Comedy";
        case 3: return "Drama";
        case 4: return "Sci-Fi";
        case 5: return "Documentary";
        default: return "Unknown";
    }
}

// ---------------------------------------------------------------------
// Converts a menu number to a mood name string
// ---------------------------------------------------------------------
string moodNumberToName(int moodChoice) {
    switch (moodChoice) {
        case 1: return "Light";
        case 2: return "Intense";
        case 3: return "Thoughtful";
        default: return "Unknown";
    }
}

// ---------------------------------------------------------------------
// Builds a hardcoded catalog of movies.
// ---------------------------------------------------------------------
vector<Movie> buildCatalog() {
    vector<Movie> catalog;

    catalog.push_back({"Mad Max Rush", "Action", "Intense",
        "A high-octane chase through a post-apocalyptic wasteland.", 8.1});
    catalog.push_back({"Weekend Getaway", "Action", "Light",
        "Two friends stumble into an action-comedy heist gone hilariously wrong.", 7.2});

    catalog.push_back({"Office Antics", "Comedy", "Light",
        "A mockumentary-style comedy about chaos in a small startup office.", 7.8});
    catalog.push_back({"The Long Con", "Comedy", "Thoughtful",
        "A witty comedy that quietly explores honesty and friendship.", 7.5});

    catalog.push_back({"Silent Harbor", "Drama", "Thoughtful",
        "A slow-burn drama about a fishing town confronting its past.", 8.4});
    catalog.push_back({"Breaking Point", "Drama", "Intense",
        "An intense courtroom drama with high emotional stakes.", 8.6});

    catalog.push_back({"Orbit Zero", "Sci-Fi", "Intense",
        "A stranded crew must outsmart a failing space station AI.", 8.3});
    catalog.push_back({"Tomorrow's Echo", "Sci-Fi", "Thoughtful",
        "A quiet, philosophical look at memory and artificial intelligence.", 8.0});

    catalog.push_back({"Stream Wars", "Documentary", "Thoughtful",
        "A documentary tracing how streaming platforms reshaped entertainment.", 8.7});
    catalog.push_back({"Feel Good Facts", "Documentary", "Light",
        "A light, fun documentary series about odd but true world records.", 7.0});

    return catalog;
}

// ---------------------------------------------------------------------
// Searches the catalog for a movie matching both genre and mood.
// ---------------------------------------------------------------------
Movie recommendMovie(const vector<Movie>& catalog, const string& genre, const string& mood, bool& found) {
    for (const Movie& m : catalog) {
        if (m.genre == genre && m.mood == mood) {
            found = true;
            return m;
        }
    }
    found = false;
    return Movie{"", "", "", "", 0.0};
}

// ---------------------------------------------------------------------
// Displays the user's watchlist with an option to remove items
// ---------------------------------------------------------------------
void showWatchlist(vector<Movie>& watchlist) {
    while (true) {
        cout << "\n================ MY WATCHLIST ================\n";
        if (watchlist.empty()) {
            cout << "Your watchlist is empty. Get a recommendation first!\n";
            cout << "================================================\n";
            return;
        } else {
            for (size_t i = 0; i < watchlist.size(); i++) {
                cout << i + 1 << ". " << watchlist[i].title
                     << " (" << watchlist[i].genre << ", " << watchlist[i].mood
                     << ") - Rating: " << watchlist[i].rating << "/10\n";
            }
        }
        cout << "================================================\n";
        cout << "1. Remove a movie from watchlist\n";
        cout << "2. Return to Main Menu\n";

        int choice = getMenuChoice(1, 2, "Enter your choice (1-2): ");
        if (choice == 1) {
            int indexToRemove = getMenuChoice(1, watchlist.size(), "Enter the number of the movie to remove: ");
            cout << "\"" << watchlist[indexToRemove - 1].title << "\" has been removed from your watchlist.\n";
            watchlist.erase(watchlist.begin() + (indexToRemove - 1));
        } else {
            break;
        }
    }
}
// ---------------------------------------------------------------------
// Displays all available movies in the catalog
// ---------------------------------------------------------------------
void showCatalog(const vector<Movie>& catalog) {
    cout << "\n================ FULL MOVIE CATALOG ================\n";
    for (size_t i = 0; i < catalog.size(); i++) {
        cout << i + 1 << ". " << catalog[i].title
             << " [" << catalog[i].genre << " | " << catalog[i].mood << "]\n"
             << "   Desc: " << catalog[i].description << "\n"
             << "   Rating: " << catalog[i].rating << "/10\n";
        cout << "----------------------------------------------------\n";
    }
}

// ---------------------------------------------------------------------
// Saves a copy of the watchlist to a text file ("watchlist.txt")
// ---------------------------------------------------------------------
void saveWatchlistToFile(const vector<Movie>& watchlist) {
    if (watchlist.empty()) {
        cout << "\nYour watchlist is empty. Nothing to save!\n";
        return;
    }

    ofstream outFile("watchlist.txt");
    if (!outFile) {
        cout << "\nError: Could not open file for writing.\n";
        return;
    }

    outFile << "=== MY PERSONAL NETFLIX WATCHLIST ===\n";
    for (size_t i = 0; i < watchlist.size(); i++) {
        outFile << i + 1 << ". " << watchlist[i].title
                << " | Genre: " << watchlist[i].genre
                << " | Mood: " << watchlist[i].mood
                << " | Rating: " << watchlist[i].rating << "/10\n";
    }
    outFile.close();

    cout << "\nSuccess! Your watchlist has been saved to 'watchlist.txt'.\n";
}

// ---------------------------------------------------------------------
// Complete Testing
// ---------------------------------------------------------------------
