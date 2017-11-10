# Chess-Game
A two player chess game implemented by C language with console interface.

# Description

### When you Run the game the Main Menu appears with different options:
* New: to start a new game
* Save: at first this option is disabled (will see it later)
* Load: to load one of the previously saved games.

![intro](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/snap_intro.PNG)

### When you start a new game you're prompted to enter the players name (players are obliged to choose differnet names).

![[playersname](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/snap_player_names.PNG)

### After this step, the board appears with the following parts:
* The tomb: in the upper part lies the tomb where each player's captured pieces are listed.
* The Board: the chess board in the middle
* Hints: in the right part, there're some hints about options to do while playing like:undo, redo, and showing the main menu.
* Prompt: in the lower part, the player is prompted to enter his move (the player's name and color of his pieces is mentioned).

![board](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/snap_board.PNG)

### When you hit ESCAPE the menu appears again but with different options:
* Resume: to resume the current game.
* Save: now the "save" option is enabled and you can save the game where it's added the "Load list".

![menu](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/snap_menu.PNG)

## Main Game

Each player moves his own pieces by typing the coordinations of the desired piece to move followed by the Destination.

for example
> B7B6

If it's an illegal move it's discarded and the player is prompted and asked to enter a new move again.
When an legal move is entered the move is applied and the next player take the turn.

You can save or load a game through the previous menu.

You can undo or redo a move by hitting the keys U,R respectively on your keyboard.

## Sample Run

#### player1 (Mike) moves one of his pawns.
> A2A3

![sample](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/sample_run_1.PNG)

#### The move is legal, so it's performed and player2 takes his turn.

![sample](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/sample_run_2.PNG)

#### player2 (Jane) enters an illegal command to move his pawn.

> B7B4

![sample](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/sample_run_3.PNG)

#### He enters a new move again, then Mike takes the turn.

> D7D5

![sample](https://raw.githubusercontent.com/youssef-ahmed/Chess-Game/master/sample_run_4.PNG)

##  Game End Conditions:
- The game is won by checkmate.

- The game may end in a draw by stalemate.


## Credits:
### contributor [Muhamed Murad](https://github.com//yahia-elshahawy)
