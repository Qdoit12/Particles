#ifndef MAIN_H
#define MAIN_H

#ifndef SFML_DEF
#define SFML_DEF
#include <SFML/Graphics.hpp>
#endif

struct WindowParameters {
    unsigned height;
    unsigned width;
};

struct WorldConstants {
    float gravity;
};

struct RenderParameters {
    unsigned maxNumParticles;
    unsigned maxNumEmitters;
    unsigned maxParticleSize;
};


struct physicsParticle
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    
    int size;
    float lifespan;
    float mass;
    
    float bounceFactor;
    unsigned char transparency;
    
    //visual
    
};

struct particleEmitter
{
    sf::Vector2f position;
    sf::Vector2f direction;
    float radiusSprite;
    float speed;
    
    float randomnessInDirection;
    
    float delay;
    float lifespan;
    float pMass;
    float bounceFactor;
    
    float lastTimeEmitted;
    float randomnessInSize;
    float randomnessInTransperency;
    
    unsigned minParticleSize;
    unsigned maxParticleSize;
    
    bool hovering;
    
};

struct EmitterCmp
{
    bool operator() (const physicsParticle& a, const physicsParticle& b) const
    {
        return (a.lifespan > b.lifespan);
    }
    
};

#endif

