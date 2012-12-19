#ifndef _API_HOOKSPRING_HPP_
#define _API_HOOKSPRING_HPP_


#include <glm/glm.hpp>
#include "Particle.hpp"
#define EPS 0.0001

using namespace glm;

class HookSpring: public Spring {
    public:
        HookSpring(float c_K, float c_lo) : K(c_K), lo(c_lo) {}

        bool generateForces(Particle* p1, Particle* p2, struct wave * wav = NULL) {
            glm::vec2 AB = p2->position - p1->position;
            float e = 1 - lo/(glm::max(float(EPS), glm::distance(p1->position,p2->position)));
            p1->force += K*e*AB;
            p2->force -= K*e*AB;

            return false;
        }

    private:
        float K;
        float lo;       
};

#endif