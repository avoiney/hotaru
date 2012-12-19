#ifndef _API_SPRING_HPP_
#define _API_SPRING_HPP_

#include <glm/glm.hpp>

class Spring {
	public:
		virtual ~Spring() {}

		//virtual bool generateForces(Particle * p1, Particle * p2) = 0 ;
		virtual bool generateForces(Particle * p1, Particle * p2, struct wave * wav = NULL) = 0 ;
};

class NullSpring : public Spring {
	public:
		bool generateForces(Particle * p1, Particle * p2, struct wave * wav = NULL) {return false;}
};

class GravitySpring : public Spring {
	public:
		GravitySpring(glm::vec2 gravity) : g(gravity) {}

		bool generateForces(Particle * p1, Particle * p2, struct wave * wav = NULL) {
			p1->force -= p1->mass*g;

			return false;
		}

		glm::vec2 g;
};

#endif