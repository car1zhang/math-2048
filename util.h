#pragma once
#include "game.h"

using namespace std;

namespace util {

  vector<vector<int>> potentialLeft(Game* game) {
    vector<vector<int>> grid=game->getGrid();
    vector<vector<bool>> merged(6,vector<bool>(6,0));
    for(auto &i:merged) fill(i.begin(),i.end(),0);

    for(int r=1;r<=4;++r) {
      for(int c=1;c<=4;++c) {
        int curr=grid[r][c];
        grid[r][c]=0;
        if(!curr) continue;

        for(int i=c-1;i>=0;--i) {
          if(grid[r][i]==curr&&!merged[r][i]) {
            grid[r][i]+=curr;
            merged[r][i]=1;
            break;
          }
          if(i==0||grid[r][i]>0) {
            grid[r][i+1]=curr;
            break;
          }
        }
      }
    }
    return grid;
  }

  vector<vector<int>> potentialRight(Game* game) {
    vector<vector<int>> grid=game->getGrid();
    vector<vector<bool>> merged(6,vector<bool>(6,0));
    for(auto &i:merged) fill(i.begin(),i.end(),0);

    for(int r=1;r<=4;++r) {
      for(int c=4;c>=1;--c) {
        int curr=grid[r][c];
        grid[r][c]=0;
        if(!curr) continue;

        for(int i=c+1;i<=5;++i) {
          if(grid[r][i]==curr&&!merged[r][i]) {
            grid[r][i]+=curr;
            merged[r][i]=1;
            break;
          }
          if(i==5||grid[r][i]>0) {
            grid[r][i-1]=curr;
            break;
          }
        }
      }
    }
    return grid;
  }

  vector<vector<int>> potentialDown(Game* game) {
    vector<vector<int>> grid=game->getGrid();
    vector<vector<bool>> merged(6,vector<bool>(6,0));
    for(auto &i:merged) fill(i.begin(),i.end(),0);

    for(int r=4;r>=1;--r) {
      for(int c=1;c<=4;++c) {
        int curr=grid[r][c];
        grid[r][c]=0;
        if(!curr) continue;

        for(int i=r+1;i<=5;++i) {
          if(grid[i][c]==curr&&!merged[i][c]) {
            grid[i][c]+=curr;
            merged[i][c]=1;
            break;
          }
          if(i==5||grid[i][c]>0) {
            grid[i-1][c]=curr;
            break;
          }
        }
      }
    }
    return grid;
  }
}