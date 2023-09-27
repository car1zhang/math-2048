#include "game.h"
#include "rng.h"
#include "util.h"
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
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
    bool alreadyWon=0;

    while(1) {
      if(trials==1) {
        cout<<'\n';
        game->printGrid();
      }
      bool moved=0;
      if(game->getIsWon()&&!alreadyWon) {
        ++wins;
        cout<<'x';
        alreadyWon=1;
      }
      if(game->getIsLost()) {
        scores.push_back(game->getScore());
        totalScore+=game->getScore();
        maxTiles.push_back(game->getMaxTileIn(1,1,size,size));
        totalMaxTile+=game->getMaxTileIn(1,1,size,size);
        if(!alreadyWon) cout<<'.';
        if(i%50==0) cout<<'\n';
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
      // check for gaps in bottom row
      for(int c=1;c<=4;++c) {
        if(game->getValueAt(4,c)==0) {
          // plug from right if possible
          if(game->getMaxTileIn(4,c+1,4,4)>0) game->moveLeft();
          // try to plug from above
          else {
            if(!game->moveDown())
              if(!game->moveLeft())
                if(!game->moveRight())
                  game->moveUp();
          }
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
              break;
            } else break;
          }
        }
        if(moved) break;
      }
      if(moved) continue;

      vector<vector<int>> potentialLeft=util::potentialLeft(game);
      vector<vector<int>> potentialRight=util::potentialRight(game);
      vector<vector<int>> potentialDown=util::potentialDown(game);

      if(potentialLeft[3]!=potentialRight[3]) {
      // otherwise align third row
        int valueLeft=0,valueRight=0,valueDown=0;
        for(int c=1;c<=4;++c) {
          valueLeft+=(potentialLeft[3][c]==game->getValueAt(4,c))*potentialLeft[3][c];
          valueRight+=(potentialRight[3][c]==game->getValueAt(4,c))*potentialRight[3][c];
          valueDown+=(potentialDown[3][c]==game->getValueAt(4,c))*potentialDown[3][c];
        }
        if(valueLeft>valueRight&&valueLeft>valueDown) {
          if(game->moveLeft()) moved=1;
        } else if(valueRight>valueLeft&&valueRight>valueDown) {
          if(game->moveRight()) moved=1;
        } else if(valueDown>valueLeft&&valueDown>valueRight) {
          if(game->moveDown()) moved=1;
        } else {
          int penaltyLeft=0,penaltyRight=0,penaltyCurr=0;
          for(int c=1;c<=4;++c) {
            penaltyLeft+=max(0,potentialLeft[3][c]-game->getValueAt(4,c));
            penaltyRight+=max(0,potentialRight[3][c]-game->getValueAt(4,c));
            penaltyCurr+=max(0,game->getValueAt(3,c)-game->getValueAt(4,c));
          }
          if(penaltyRight==0) {
            if(game->moveRight()) moved=1;
          }
          if(penaltyRight<=penaltyLeft&&penaltyRight<penaltyCurr) {
            game->moveRight();
            moved=1;
          }
          if(penaltyLeft<=penaltyRight&&penaltyLeft<penaltyCurr) {
            game->moveLeft();
            moved=1;
          }
        }
      }
      if(moved) continue;
      // direct merges into 3rd row
      for(int c=1;c<=4;++c) {
        for(int r=2;r>=1;--r) {
          if(game->getValueAt(r,c)>0) {
            if(game->getValueAt(r,c)==game->getValueAt(3,c)) {
              game->moveDown();
              moved=1;
              break;
            } else break;
          }
        }
        if(moved) break;
      }
      if(moved) continue;

      if(!game->moveDown())
        if(!game->moveLeft())
          if(!game->moveRight())
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
