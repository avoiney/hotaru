#ifndef _API_BOX_HPP_
#define _API_BOX_HPP_

#include <glm/glm.hpp>



class Box {
public:
    static void build(const glm::vec2& position, float width, float height, Polygon& polygon) {
        polygon.addVertex(position);
        polygon.addVertex(position + glm::vec2(width, 0));
        polygon.addVertex(position + glm::vec2(width, height));
        polygon.addVertex(position + glm::vec2(0, height));
    }
};

#endif