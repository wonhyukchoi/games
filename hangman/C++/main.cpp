/* Reference hangman implementation in C++. */
#include "hangman.h"


int main() {
    Hangman hangman = Hangman("foo", 7);
    hangman.play();
}
