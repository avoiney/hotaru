#ifndef _API_CIRCLE_HPP_
#define _API_CIRCLE_HPP_

#include <glm/glm.hpp>



class Circle {
public:
    Circle(glm::vec2 c_position, float radius, float c_mass) : position(c_position), mass(c_mass), m_radius(radius) {};

    void build(Polygon& polygon) {
        float delta = 2 * 3.14 / 30; // 2pi / nombre de particules
        for(size_t i = 0; i < 30; ++i) {
        	float c = cos(i * delta), s = sin(i * delta);
	        polygon.addVertex(glm::vec2(position.x + m_radius * c, position.y + m_radius * s));
	    }
        
    }

    glm::vec2 position;
    float mass;
    float m_radius;
};

#endif