#include "glm/glm.hpp"

namespace glm {
  bool operator<(glm::vec2 vec1, glm::vec2 vec2) {
    return (glm::lessThan(vec1, vec2)[0] || glm::lessThan(vec1, vec2)[1]);
  }

  bool operator>(glm::vec2 vec1, glm::vec2 vec2) {
    return (glm::greaterThan(vec1, vec2)[0] || glm::greaterThan(vec1, vec2)[0]);
  }

  bool operator<=(glm::vec2 vec1, glm::vec2 vec2) {
    return (glm::lessThanEqual(vec1, vec2)[0] || glm::lessThanEqual(vec1, vec2)[1]);
  }

  bool operator>=(glm::vec2 vec1, glm::vec2 vec2) {
    return (glm::greaterThanEqual(vec1, vec2)[0] || glm::lessThanEqual(vec1, vec2)[1]);
  }
}
