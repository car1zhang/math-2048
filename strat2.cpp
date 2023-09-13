#include "game.h"
#include "rng.h"
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <math.h>

#define underline "\033[4m"
#define nounderline "\033[0m"

using namespace std;

int main() {
  int size=4,target=2048,trials;
  /*
  cout<<"Enter grid size: ";
  cin>>size;
  cout<<"Enter target value: ";
  cin>>target;
  */
  cout<<"Enter number of trials: ";
  cin>>trials;
  Rng* rng=new Rng();
  long long totalScore=0,totalMaxTile=0;
  int wins=0;
  vector<int> scores,maxTiles;
  for(int i=1;i<=trials;++i) {
    Game* game = new Game(size,target,rng);

    while(1) {
      bool moved=0;
      if(game->getIsWon()) {
        ++wins;
        scores.push_back(game->getScore());
        totalScore+=game->getScore();
        maxTiles.push_back(game->getMaxTileIn(1,1,size,size));
        totalMaxTile+=game->getMaxTileIn(1,1,size,size);
        cout<<'x';
        if(i%20==0) cout<<'\n';
        break;
      }
      if(game->getIsLost()) {
        scores.push_back(game->getScore());
        totalScore+=game->getScore();
        maxTiles.push_back(game->getMaxTileIn(1,1,size,size));
        totalMaxTile+=game->getMaxTileIn(1,1,size,size);
        cout<<'.';
        if(i%20==0) cout<<'\n';
        break;
      }
      
      // merge bottom row if possible
      for(int c=1;c<=3;++c) {
        if(game->getValueAt(4,c)==game->getValueAt(4,c+1)&&game->getValueAt(4,c)>0) {
          game->moveLeft();
          moved=1;
          break;
        }
      }
      if(moved) continue;
      // direct merges into bottom row
      for(int c=1;c<=4;++c) {
        for(int r=3;r>=1;--r) {
          if(game->getValueAt(r,c)>0) {
            if(game->getValueAt(r,c)==game->getValueAt(4,c)) {
              game->moveDown();
              moved=1;
            } else break;
          }
        }
      }
      if(moved) continue;
      // check for gaps in bottom row
      for(int c=1;c<=4;++c) {
        if(game->getValueAt(4,c)==0) {
          // plug from left if possible
          if(game->getMaxTileIn(4,c+1,4,4)>0) game->moveLeft();
          // try to plug from above
          else {
            if(!game->moveDown())
              if(!game->moveLeft())
                if(!game->moveRight())
                  game->moveUp();
          }
          moved=1;
        }
      }
      if(moved) continue;
      // align 3rd row
      // if 3rd row is clogged
        // look at 2nd row
      // otherwise evaluate left, right, down
        // left if row 3 blocking row 4
        // otherwise
          // right if leads to direct merges with row 4
          // down if direct merges into row 3
          // right if it doesn't block row 4

      if(!game->moveDown())
        if(!game->moveRight())
          if(!game->moveLeft())
            game->moveUp();
    }
  }

  long double meanScore=((long double)totalScore/(long double)trials);
  long double stdScore=0;
  for(int s:scores) stdScore+=((long double)s-meanScore)*((long double)s-meanScore);
  stdScore=sqrt(stdScore/(long double)trials);

  long double meanMaxTile=((long double)totalMaxTile/(long double)trials);
  long double stdMaxTile=0;
  for(int s:maxTiles) stdMaxTile+=((long double)s-meanMaxTile)*((long double)s-meanMaxTile);
  stdMaxTile=sqrt(stdMaxTile/(long double)trials);

  cout<<"Wins: "<<wins<<'/'<<trials<<'\n';
  cout<<underline<<"Score"<<'\n'<<nounderline;
  cout<<"Mean: "<<meanScore<<'\n';
  cout<<"Standard Deviation: "<<stdScore<<'\n';
  cout<<underline<<"Max Tile"<<'\n'<<nounderline;
  cout<<"Mean: "<<meanMaxTile<<'\n';
  cout<<"Standard Deviation: "<<stdMaxTile<<'\n';
  return 0;
}
