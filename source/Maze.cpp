/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the MIT License.
 * If a copy of the ML was not distributed with this
 * file, You can obtain one at https://opensource.org/licenses/MIT */

/* author jackred@tuta.io */

#include "Maze.hpp"

backstage::Maze::Maze(size_t width, size_t length) {
  srand(time(NULL));
  _maze = generateEmptyMaze(width, length);
  generateSeed();
  generateCorridor();
  eraseCorridor();
  toWalls();
}

backstage::maze_t backstage::Maze::generateEmptyMaze(size_t width, size_t length) {
  maze_t newMaze;
  for (size_t i=0 ; i<width ; i++) {
    std::vector<unsigned int> arr(length);
    newMaze.push_back(arr);
    for (size_t j=0; j<newMaze[i].size(); j++) {
      newMaze[i][j]=0;
    }
  }
  return newMaze;
}

void backstage::Maze::generateSeed() {
  for (size_t i=_maze.size()/2 ; i<(_maze.size()/2)+5 ; i++) {
    for (size_t j=_maze[i].size()/2 ; j<(_maze[i].size()/2)+5 ; j++) {
      _maze[i][j] = (rand() % 2);
    }
  }
}

unsigned int backstage::Maze::countNeighbors(backstage::maze_t maze, size_t x, size_t y) {
  unsigned int sum=0;
  size_t minx = (x == 0) ? 0 : x-1;
  size_t miny = (y == 0) ? 0 : y-1;
  for (size_t i=minx ; i<std::min(maze.size(), x+2) ; i++) {
    for (size_t j=miny ; j<std::min(maze[0].size(), y+2) ; j++) {
      if ((i != x) || (j != y)) {
        sum += maze[i][j];
      }
    }
  }
  return sum;
}

unsigned int backstage::Maze::ruleB3_1234Cell(backstage::maze_t maze, size_t x, size_t y) {
  unsigned int nbNeighbors = countNeighbors(maze, x, y);
  return (((maze[x][y] == 1) && ((nbNeighbors >= 1) && (nbNeighbors <= 4)))|| nbNeighbors == 3);
}

void backstage::Maze::ruleB3_1234Iteration() {
  backstage::maze_t newMaze = generateEmptyMaze(_maze.size(), _maze[0].size());
  for (size_t i=0 ; i<_maze.size() ; i++) {
    for (size_t j=0 ; j<_maze[i].size() ; j++) {
      newMaze[i][j] = ruleB3_1234Cell(_maze, i, j);
    }
  }
  _maze = newMaze;
}

void backstage::Maze::generateCorridor() {
  for (int k = 0 ; k < std::max(_maze.size(), _maze[0].size()) * 3 ; k ++) {
    std::cout << *this << std::endl;
    ruleB3_1234Iteration();
  }
}

void backstage::Maze::eraseCorridor() {
  for (size_t i=0 ; i<_maze.size() ; i++) {
    for (size_t j=0 ; j<_maze[i].size() ; j++) {
      size_t tmpCount = countNeighbors(_maze, i, j);
      if ((_maze[i][j] == 1) && ( tmpCount>= 1) && (tmpCount <= 5) && ((rand() % 10) == 0)){
        _maze[i][j] = 0;
      }
    }
  }
}

void backstage::Maze::toWalls() {
  
}

/* overloaded */

std::ostream& backstage::operator<<(std::ostream &os, const backstage::Maze &maze) {
  backstage::maze_t arr = maze.getMaze();
  os << "\033c";
  for (size_t i=0; i<arr.size(); i++) {
    for (size_t j=0; j<arr[i].size(); j++) {
      if (arr[i][j] == 1) {
        os << "\033[31m1";
      } else {
        os << "\033[97m0";
      }
    }
    os << std::endl;
  }
  return os;
}

/* getters */

backstage::walls_t backstage::Maze::getWalls() const {
  return _walls;
}

glm::vec2 backstage::Maze::getStart() const {
  return _start;
}

glm::vec2 backstage::Maze::getEnd() const {
  return _end;
}

std::vector<std::vector<unsigned int>> backstage::Maze::getMaze() const {
  return _maze;
}

