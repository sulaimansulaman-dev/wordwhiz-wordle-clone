#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <memory>
#include <locale>  // Include the locale library for tolower
#include <algorithm>  // Include the algorithm library for std::transform


// Define a struct to hold game information
struct GameInfo {
    std::string secretWord;
    int attemptsLeft;
};

// Define a template class for word lists
template <typename T, size_t N>
class WordList {
public:
    WordList(const std::string& filename) {
        LoadWords(filename);
    }

    T SelectRandomWord() {
        if (numWords == 0) {
            throw std::runtime_error("Word list is empty.");
        }
        int randomIndex = rand() % numWords;
        return words[randomIndex];
    }

private:
    T words[N];
    size_t numWords = 0;

    void LoadWords(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Error: Could not open word list file.");
        }

        T word;
        while (numWords < N && file >> word) {
            // Convert the word to lowercase before storing it
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            words[numWords++] = word;
        }
        file.close();
    }
};

// Define the Wordle class
class Wordle {
public:
    Wordle() : attemptsLeft(6) {
        WordList<std::string, 1000> wordList("wordlist.txt"); // Load the word list from a file (up to 1000 words)
        srand(time(nullptr)); // Seed the random number generator
        gameInfo.secretWord = wordList.SelectRandomWord();
    }

    // Play the Wordle game
    void Play() {
        std::cout << "Welcome to Wordle! You have " << attemptsLeft << " attempts to guess the secret word." << std::endl;

        while (attemptsLeft > 0) {
            std::cout << "Attempt #" << 7 - attemptsLeft << ": ";
            std::string guess;
            std::cin >> guess;

            // Convert the guess to lowercase
            std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);

            if (guess.length() != gameInfo.secretWord.length()) {
                std::cout << "Invalid guess length. Please guess a " << gameInfo.secretWord.length() << "-letter word." << std::endl;
                continue;
            }

            if (IsCorrectGuess(guess)) {
                std::cout << "Congratulations! You've guessed the secret word: " << gameInfo.secretWord << std::endl;
                break;
            }

            DisplayHint(guess);
            attemptsLeft--;
        }

        if (attemptsLeft == 0) {
            std::cout << "Game over! The secret word was: " << gameInfo.secretWord << std::endl;
        }
    }

    // Ask the player if they want to play again
    bool PlayAgain() {
        std::cout << "Would you like to play again? (yes/no): ";
        std::string response;
        std::cin >> response;
        return (response == "yes" || response == "Yes" || response == "YES");
    }

private:
    GameInfo gameInfo;
    int attemptsLeft;

    bool IsCorrectGuess(const std::string& guess) {
        return guess == gameInfo.secretWord;
    }

    void DisplayHint(const std::string& guess) {
        std::string hint = "";
        for (size_t i = 0; i < gameInfo.secretWord.length(); i++) {
            if (guess[i] == gameInfo.secretWord[i]) {
                hint += guess[i];
            } else if (gameInfo.secretWord.find(guess[i]) != std::string::npos) {
                hint += '+';
            } else {
                hint += '-';
            }
        }

        std::cout << "Hint: " << hint << std::endl;
    }
};

int main() {
    bool playAgain;
    do {
        try {
            std::unique_ptr<Wordle> game = std::make_unique<Wordle>();
            game->Play();
            playAgain = game->PlayAgain();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    } while (playAgain);

    return 0;
}


