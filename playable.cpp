#include "game.h"

int main() {
  ios::sync_with_stdio(0);
  Game* game = new Game();
  game->printGrid();
  while(1) {
    char move;
    cin>>move;
    switch(move) {
      case 'w': 
        if(game->moveUp()) {
          game->generateTile();
          game->printGrid();
        }
        break;
      case 'a':
        if(game->moveLeft()) {
          game->generateTile();
          game->printGrid();
        }
        break;
      case 's':
        if(game->moveDown()) {
          game->generateTile();
          game->printGrid();
        }
        break;
      case 'd':
        if(game->moveRight()) {
          game->generateTile();
          game->printGrid();
        }
        break;
    }
  }
  return 0;
}
