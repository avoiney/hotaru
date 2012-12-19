#include <iostream>
#include <vector>
#include <cmath>
#include <SDL/SDL.h>
#include <ctime>

int cpt = 0;
const int itPerSec = 500;

static const size_t WINDOW_WIDTH = 512;
static const size_t WINDOW_HEIGHT = 512;
static const size_t BYTES_PER_PIXEL = 32;
static const char* WINDOW_TITLE = "Hotaru No Ongaku";
static int NB_PARTICLE = 8;
static int N_SEC = 15;
static double octaveModifer = 0;
double FMmodulation(int16_t data, double fm);
double basicNote(int16_t data, double fm);
double(*theFunFunction)(int16_t data, double fm) = basicNote;

#include "api/wave.h"
#include "api/Particle.hpp"
#include "api/Spring.hpp"
#include "api/LeapfrogSolver.hpp"
#include "api/Polygon.hpp"
#include "api/PolygonSpring.hpp"
#include "api/ConstantSpring.hpp"
#include "api/HookSpring.hpp"
#include "api/FluidSpring.hpp"
#include "api/ViscuoseSpring.hpp"
#include "api/CineticBrake.hpp"
#include "api/AttractiveSpring.hpp"
#include "api/Box.hpp"
#include "api/Circle.hpp"
#include "renderer/GLRenderer.hpp"

#define DO      0.037275246167682
#define RE      0.041840030159809
#define MI      0.046963889416093
#define FA      0.049756558695141
#define SOL     0.055849823684532
#define LA      0.062689377214490
#define SI      0.070366403833691
#define DO_1    0.074550634811044
#define DTMAX   0.00001

using namespace std;


int main(int argc, char** argv) {
    
    // demande d'options
    char name[30];
    char cercle_c;
    char cercle_g;
    char gravite_c;
    char part_c;
    char function_c;
    std::cout << "File Name" << std::endl;
    std::cin >> name ;
    std::cout << "Musique Length (s)" << std::endl;
    std::cin >> N_SEC ;
    std::cout << "Particles Number" << std::endl;
    std::cin >> NB_PARTICLE ;
    std::cout << "Do want to enable the cricle ? (y/n):" << std::endl;
    std::cin >> cercle_c ;
    bool circle = (cercle_c == 'y')? true : false;
    bool activeCircleGravitation = false;
    if(circle){
        std::cout << "With gravitation ? (y/n):" << std::endl;
        std::cin >> cercle_g ;
        activeCircleGravitation = (cercle_g == 'y')? true : false;
    }
    std::cout << "Do to enable particles attraction ? (y/n):" << std::endl;
    std::cin >> part_c ;
    bool activePartGrav = (part_c == 'y')? true : false;
    std::cout << "Do you want to enable gravity? (y/n):" << std::endl;
    std::cin >> gravite_c ;
    bool activeGravity = (gravite_c == 'y')? true : false;
    std::cout << "Do you want to enable frequences modulation ? (y/n)" << std::endl;
    std::cin >> function_c ;
    bool FMmodulation_b = (function_c == 'y')? true : false;
    if(FMmodulation_b) theFunFunction = &FMmodulation;

    struct wave wav = { 0 };
    FILE *fich = fopen(strcat(name,".wav"), "wb+");
    
    if (fich == NULL)
        return EXIT_FAILURE;
    
    /* Seul les paramètres taille, subTaille2 et data ne sont pas initialisés */
    initWave(&wav);

    // Initialisation de la SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BYTES_PER_PIXEL, SDL_OPENGL);
    SDL_WM_SetCaption(WINDOW_TITLE, 0);
    static float dt = DTMAX; // Pas de simulation, choix arbitraire mais dois rester petit pour que le systeme n'explose pas;

    // Creation du renderer
    GLRenderer renderer;

    /** Placez votre code d'initialisation de la simulation ici **/
    float gravity_constant = -981.0;
    if(!activeGravity) gravity_constant = 0;
    ConstantSpring gravity(glm::vec2(0, gravity_constant)); // pointe vers le bas, 3 est un choix arbitraire
    ViscuoseSpring eau(1.002 * 10);
    FluidSpring fluidSpring(dt, 0.00000000000000000000001, 80000);
    AttractiveSpring gravitation(100, 1.2);
    float Gravitation_constant = 100;
    if(!activePartGrav) Gravitation_constant = 0;
    AttractiveSpring gravitationParticule(Gravitation_constant, 1.2);
    LeapfrogSolver solver;

    std::vector<Polygon> obstacles;
    
    // Content Box
    obstacles.push_back(Polygon(Col3f(0.1,.1,.2), true)); // polygone interne
    Box::build(glm::vec2(-0.9, -0.9), 1.8, 1.8, obstacles.back());

    obstacles.push_back(Polygon(Col3f(0.1,.1,.2), false)); // polygone externe
    float circle_mass = 0.1;
    if(!activeCircleGravitation or !circle) circle_mass = 0;
    Circle c(glm::vec2(0.,0.), 0.1, circle_mass);
    if(circle) c.build(obstacles.back());

    for(size_t i = 0; i < obstacles.size(); ++i) {
        renderer.addPolygon(&obstacles[i].getVertices()[0], obstacles[i].size(), obstacles[i].color);
    }



    // Code d'exemple pour le rendu: des particles placées en cercle
    std::vector<Particle> particles;

    float wallElasticity = 2.; // Les particules vont tomber après quelques rebonds
    // Creation des ressort de collision
    std::vector<PolygonSpring> polygonSprings;
    for(size_t i = 0; i < obstacles.size(); ++i) {
            polygonSprings.push_back(PolygonSpring(obstacles[i], wallElasticity, solver, dt));
    }

    float delta = 2 * 3.14 / NB_PARTICLE; // 2pi / nombre de particules
    double notes[13] = {DO, DO + DO/2, RE, RE + RE/2, MI, FA, FA + FA/2, SOL, SOL + SOL/2, LA, LA + LA/2, SI, DO_1};
    for(size_t i = 0, j = 0 ; i < NB_PARTICLE; ++i, ++j) {
        double note;
        if(j == 13) j = 0;
        note = notes[j];
        //else j = 0;
        float c = cos(i * delta), s = sin(i * delta);
        float r3 = -.7 + (float)rand()/((float)RAND_MAX/(1.4));
        float r4 = .1 + (float)rand()/((float)RAND_MAX/(.6));
        particles.push_back(Particle(glm::vec2(r3, 0.5*s), glm::vec2(0,0), glm::vec3(c, 1, 1), float(r4), note));
    }


    
    double val = 0;
    bool done = false;
    
    while(!done) {
        // Rendu
        renderer.render(&particles[0], particles.size());
        SDL_GL_SwapBuffers();

        /** Placez votre code de simulation ici */
        //Application de la gravité sur chaque particule:
        for(size_t i = 0; i < particles.size(); ++i) {
            gravity.generateForces(&particles[i], 0);
            gravitation.generateForces(&particles[i], &c);
        }

        for(size_t i = 0 ; i < particles.size() ; ++i) {
            for(size_t j = i+1 ; j < particles.size() ; ++j) {
                //fluidSpring.generateForces(&particles[i], &particles[j]);
                gravitationParticule.generateForces(&particles[i], &particles[j]);
            }
        }

        for(size_t i = 0; i < particles.size() ; ++i) { // Pour chaque particule
            for(size_t j = 0; j < polygonSprings.size(); ++j) { // Pour chaque ressort de collision
                done |= polygonSprings[j].generateForces(&particles[i], 0, &wav); // Test de collision et génération des forces
            }
        }  


        for(size_t i = 0; i < particles.size() ; ++i) {
            solver.solve(dt, particles[i]);
        }

    

        /** Placez votre gestion des évenements ici */
        SDL_Event e;
        static float dt_tmp;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN) {
                SDLKey keyText = e.key.keysym.sym;
                
                if(keyText == SDLK_ESCAPE)
                    done = true;

                if(keyText == SDLK_RIGHT){
                    if(dt < DTMAX)
                        dt += DTMAX/10;
                    std::cout << "Pas de simulation : " << dt << std::endl;
                }

                if(keyText == SDLK_LEFT){
                    if(dt > .0)
                        dt -= DTMAX/10;
                    std::cout << "Pas de simulation : " << dt << std::endl;
                }

                if(keyText == SDLK_UP){
                    if(octaveModifer < 3)
                        octaveModifer += 0.25;
                    std::cout << "Modificateur d'octave : " << octaveModifer << std::endl;
                }

                if(keyText == SDLK_DOWN){
                    if(octaveModifer > .0)
                        octaveModifer -= 0.25;
                    std::cout << "Modificateur d'octave : " << octaveModifer << std::endl;
                }
                    
                

                break;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                if(e.button.button == SDL_BUTTON_LEFT && dt == 0)
                    dt = dt_tmp;
                else if(e.button.button == SDL_BUTTON_LEFT && dt != 0) {
                    dt_tmp = dt;
                    dt = 0;
                }
                break;
            }

            if(e.type == SDL_QUIT) {
                done = true;
                break;
            }

            
        }
        cpt++;
    }

    SDL_Quit();
    std::cout << "Fin de la simulation" << std::endl << std::endl;

    std::cout << "########## Ecriture du fichier ##############" << std::endl;
    std::cout << name << ".wav" << std::endl;
    fwrite(&wav, T_ENTETE, 1, fich);
    /* On écris les data */
    fwrite(wav.data, wav.subTaille2, 1, fich);
    std::cout << "########## Ecriture finie ##############" << std::endl;
    /* On libère notre mémoire, et on ferme le fichier */
    free(wav.data);
    fclose(fich);
    return 0;
}



double FMmodulation(int16_t data, double fm) {
    return (data/32267 + cos((fm/110)*sin(fm)))*32267;
}

double basicNote(int16_t data, double fm) {
    return (data/32267) + sin(fm)*32267;
}