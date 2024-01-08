#include "game.h"
#include "rng.h"
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main() {
  int size,target,trials;
  cout<<"Enter grid size: ";
  cin>>size;
  cout<<"Enter target value: ";
  cin>>target;
  cout<<"Enter number of trials: ";
  cin>>trials;
  Rng* rng=new Rng();
  long long avgScore=0;
  int wins=0;
  for(int i=1;i<=trials;++i) {
    Game* game = new Game(size,target,rng);
    while(1) {
      
      if(!game->moveDown()) {
        if(!game->moveLeft()) {
          if(!game->moveRight()) {
            game->moveUp();
          }
        }
      }

      if(game->getIsWon()) {
        avgScore+=game->getScore();
        ++wins;
        break;
      }
      if(game->getIsLost()) {
        avgScore+=game->getScore();
        cout<<game->getScore()<<'\n';
        break;
      }
    }
  }
  cout<<"Wins: "<<wins<<'/'<<trials<<'\n';
  cout<<"Avg Score: "<<((double)avgScore/(double)trials)<<'\n';
  return 0;
}
