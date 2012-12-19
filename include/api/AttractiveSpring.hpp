#ifndef _API_ATTRACTIVESPRING_HPP_
#define _API_ATTRACTIVESPRING_HPP_


#include <glm/glm.hpp>
#include "Particle.hpp"
#include "Circle.hpp"
#define EPS 0.1

using namespace glm;

class AttractiveSpring: public Spring {
    public:
        AttractiveSpring(float c_G, float c_dmin) : G(c_G), dmin(c_dmin) {}

        bool generateForces(Particle* p1, Particle* p2, struct wave * wav = NULL) {
            vec2 AB = p2->position - p1->position;
            float norm = sqrt(AB.x*AB.x + AB.y*AB.y);
        
            if(norm < dmin) {
                vec2 uAB = AB/norm;
                vec2 F = uAB*(G*(10*p1->mass*p2->mass)/max(float(EPS),(norm*norm)));
        
                p1->force += F;
                p2->force -= F;
            }

            return false;
        }

        bool generateForces(Particle* p1, Circle* pol, struct wave * wav = NULL) {
            vec2 AB = pol->position - p1->position;
            float norm = sqrt(AB.x*AB.x + AB.y*AB.y);
        
            if(norm < dmin) {
                vec2 uAB = AB/norm;
                vec2 F = uAB*(G*(10*p1->mass*pol->mass)/max(float(EPS),(norm*norm)));
        
                p1->force += F;
            }

            return false;
        }

    private:
        float G;
        float dmin;    
};

#endif