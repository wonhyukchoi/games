#include <iostream>
#include "hangman.h"
#define SET_ELEM(char, set) (set.find(char) != set.end())

Hangman::Hangman(const std::string& word, int lives) {
    this->word = word;
    this->showWord = std::string(word.length(), '_');
    this->lives = lives;
    this->numGuesses = 0;
}

Hangman::~Hangman() {
    std::cout << "Thanks for playing!\n";
}

void Hangman::play() {

    std::cout << "Starting hangman...\n";
    show();

    while(true) {

        std::string input;
        std::cin >> input;
        char guessChar;
        guessChar = input[0];

        if (input.length() != 1){
            std::cout << "Please enter a single character.\n";
        }

        else if (SET_ELEM(guessChar, guessed)){
            std::cout << "You already guessed that!\n";
        }

        else {
            if (charInWord(guessChar)){
                updateWord(guessChar);
                if (hasWon()){victory(); break;}
            }

            guessed.insert(guessChar);
            ++numGuesses;

            if (hasLost()){defeat(); break;}

            show();

        }
    }
}

void Hangman::updateWord(char guessChar) {

    for(int i=0; i < word.length(); i++){
        if (word[i] == guessChar){
            showWord[i] = guessChar;
        }
    }
}

void Hangman::show(){
    std::cout << showWord << '\n';
    std::cout << "Guesses: " << numGuesses << '/' << lives << '\n';
}