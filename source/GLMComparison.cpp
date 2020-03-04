#include "GLMComparison.hpp"

bool glm::operator<(glm::vec2 vec1, glm::vec2 vec2) {
  if (vec1[0] == vec2[0]) {
    return (vec1[1] < vec2[1]);
  } else {
    return (vec1[0] < vec2[0]);
  }
}

bool glm::operator>(glm::vec2 vec1, glm::vec2 vec2) {
  if (vec1[0] == vec2[0]) {
    return (vec1[1] > vec2[1]);
  } else {
    return (vec1[0] > vec2[0]);
  }
}

bool glm::operator<=(glm::vec2 vec1, glm::vec2 vec2) {
  return (glm::lessThanEqual(vec1, vec2)[0]);
}

bool glm::operator>=(glm::vec2 vec1, glm::vec2 vec2) {
  return (glm::greaterThanEqual(vec1, vec2)[0]);
}
