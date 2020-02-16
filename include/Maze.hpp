/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the MIT License. 
 * If a copy of the ML was not distributed with this
 * file, You can obtain one at https://opensource.org/licenses/MIT */

/* author jackred@tuta.io */

class
#ifndef _MAZE_H_
#define _MAZE_H_

#include <vector>
#include <iostream>
#include "glm/glm.hpp"

namespace backstage {

  class Maze {
  private:
    std::vector<std::array<glm::vec2> 4> walls;
    glm::vec2 start;
    glm::vec2 end;

  public:
    Maze(unsigned int with, unsigned int length);
    std::vector<glm::vec4> getWalls() const;
    glm::vec2 getStart() const;
    glm::vec2 getEnd() const;
  };

}

#endif /* _MAZE_H_ */
