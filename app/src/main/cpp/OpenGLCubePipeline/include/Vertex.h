#ifndef VERTEX_H
#define VERTEX_H

#include "../../ThirdParty/glm/glm/glm.hpp"
#include "../../ThirdParty/glm/glm/gtc/quaternion.hpp"
#include "../../ThirdParty/glm/glm/gtc/matrix_transform.hpp"
#include <cstdint>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

#endif //!VERTEX_H