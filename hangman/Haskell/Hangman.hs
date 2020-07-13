{-# LANGUAGE MultiWayIf #-}

{- A simple gameLoop of hangman.
   Heavily refactored and cleaned thanks to code review.
   https://codereview.stackexchange.com/a/245167/217623
-}
module Hangman where

import qualified Data.Set as Set
import qualified System.IO as IO
import Control.Monad.State
import Control.Monad.IO.Class(liftIO)

{-
Letter  : Letters of word
Term    : Word to guess (type name Term to avoid ambiguity)
Guessed : Already guessed characters
Status  : Status of game
-}
data Letter  
  = Hidden Char 
  | Revealed Char 
  deriving (Eq)

type Term    = [Letter]

type Guessed = Set.Set Char

data Hangman = Hangman {word :: Term 
                       ,lives :: Int 
                       ,guessedChars :: Guessed
                       } 

type HangmanM a = StateT Hangman IO a

-- pro bono
mkHangman :: String -> Int -> Hangman
mkHangman word lives = Hangman (map Hidden word) lives Set.empty

sampleMan :: Hangman
sampleMan = mkHangman "apple" 7

data Status  
  = Playing 
  | Defeat 
  | Victory 
  | Repeat 
  deriving (Show)

unwrapLetter :: Letter -> Char
unwrapLetter (Hidden _)   = '_'
unwrapLetter (Revealed c) = c

revealTerm :: Term -> String
revealTerm = map revealLetter
  where revealLetter (Hidden c)   = c
        revealLetter (Revealed c) = c

instance Show Hangman where
  show (Hangman words lives guessedChars) =
    unwords [shownWord
            ,"\nLives: "
            ,show lives
            ,"  Guesses: "
            ,shownGuessedChars]
    where shownWord         = map unwrapLetter words
          shownGuessedChars = Set.elems guessedChars

main = do
  IO.hSetEcho IO.stdin False
  IO.hSetBuffering IO.stdin IO.NoBuffering
  playGame sampleMan

playGame :: Hangman -> IO ()
playGame hangman = do
  (state, hangman') <- runStateT gameLoop hangman
  case state of

    Victory -> 
      putStrLn $ "\nYou won! You had " ++ livesLeft ++ " guesses left!"
      where livesLeft = show $ lives hangman'

    Defeat  -> 
      putStrLn $ "\nYou lost! Word was: \"" ++ guessWord ++ "\""
      where guessWord = revealTerm $ word hangman

    _       -> error "This should never happen."

gameLoop :: HangmanM Status
gameLoop = do
  newGuess <- liftIO IO.getChar
  liftIO $ putStrLn $ "Your guess: " ++ [newGuess]
  val <- guess newGuess
  case val of 
    Repeat -> do
      liftIO $ putStrLn "You already tried that.\n"
      gameLoop
    Playing -> do
      newHangman <- get
      liftIO $ putStrLn $ show newHangman ++ "\n"
      gameLoop
    Victory -> return Victory
    Defeat  -> return Defeat


guess :: Char -> HangmanM Status
guess guessChar = do
  h@(Hangman word lives guessedChars) <- get
  let newLives       = lives - 1
      updatedGuesses = Set.insert guessChar guessedChars
  if 
    | guessChar `elem` guessedChars ->
      return Repeat

    | Hidden guessChar `elem` word -> do
      let updatedWord = updateWord word guessChar
      put $ Hangman updatedWord newLives updatedGuesses
      return $ hasWon updatedWord

    | newLives <= 0 -> 
      return Defeat
    
    | otherwise -> do
      put h{lives=newLives, guessedChars=updatedGuesses}
      return Playing


updateWord :: Term -> Char -> Term
updateWord word guessChar = map helper word
  where 
    helper (Hidden hiddenChar)
      | hiddenChar == guessChar = Revealed guessChar
    helper val                  = val
  
hasWon :: Term -> Status
hasWon word = if all isRevealed word then Victory else Playing
  where 
    isRevealed (Hidden _)   = False
    isRevealed (Revealed _) = True