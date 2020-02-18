/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the MIT License. 
 * If a copy of the ML was not distributed with this
 * file, You can obtain one at https://opensource.org/licenses/MIT */

/* author jackred@tuta.io */

#ifndef _MAZE_H_
#define _MAZE_H_

#include <vector>
#include <array>
#include <iostream>
#include <stdlib.h>

#include "glm/glm.hpp"

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
      
  public:
    Maze(size_t width=20,  size_t length=20);
    walls_t getWalls() const;
    glm::vec2 getStart() const;
    glm::vec2 getEnd() const;
    size_t getWidth() const;
    size_t getLength() const;
    std::vector<std::vector<unsigned int>> getMaze() const;
  };

  std::ostream& operator<<(std::ostream &os, const Maze &maze);
  
}



#endif /* _MAZE_H_ */
