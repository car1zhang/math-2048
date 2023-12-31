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

      // hypothetical left and right 3rd row
      vector<int> arrRight3(6),arrLeft3(6);
      vector<bool> merged(6,0);
      fill(merged.begin(),merged.end(),0);
      for(int c=4;c>=1;--c) {
        int curr=game->getValueAt(3,c);
        if(curr==0) continue;
        for(int i=c+1;i<=5;++i) {
          if(arrRight3[i]==curr&&!merged[i]) {
            arrRight3[i]+=curr;
            merged[i]=1;
            break;
          }
          if(i==5||arrRight3[i]>0) {
            arrRight3[i-1]=curr;
            break;
          }
        }
      }
      fill(merged.begin(),merged.end(),0);
      for(int c=1;c<=4;++c) {
        int curr=game->getValueAt(3,c);
        if(curr==0) continue;
        for(int i=c-1;i>=0;--i) {
          if(arrLeft3[i]==curr&&!merged[i]) {
            arrLeft3[i]+=curr;
            merged[i]=1;
            break;
          }
          if(i==0||arrLeft3[i]>0) {
            arrLeft3[i+1]=curr;
            break;
          }
        }
      }
      if(arrLeft3!=arrRight3) {
      // otherwise align third row
        int valueLeft=0,valueRight=0;
        for(int c=1;c<=4;++c) {
          valueLeft+=(arrLeft3[c]==game->getValueAt(4,c))*arrLeft3[c];
          valueRight+=(arrRight3[c]==game->getValueAt(4,c))*arrRight3[c];
        }
        if(valueLeft>valueRight) {
          if(game->moveLeft()) moved=1;
        } else if(valueLeft<valueRight) {
          if(game->moveRight()) moved=1;
        } else {
          int penaltyLeft=0,penaltyRight=0,penaltyCurr=0;
          for(int c=1;c<=4;++c) {
            penaltyLeft+=max(0,arrLeft3[c]-game->getValueAt(4,c));
            penaltyRight+=max(0,arrRight3[c]-game->getValueAt(4,c));
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

      // hypothetical left and right 2nd row
      vector<int> arrRight2(6),arrLeft2(6);
      fill(merged.begin(),merged.end(),0);
      for(int c=4;c>=1;--c) {
        int curr=game->getValueAt(2,c);
        if(curr==0) continue;
        for(int i=c+1;i<=5;++i) {
          if(arrRight2[i]==curr&&!merged[i]) {
            arrRight2[i]+=curr;
            merged[i]=1;
            break;
          }
          if(i==5||arrRight2[i]>0) {
            arrRight2[i-1]=curr;
            break;
          }
        }
      }
      fill(merged.begin(),merged.end(),0);
      for(int c=1;c<=4;++c) {
        int curr=game->getValueAt(2,c);
        if(curr==0) continue;
        for(int i=c-1;i>=0;--i) {
          if(arrLeft2[i]==curr&&!merged[i]) {
            arrLeft2[i]+=curr;
            merged[i]=1;
            break;
          }
          if(i==0||arrLeft2[i]>0) {
            arrLeft2[i+1]=curr;
            break;
          }
        }
      }
      if(arrLeft2 != arrRight2) {
      // otherwise align second row
        int valueLeft=0,valueRight=0;
        for(int c=1;c<=4;++c) {
          valueLeft+=(arrLeft2[c]==game->getValueAt(3,c))*arrLeft2[c];
          valueRight+=(arrRight2[c]==game->getValueAt(3,c))*arrRight2[c];
        }
        if(valueLeft>valueRight) {
          if(game->moveLeft()) moved=1;
        } else if(valueLeft<valueRight) {
          if(game->moveRight()) moved=1;
        } else {
          int penaltyLeft=0,penaltyRight=0,penaltyCurr=0;
          for(int c=1;c<=4;++c) {
            penaltyLeft+=max(0,arrLeft2[c]-game->getValueAt(3,c));
            penaltyRight+=max(0,arrRight2[c]-game->getValueAt(3,c));
            penaltyCurr+=max(0,game->getValueAt(2,c)-game->getValueAt(3,c));
          }
          if(penaltyRight<=penaltyLeft&&penaltyRight<penaltyCurr) {
            game->moveRight();
            moved=1;
          }
          if(penaltyLeft<=penaltyRight&&penaltyLeft<penaltyCurr) {
            game->moveLeft();
            moved=1;
          }
          if(penaltyLeft==0) {
            if(game->moveLeft()) moved=1;
          }
        }
      }
      if(moved) continue;
      // direct merges into 2nd row
      for(int c=1;c<=4;++c) {
        if(game->getValueAt(1,c)>0) {
          if(game->getValueAt(1,c)==game->getValueAt(2,c)) {
            game->moveDown();
            moved=1;
            break;
          } else break;
        }
        if(moved) break;
      }
      if(moved) continue;

      // hypothetical left and right 1st row
      vector<int> arrRight1(6),arrLeft1(6);
      fill(merged.begin(),merged.end(),0);
      for(int c=4;c>=1;--c) {
        int curr=game->getValueAt(1,c);
        if(curr==0) continue;
        for(int i=c+1;i<=5;++i) {
          if(arrRight1[i]==curr&&!merged[i]) {
            arrRight1[i]+=curr;
            merged[i]=1;
            break;
          }
          if(i==5||arrRight1[i]>0) {
            arrRight1[i-1]=curr;
            break;
          }
        }
      }
      fill(merged.begin(),merged.end(),0);
      for(int c=1;c<=4;++c) {
        int curr=game->getValueAt(1,c);
        if(curr==0) continue;
        for(int i=c-1;i>=0;--i) {
          if(arrLeft1[i]==curr&&!merged[i]) {
            arrLeft1[i]+=curr;
            merged[i]=1;
            break;
          }
          if(i==0||arrLeft1[i]>0) {
            arrLeft1[i+1]=curr;
            break;
          }
        }
      }
      if(arrLeft1 != arrRight1) {
      // otherwise align second row
        int valueLeft=0,valueRight=0;
        for(int c=1;c<=4;++c) {
          valueLeft+=(arrLeft1[c]==game->getValueAt(2,c))*arrLeft1[c];
          valueRight+=(arrRight1[c]==game->getValueAt(2,c))*arrRight1[c];
        }
        if(valueLeft>valueRight) {
          if(game->moveLeft()) moved=1;
        } else if(valueLeft<valueRight) {
          if(game->moveRight()) moved=1;
        } else {
          int penaltyLeft=0,penaltyRight=0,penaltyCurr=0;
          for(int c=1;c<=4;++c) {
            penaltyLeft+=max(0,arrLeft1[c]-game->getValueAt(2,c));
            penaltyRight+=max(0,arrRight1[c]-game->getValueAt(2,c));
            penaltyCurr+=max(0,game->getValueAt(1,c)-game->getValueAt(2,c));
          }
          if(penaltyRight<=penaltyLeft&&penaltyRight<penaltyCurr) {
            game->moveRight();
            moved=1;
          }
          if(penaltyLeft<=penaltyRight&&penaltyLeft<penaltyCurr) {
            game->moveLeft();
            moved=1;
          }
          if(penaltyRight==0) {
            if(game->moveLeft()) moved=1;
          }
        }
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
