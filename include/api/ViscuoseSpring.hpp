#ifndef _API_VISCUOSESPRING_HPP_
#define _API_VISCUOSESPRING_HPP_

#include <glm/glm.hpp>


class ViscuoseSpring: public Spring {
public:
    ViscuoseSpring(const float coeff):
        fluidityCoeff(coeff) {}

    bool generateForces(Particle* p1, Particle* p2, struct wave * wav = NULL) {
        p1->force -= fluidityCoeff*p1->velocity;
        return false;
    }
private:
    float fluidityCoeff;
};

#endif

// Celui de Laurent : Hook + Cinetic Brak
// Lo = 0.03*(p1.mass + p2.mass)
// Si d > Lo on return;