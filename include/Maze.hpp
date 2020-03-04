/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the MIT License. 
 * If a copy of the ML was not distributed with this
 * file, You can obtain one at https://opensource.org/licenses/MIT */

/* author jackred@tuta.io */

#ifndef _MAZE_H_
#define _MAZE_H_

#include <vector>
#include <tuple>
#include <stack>
#include <map>
#include <set>
#include <list>
#include <array>
#include <iostream>
#include <stdlib.h>
#include "glm/glm.hpp"
#include "GLMComparison.hpp"

namespace backstage {

  typedef std::vector<std::array<glm::vec2, 2>> walls_t;
  typedef std::vector<std::vector<unsigned int>> maze_t;
  
  class Maze {
  private:
    maze_t _maze;
    walls_t _walls;
    glm::vec2 _start;
    glm::vec2 _end;
    size_t _width;
    size_t _length;
    std::stack<glm::vec2> _aStar;
    unsigned int countNeighbors(maze_t maze, size_t x, size_t y);
    maze_t generateEmptyMaze();
    void generateSeed();
    unsigned int ruleB3_1234Cell(maze_t maze, size_t i, size_t j);
    void ruleB3_1234Iteration();
    void generateCorridor();
    void eraseCorridor();
    void makeBorder();
    bool hasNeighbor(maze_t maze, size_t x, size_t y);
    void toWallsHorizontal();
    void toWallsVertical();
    void toWalls();
    void newPath(glm::vec2 start, size_t minDist=10);
    void assignStart();
    void assignEnd(size_t minDist=10);
    void assignEndUntilPath(size_t minDist=10);
    size_t manhattanDistance(glm::vec2 x, glm::vec2 y);
    void insertInPosition(glm::vec2 toInsert, size_t heuristicDist, std::list<glm::vec2> &toVisit, std::map<size_t, std::tuple<glm::vec2, size_t, size_t>> mapPoint);
    void expand(glm::vec2 toExpand, size_t x, size_t y, std::list<glm::vec2> &toVisit, std::map<size_t, std::tuple<glm::vec2, size_t, size_t>> &mapPoint, std::set<glm::vec2> visited);
    void expandAll(glm::vec2 toExpand, std::list<glm::vec2> &toVisit, std::map<size_t, std::tuple<glm::vec2, size_t, size_t>> &mapPoint, std::set<glm::vec2> visited);
    std::stack<glm::vec2> rebuildPath(std::map<size_t, std::tuple<glm::vec2, size_t, size_t>> mapPoint, std::stack<glm::vec2> acc, std::set<glm::vec2> acc2);
    void aStar();

  public:
    std::set<glm::vec2> _aStar2;
    Maze(size_t width=20,  size_t length=20, size_t minDist=10);
    walls_t getWalls() const;
    glm::vec2 getStart() const;
    glm::vec2 getEnd() const;
    size_t getWidth() const;
    size_t getLength() const;
    std::stack<glm::vec2> getAStar() const;
    void setStart(glm::vec2 start);
    void resetPathFromEnd(size_t minDist=10);
    std::vector<std::vector<unsigned int>> getMaze() const;
    void init(size_t minDist=10);
  };

  std::ostream& operator<<(std::ostream &os, const Maze &maze);
  
}



#endif /* _MAZE_H_ */
