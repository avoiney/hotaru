#ifndef _API_CINETICBRAKE_HPP_
#define _API_CINETICBRAKE_HPP_

#include <glm/glm.hpp>


class CineticBrake: public Spring {
public:
    CineticBrake(float c_dt, float c_z) : m_dt(c_dt), m_z(c_z){}

    bool generateForces(Particle* p1, Particle* p2, struct wave * wav = NULL) {
        glm::vec2 v = p2->velocity - p1->velocity;
        p2->force += (m_z/m_dt)*v;
        p1->force -= (m_z/m_dt)*v;

        return false;
    } 
private:
    float m_dt;
    float m_z;
};

#endif