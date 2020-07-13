"""The simplest game anybody could ever make."""

import json
from random import randint


def load_config(json_dict: dict) -> (int, int, int):
    min_val = json_dict['min']
    max_val = json_dict['max']
    lives = json_dict['lives']
    return min_val, max_val, lives


def play_game(to_guess: int, lives: int,
              min_val: int, max_val: int) -> None:
    while lives > 0:
        guess = input("Your guess: ")

        try:
            guess_val = int(guess)
        except ValueError:
            print("%s is not a valid number."
                  "Please try again." % guess)
            continue

        if guess_val < min_val:
            print(f"{guess_val} is smaller than the minimum, {min_val}. "
                  f"Please try again.")
            continue

        elif guess_val > max_val:
            print(f"{guess_val} is greater than the minimum, {max_val}. "
                  f"Please try again.")
            continue

        if guess_val == to_guess:
            break
        else:
            if guess_val > to_guess:
                print("Too high...")
            elif guess_val < to_guess:
                print("Too low...")
            lives -= 1
            print(f"{lives} lives remaining.\n")

    if lives <= 0:
        print("You lost! The number was {}".format(to_guess))
    else:
        print("You won! You even had {} lives left!".format(lives))


if __name__ == "__main__":

    with open("config.json") as f:
        json_file = json.load(f)
        minimum, maximum, guesses = load_config(json_file)

    print(f"You have {guesses} guesses to guess an integer between "
          f"{minimum} and {maximum}.")

    number = randint(minimum, maximum)
    play_game(to_guess=number, lives=guesses,
              min_val=minimum, max_val=maximum)
    print("Thanks for playing!")
