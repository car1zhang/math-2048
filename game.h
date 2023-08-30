#pragma once
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <time.h>

using namespace std;

class Game {
  public:
  Game() {
    game.resize(6);
    for(int i=0;i<6;++i) game[i].resize(6);
    merged.resize(6);
    for(int i=0;i<6;++i) merged[i].resize(6);
    generateTile();
    generateTile();
  }

  void printGrid() {
    for(int r=1;r<=4;++r)
      for(int c=1;c<=4;++c) cout<<game[r][c]<<" \n"[c==4];
  }

  void generateTile() {
    vector<pair<int,int>> avail;
    for(int r=1;r<=4;++r) for(int c=1;c<=4;++c) if(!game[r][c]) avail.push_back({r,c});
    srand(time(NULL));
    pair<int,int> loc=avail[rand()%avail.size()];
    srand(time(NULL));
    int num=(rand()%11==0?4:2);
    game[loc.first][loc.second]=num;
  }

  bool moveUp() {
    bool valid=0;
    for(int r=2;r<=4;++r) {
      for(int c=1;c<=4;++c) {
        if(game[r][c]&&(game[r-1][c]==game[r][c]||!game[r-1][c])) valid=1;
      }
    }
    if(!valid) return 0;

    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=1;r<=4;++r) {
      for(int c=1;c<=4;++c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=r-1;i>=0;--i) {
          if(game[i][c]==curr&&!merged[i][c]) {
            game[i][c]+=curr;
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
    return 1;
  }

  bool moveDown() {
    bool valid=0;
    for(int r=1;r<=3;++r) {
      for(int c=1;c<=4;++c) {
        if(game[r][c]&&(game[r+1][c]==game[r][c]||!game[r+1][c])) valid=1;
      }
    }
    if(!valid) return 0;

    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=4;r>=1;--r) {
      for(int c=1;c<=4;++c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=r+1;i<=5;++i) {
          if(game[i][c]==curr&&!merged[i][c]) {
            game[i][c]+=curr;
            merged[i][c]=1;
            break;
          }
          if(i==5||game[i][c]>0) {
            game[i-1][c]=curr;
            break;
          }
        }
      }
    }
    return 1;
  }

  bool moveLeft() {
    bool valid=0;
    for(int r=1;r<=4;++r) {
      for(int c=2;c<=4;++c) {
        if(game[r][c]&&(game[r][c-1]==game[r][c]||!game[r][c-1])) valid=1;
      }
    }
    if(!valid) return 0;

    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=1;r<=4;++r) {
      for(int c=1;c<=4;++c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=c-1;i>=0;--i) {
          if(game[r][i]==curr&&!merged[r][i]) {
            game[r][i]+=curr;
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
    return 1;
  }

  bool moveRight() {
    bool valid=0;
    for(int r=1;r<=4;++r) {
      for(int c=1;c<=3;++c) {
        if(game[r][c]&&(game[r][c+1]==game[r][c]||!game[r][c+1])) valid=1;
      }
    }
    if(!valid) return 0;

    for(auto &i:merged) std::fill(i.begin(),i.end(),0);
    for(int r=1;r<=4;++r) {
      for(int c=4;c>=1;--c) {
        int curr=game[r][c];
        game[r][c]=0;
        if(!curr) continue;
        for(int i=c+1;i<=5;++i) {
          if(game[r][i]==curr&&!merged[r][i]) {
            game[r][i]+=curr;
            merged[r][i]=1;
            break;
          }
          if(i==5||game[r][i]>0) {
            game[r][i-1]=curr;
            break;
          }
        }
      }
    }
    return 1;
  }

  private:
  vector<vector<int>> game;
  vector<vector<bool>> merged;
};
