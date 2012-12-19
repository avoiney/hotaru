#ifndef _API_FLUIDSPRING_HPP_
#define _API_FLUIDSPRING_HPP_

#include <glm/glm.hpp>
#define EPS 0.0001

class FluidSpring: public Spring {
public:
		FluidSpring(float c_dt, float c_z, float c_K) : m_dt(c_dt), m_z(c_z), K(c_K) {}

		bool generateForces(Particle* p1, Particle* p2, struct wave * wav = NULL) {
			float lo =  0.03*(p1->mass + p2->mass);
			glm::vec2 AB = p2->position - p1->position;
			float norm = sqrt(AB.x*AB.x*AB.y*AB.y);
			if(norm > lo + 0.000001) return false;
        	float e = 1 - lo/(glm::max(float(EPS), glm::distance(p1->position,p2->position)));
        	p1->force += K*e*AB;
        	p2->force -= K*e*AB;

        	glm::vec2 v = p2->velocity - p1->velocity;
          p2->force += (m_z/m_dt)*v;
	        p1->force -= (m_z/m_dt)*v;

          return false;
    	}
   	private:
   		float m_dt;
   		float m_z;
   		float K;
};

#endif

// Celui de Laurent : Hook + Cinetic Brak
// Lo = 0.03*(p1.mass + p2.mass)
// Si d > Lo on return;