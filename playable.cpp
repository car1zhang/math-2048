#include "game.h"
#include "rng.h"

int main() {
  ios::sync_with_stdio(0);
  cout<<"This game uses WASD controls and standard IO.\n";
  Rng* rng=new Rng();
  Game* game=new Game(2,2048,rng);
  game->printGrid();
  while(1) {
    char move;
    cin>>move;
    switch(move) {
      case 'w': 
        if(game->moveUp()) {
          game->printGrid();
        }
        break;
      case 'a':
        if(game->moveLeft()) {
          game->printGrid();
        }
        break;
      case 's':
        if(game->moveDown()) {
          game->printGrid();
        }
        break;
      case 'd':
        if(game->moveRight()) {
          game->printGrid();
        }
        break;
    }
    if(game->getIsWon()) {
      cout<<"You win!\n"<<"Score: "<<game->getScore()<<'\n'<<"Moves: "<<game->getNumMoves()<<'\n';
      break;
    }
    if(game->getIsLost()) {
      cout<<"You lose.\n"<<"Score: "<<game->getScore()<<'\n'<<"Moves: "<<game->getNumMoves()<<'\n';
      break;
    }
  }
  return 0;
}
