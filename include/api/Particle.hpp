#ifndef _API_PARTICLE_HPP_
#define _API_PARTICLE_HPP_

#include <glm/glm.hpp>
#define DO      0.037275246167682
#define RE      0.041840030159809
#define MI      0.046963889416093
#define FA      0.049756558695141
#define SOL     0.055849823684532
#define LA      0.062689377214490
#define SI      0.070366403833691
#define DO_1    0.074550634811044

class Particle {
public:
	Particle(glm::vec2 c_position, glm::vec2 c_velocity, glm::vec3 c_color, float c_mass, double c_note) : position(c_position), velocity(c_velocity), color(c_color), mass(c_mass), force(0,0), note(c_note), octave(1)  {}
	Particle() : position(0,0), velocity(0,0), color(1.0,1.0,1.0), mass(0), force(0,0), note(LA), octave(1)  {}

	void resetForces() {
		glm::vec2 res(0,0);
		force = res;
	}


    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec3 color;
    float mass;
    glm::vec2 force;
    double note;
    int octave;
};

#endif
