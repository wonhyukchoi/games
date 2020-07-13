#ifndef HANGMAN_HANGMAN_H
#define HANGMAN_HANGMAN_H
#include <string>
#include <iostream>
#include <unordered_set>
#define CHAR_ELEM(char, string) (string.find(char) < string.length())

class Hangman {
public:
    Hangman(const std::string& word, int lives);
    ~Hangman();
    void play();
private:
    int lives;
    int numGuesses;
    std::string word;
    std::string showWord;
    std::unordered_set<char> guessed;
    bool hasWon(){return not(CHAR_ELEM('_', showWord));};
    bool hasLost(){return (numGuesses >= lives);};
    void victory(){std::cout<<"Victory! You guessed <" << word << "> correctly!\n";};
    void defeat(){std::cout<<"Defeat... Word was <" << word << ">\n";};
    bool charInWord(char guessChar){return CHAR_ELEM(guessChar, word);};
    void updateWord(char guessChar);
    void show();
};


#endif //HANGMAN_HANGMAN_H
