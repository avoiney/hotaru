#ifndef _API_LEAPFROGSOLVER_HPP_
#define _API_LEAPFROGSOLVER_HPP_

#include <glm/glm.hpp>

class LeapfrogSolver {
	public:
		void solve(float step, Particle& p) {
			p.velocity += step*p.force/p.mass;
			p.position += step*p.velocity;
			p.resetForces();
		}

		void getNextState(const Particle& particle, float dt, Particle& next) const {
		    next.velocity = particle.velocity + dt * particle.force / particle.mass;
		    next.position = particle.position + dt * next.velocity;
		}
};

#endif