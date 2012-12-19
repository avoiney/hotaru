#ifndef _API_CONSTANTSPRING_HPP_
#define _API_CONSTANTSPRING_HPP_

#include <glm/glm.hpp>


class ConstantSpring: public Spring {
public:
    ConstantSpring(const glm::vec2& force):
        m_Force(force) {
    }

    bool generateForces(Particle* p1, Particle* p2, struct wave * wav = NULL) {
        p1->force += m_Force;

        return false;
    }
private:
    glm::vec2 m_Force;
};

#endif