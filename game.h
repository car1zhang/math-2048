#pragma once
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <time.h>
#include "rng.h"

using namespace std;

class Game {

  public:
  Game(int iBoardSize,int iWinValue,Rng* iRng) {
    numMoves=0;
    score=0;
    boardSize=iBoardSize;
    winValue=iWinValue;
    rng=iRng;
    game.resize(boardSize+2);
    for(int i=0;i<boardSize+2;++i) game[i].resize(boardSize+2);
    merged.resize(boardSize+2);
    for(int i=0;i<boardSize+2;++i) merged[i].resize(boardSize+2);
    generateTile();
    generateTile();
  }

  void printGrid() {
    for(int r=1;r<=boardSize;++r)
      for(int c=1;c<=boardSize;++c) cout<<game[r][c]<<" \n"[c==boardSize];
  }

  bool moveUp() {
    if(!canMoveUp()) return 0;
    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=1;r<=boardSize;++r) {
      for(int c=1;c<=boardSize;++c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=r-1;i>=0;--i) {
          if(game[i][c]==curr&&!merged[i][c]) {
            game[i][c]+=curr;
            score+=game[i][c];
            merged[i][c]=1;
            break;
          }
          if(i==0||game[i][c]>0) {
            game[i+1][c]=curr;
            break;
          }
        }
      }
    }
    generateTile();
    ++numMoves;
    return 1;
  }

  bool moveDown() {
    if(!canMoveDown()) return 0;
    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=boardSize;r>=1;--r) {
      for(int c=1;c<=boardSize;++c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=r+1;i<=boardSize+1;++i) {
          if(game[i][c]==curr&&!merged[i][c]) {
            game[i][c]+=curr;
            score+=game[i][c];
            merged[i][c]=1;
            break;
          }
          if(i==boardSize+1||game[i][c]>0) {
            game[i-1][c]=curr;
            break;
          }
        }
      }
    }
    generateTile();
    ++numMoves;
    return 1;
  }

  bool moveLeft() {
    if(!canMoveLeft()) return 0;
    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=1;r<=boardSize;++r) {
      for(int c=1;c<=boardSize;++c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=c-1;i>=0;--i) {
          if(game[r][i]==curr&&!merged[r][i]) {
            game[r][i]+=curr;
            score+=game[i][c];
            merged[r][i]=1;
            break;
          }
          if(i==0||game[r][i]>0) {
            game[r][i+1]=curr;
            break;
          }
        }
      }
    }
    generateTile();
    ++numMoves;
    return 1;
  }

  bool moveRight() {
    if(!canMoveRight()) return 0;
    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=1;r<=boardSize;++r) {
      for(int c=boardSize;c>=1;--c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=c+1;i<=boardSize+1;++i) {
          if(game[r][i]==curr&&!merged[r][i]) {
            game[r][i]+=curr;
            score+=game[i][c];
            merged[r][i]=1;
            break;
          }
          if(i==boardSize+1||game[r][i]>0) {
            game[r][i-1]=curr;
            break;
          }
        }
      }
    }
    ++numMoves;
    generateTile();
    return 1;
  }

  bool getIsLost() {
    return !(canMoveUp()||canMoveDown()||canMoveLeft()||canMoveRight());
  }
  bool getIsWon() {
    bool found=0;
    for(int r=1;r<=boardSize;++r) for(int c=1;c<=boardSize;++c) if(game[r][c]>=winValue) found=1;
    return found;
  }
  int getScore() { return score; }
  int getNumMoves() { return numMoves; }
  int getValueAt(int r,int c) { return game[r][c]; }
  int getMaxTileIn(int r1,int c1,int r2,int c2) {
    int maxTile=0;
    for(int r=r1;r<=r2;++r) for(int c=c1;c<=c2;++c) maxTile=max(maxTile,game[r][c]);
    return maxTile;
  }
  int getMinTileIn(int r1,int c1,int r2,int c2) {
    int minTile=0;
    for(int r=r1;r<=r2;++r) for(int c=c1;c<=c2;++c) minTile=min(minTile,game[r][c]);
    return minTile;
  }

  private:
  bool canMoveUp() {
    bool valid=0;
    for(int r=2;r<=boardSize;++r) {
      for(int c=1;c<=boardSize;++c) {
        if(game[r][c]&&(game[r-1][c]==game[r][c]||!game[r-1][c])) valid=1;
      }
    }
    return valid;
  }
  bool canMoveDown() {
    bool valid=0;
    for(int r=1;r<=boardSize-1;++r) {
      for(int c=1;c<=boardSize;++c) {
        if(game[r][c]&&(game[r+1][c]==game[r][c]||!game[r+1][c])) valid=1;
      }
    }
    return valid;
  }
  bool canMoveLeft() {
    bool valid=0;
    for(int r=1;r<=boardSize;++r) {
      for(int c=2;c<=boardSize;++c) {
        if(game[r][c]&&(game[r][c-1]==game[r][c]||!game[r][c-1])) valid=1;
      }
    }
    return valid;
  }
  bool canMoveRight() {
    bool valid=0;
    for(int r=1;r<=boardSize;++r) {
      for(int c=1;c<=boardSize-1;++c) {
        if(game[r][c]&&(game[r][c+1]==game[r][c]||!game[r][c+1])) valid=1;
      }
    }
    return valid;
  }
  void generateTile() {
    vector<pair<int,int>> avail;
    for(int r=1;r<=boardSize;++r) for(int c=1;c<=boardSize;++c) if(!game[r][c]) avail.push_back({r,c});
    pair<int,int> loc=avail[rng->generate(0,avail.size()-1)];
    int num=(rng->generate(0,10)==0?4:2);
    game[loc.first][loc.second]=num;
  }
  vector<vector<int>> game;
  vector<vector<bool>> merged;
  int numMoves;
  int score;
  int boardSize;
  int winValue;
  Rng* rng;
};
