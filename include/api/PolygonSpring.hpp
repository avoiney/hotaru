#ifndef _API_POLYGONSPRING_HPP_
#define _API_POLYGONSPRING_HPP_


#include <glm/glm.hpp>
#include "LeapfrogSolver.hpp"
#include "Particle.hpp"


using namespace glm;


bool intersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& A, const glm::vec2& B,
    glm::vec2* intersection, glm::vec2* normal);

class PolygonSpring: public Spring {
    // Génère une force s'il a collision avec le coté [v0,v1]
    bool generateForces(Particle* p, const glm::vec2& v0, const glm::vec2& v1, struct wave * wav) const {
        Particle next;
        m_Solver->getNextState(*p, m_fDt, next);
        glm::vec2 intersection, normal;
	
	// si intersection, ajout de la force (voir "Force de collision" plus haut)
        if(intersect(p->position, next.position, v0, v1, &intersection, &normal)) {
            float alpha = glm::dot(next.velocity, -normal) * p->mass / m_fDt;
            p->force += m_fElasticity * alpha * normal;
            double val = 0;
            int i = 1.5*cpt*(wav->ByteRate/itPerSec);
            int taille = i + wav->ByteRate/2;
            double fm = 0;
            if(taille >= wav->subTaille2)
                return true;
            if(p->note == 0) return false;
            for (i, val = 0.0; i < taille ; i += 2, val += p->note) {
                fm = val*(p->octave+octaveModifer);
                //wav->data[i+1] = wav->data[i] = (wav->data[i]/32267 + cos((fm/110)*sin(fm)))*32267;
                wav->data[i+1] = wav->data[i] = theFunFunction(wav->data[i], fm);
            }
            p->octave+=0.25;
        }
            return false;
    }
    public:
        PolygonSpring(const Polygon& polygon, float elasticity, const LeapfrogSolver& solver, float dt):
            m_Polygon(&polygon), m_fElasticity(elasticity), m_Solver(&solver), m_fDt(dt) {
        }

        bool generateForces(Particle* p1, Particle* p2, struct wave * wav) {
    	// si le polygone est interne, on parcourt les sommets dans l'ordre trigonométrique
            bool test = false;
            if(m_Polygon->isInner()) {
                for(int i = 0; i < m_Polygon->size(); ++i) {
                    test |= generateForces(p1, m_Polygon->getVertex(i), m_Polygon->getVertex((i + 1) % m_Polygon->size()), wav);
                }
            } 
    	// Sinon dans l'ordre contraire
    	   else {
                for(int i = m_Polygon->size() - 1; i >= 0; --i) {
                    test |= generateForces(p1, m_Polygon->getVertex(i), m_Polygon->getVertex((i - 1 + m_Polygon->size()) % m_Polygon->size()), wav);
                }
            }
            return test;
        }
    private:
        const Polygon* m_Polygon;
        float m_fElasticity;
        const LeapfrogSolver* m_Solver;
        float m_fDt;
};


bool intersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& A, const glm::vec2& B,
    glm::vec2* intersection, glm::vec2* normal) {
    static const float epsilon = 0.001; // Permet de tricher en allongeant la taille des coté

    glm::vec2 dir = p2 - p1; // Vecteur p1p2
    glm::vec2 AB = B - A; // Vecteur AB
    glm::vec2 N = glm::vec2(-AB.y, AB.x); // Vecteur normal non unitaire tel que (AB, N) forme un repère direct

    // Si le vecteur p1p2 et le vecteur N on la meme direction, il n'y a pas d'intersection
        if(glm::dot(dir, N) >= 0) {
            return false;
        }
        
    // Calcul du parametre t tel que I = p1 + t x p1p2
        float t = -glm::dot(N, p1 - A) / glm::dot(N, dir);
        
        // Si t n'est pas dans [0,1], le point d'intersection n'est pas dans le segment [p1,p2]
        if(t < 0.f || t > 1) {
            return false;
        }

    glm::vec2 I = p1 + t * dir; // Point d'intersection

    float dot = glm::dot(I - A, AB); // Produit scalaire entre le vecteur AI et le vecteur AB

    // Permet de savoir si le point d'intersection est sur le segment [A - epsilon, B + epsilon]
    if(dot < -epsilon || dot > glm::dot(AB, AB) + epsilon) {
        return false;
    }

    if(intersection) {
        *intersection = I;
    }

    if(normal) {
        *normal = glm::normalize(N); // On normalise le vecteur normal pour avoir un vecteur unitaire en sortie
    }
        
    return true;
}

#endif