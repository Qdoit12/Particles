// MAIN ON 300

//TODO(JK): Epsilon distance;
//TODO(JK) Window Scaling
//TODO(JK): Engine GUI

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <set>
#include <climits>
#include <cassert>
//#include <random>

#define COUT(x) std::cout<<#x<<": "<<x<<std::endl;


const float EPSILON = 0.005f;


bool resized;

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

sf::Vector2f toNDC(const sf::Vector2f& v, WindowParameters windowParameters)
{
    float newX = 2 * v.x / windowParameters.width - 1;
    float newY = 2 * v.y / windowParameters.height - 1; newY *= -1;
    
    return sf::Vector2f(newX, newY);
}

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


sf::Vector2f toSFML(const sf::Vector2f& v, WindowParameters windowParameters)
{
    float newX = v.x*windowParameters.width/2 + windowParameters.width/2;
    float newY = -(v.y * windowParameters.height/2 - windowParameters.height/2);
    return sf::Vector2f(newX, newY);
}

float lenSq(const sf::Vector2f v)
{
    return (v.x * v.x + v.y * v.y);
}

float length(const sf::Vector2f& a)
{
    return sqrt(lenSq(a));
}

float rand01()
{
    return ((float)rand()/INT_MAX);
}
float randN1to1()
{
    return (2.0f * rand01() - 1);
}

sf::Vector2f randVec2()
{
    
    
    sf::Vector2f result(randN1to1(), randN1to1());
    
    
    result = result / length(result);
    return result;
}

template<typename T>
T lerp (const T& a, const T& b, const float inter)
{
    return ((1 - inter) * a + inter * b);
}


void emit(physicsParticle* particles, particleEmitter& e, RenderParameters renderParm, WorldConstants worldConst)
{
    physicsParticle newParticle;
    
   newParticle.position = e.position;
    newParticle.velocity = e.speed * lerp<sf::Vector2f>(e.direction, randVec2(), e.randomnessInDirection);
    newParticle.mass = e.pMass;
    newParticle.acceleration = newParticle.mass * sf::Vector2f(0,worldConst.gravity);
    newParticle.lifespan = e.lifespan;
    newParticle.bounceFactor = e.bounceFactor;
    assert(e.randomnessInSize <= 1.0 && e.randomnessInSize >= 0.0);
    newParticle.size = (unsigned)(rand01() * (1.0f - e.randomnessInSize) * e.maxParticleSize) + e.minParticleSize;
    newParticle.transparency = (unsigned char)(rand01() * (1.0f - e.randomnessInTransperency) * 255);
    
    
    
    for(int i = 0; i < renderParm.maxNumParticles; ++i)
    {
        auto& curPart = particles[i];
        
        if(curPart.lifespan <= 0)
        {
            curPart = newParticle;
            break;
        }
    }
}

float absol_V(float a)
{
    return a < EPSILON ? -a : a;
}

/*void update(float dt, physicsParticle* particles, particleEmitter& e)
{
    e.lastTimeEmitted += dt;
    if(e.lastTimeEmitted > e.delay)
    {
        emit(particles, e);
        e.lastTimeEmitted = 0;
    }
    for(int i = 0; i < renderParm.maxNumParticles; i++)
    {
        physicsParticle& curPart = particles[i];
        
        float bounceFactor = 0.7;
        bool bounced = false;
        
        curPart.velocity += dt * curPart.acceleration;
        auto nextPos = curPart.position + dt * curPart.velocity;
        
        if(isWithinEpsilonDistance(nextPos.y, -1.0f) || isWithinEpsilonDistance( nextPos.y, 1.0f))
        {
            curPart.velocity.y = -curPart.velocity.y;
            bounced = true;
        }
        if(isWithinEpsilonDistance(nextPos.x, -1.0f) || isWithinEpsilonDistance( nextPos.x, 1.0f))
        {
            curPart.velocity.x = -curPart.velocity.x;
            bounced = true;
        }
        
        if(bounced)
        {
            std::cout << "BBOUNCED" << std::endl;
            if(lenSq(curPart.velocity) < 0.01);
            {
                curPart.velocity = {};
                curPart.acceleration = {};
            }
            curPart.velocity =  curPart.velocity * bounceFactor;
        }
        
        
        curPart.position += dt * curPart.velocity;
        
        curPart.lifespan -= dt;
        
        
    }
    

}*/

void update(float dt, physicsParticle* particles, particleEmitter* emitters, RenderParameters renderParm, WindowParameters windowParameters, WorldConstants worldConst)
{
    
    for(int i = 0; i < renderParm.maxNumEmitters; ++i)
    {
        
        auto& e = emitters[i];
        if(resized)
        {
            
        }
        e.lastTimeEmitted += dt;

        if (e.lastTimeEmitted > e.delay)
        {
            emit(particles, e, renderParm, worldConst);
            e.lastTimeEmitted = 0;
        }
    }
    

    for (int i = 0; i < renderParm.maxNumParticles; ++i)
    {
        auto& curPart = particles[i];

        float bounceFactor = 0.7f;
        bool bounced = false;

        curPart.velocity += dt * curPart.acceleration; // 0.5f * at^2

        auto nextPos = curPart.position + dt * curPart.velocity;

        if (nextPos.y < -1.0f || 
             nextPos.y > 1.0f)
        {
            curPart.velocity.y = -curPart.velocity.y;
            bounced = true;
        }
        if (nextPos.x < -1.0f ||
             nextPos.x >  1.0f)
        {
            curPart.velocity.x = -curPart.velocity.x;
            bounced = true;
        }

        if (bounced)
        {
            curPart.velocity = lerp<sf::Vector2f>(curPart.velocity, randVec2(), 1.0f - curPart.bounceFactor);
            if (lenSq(curPart.velocity) < 0.01)
            {
                curPart.velocity = {};
                curPart.acceleration = {};
            }
            curPart.velocity = curPart.velocity * curPart.bounceFactor;
        }



        curPart.position += dt * curPart.velocity;

        curPart.lifespan -= dt;
    }
    if(resized)
    {
        resized = 0;
    }
}





int main(int, char const**)
{
    
    //Create window constraints
    WindowParameters windowParameters;
    windowParameters.width = 1920;
    windowParameters.height = 1080;
    
    WorldConstants worldConst;
    worldConst.gravity = -9.6;
    
   RenderParameters renderParm;
    renderParm.maxNumParticles = 1000;
    renderParm.maxNumEmitters = 3;
    renderParm.maxParticleSize = 40; 

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(windowParameters.width, windowParameters.height), "Particle's!");
    
    sf::Clock clock;
    float elapsed;
    
    physicsParticle* particles = (physicsParticle*)malloc(sizeof(physicsParticle) * renderParm.maxNumParticles);
    
    particleEmitter* emitters = (particleEmitter*)malloc(sizeof(particleEmitter) * 5);
    
    
    srand(13333); 
    
    for(int i = 0; i < renderParm.maxNumEmitters; i++)
    {
        auto& emitter = emitters[i];
        
        emitter.radiusSprite = 20;
        emitter.position = sf::Vector2f(randN1to1(), randN1to1());
        emitter.direction = sf::Vector2f(0.0f, 1.0f);
        emitter.speed = 3;
        emitter.delay = 0.01;
        emitter.lastTimeEmitted = 0;
        emitter.randomnessInDirection = 0.08;
        emitter.randomnessInTransperency = 0.1;
        emitter.randomnessInSize = 1.0;
        emitter.lifespan = 1;
        emitter.pMass = 0.6;
        emitter.hovering = 0;
        emitter.bounceFactor = 0.1;
        emitter.minParticleSize = 20;
        emitter.maxParticleSize = 40;
    }
    
    
    
    bool grabbing = false;
    
    // Start the game loop
    while (window.isOpen())
    {
        double dt = clock.restart().asSeconds(); //dt
        elapsed += dt;
        
        //windowParameters.height = window.getSize().y;
        //windowParameters.width = window.getSize().x;
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            /*if(event.type == sf::Event::Resized)
            {
                windowParameters.width = window.getSize().x;
                windowParameters.height = window.getSize().y;
                
                sf::View v;
                v.setSize(windowParameters.width, windowParameters.height);
                window.setView(v);
            }*/
        }
        
        
        auto msPos = sf::Mouse::getPosition(window);
        
        auto mousePosition = toNDC((sf::Vector2f)msPos, windowParameters); bool clicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        
        int whichEmitterPicked = -1; // which emitter picked
        float minDist = 100;
       
        for(int i = 0; i < renderParm.maxNumEmitters; ++i)
        {
            auto& emitter = emitters[i];
            
            auto len = length(mousePosition - emitter.position);
            
            if(len < minDist)
            {
                minDist = len;
                whichEmitterPicked = i;
            }
        }
        
       
        if(minDist < 2.0f * emitters[whichEmitterPicked].radiusSprite / windowParameters.width) {
            emitters[whichEmitterPicked].hovering = 1;
            if(clicked)
            {
                grabbing = 1;
            } else {
                grabbing = 0;
            }
        } else {
            emitters[whichEmitterPicked].hovering = 0;
        }
        if(grabbing)
        {
            emitters[whichEmitterPicked].position = mousePosition;
        }
        
        update(dt, particles, emitters, renderParm, windowParameters, worldConst);
        
        // Clear screen
        window.clear();
        
        sf::CircleShape visible;
        
        visible.setRadius(500);
        visible.setOrigin(500, 500);
        visible.setPosition(toSFML({0, 0}, windowParameters));
        visible.setFillColor({0, 50, 0});
        window.draw(visible);
        
        visible.setRadius(10);
        visible.setOrigin(10, 10);
        
        for(int i = 0; i < renderParm.maxNumParticles; i++)
        {
            physicsParticle& curPart = particles[i];
            
            
            visible.setFillColor({0, 255, 255, (unsigned char)curPart.transparency});
            //COUT(curPart.size)
            visible.setRadius(curPart.size);
            
            if(curPart.lifespan > 0)
            {
                visible.setPosition(toSFML(curPart.position, windowParameters));
                
                window.draw(visible);
            }
                

        }
        
        visible.setFillColor({125, 0, 0});
        for(int i = 0; i < renderParm.maxNumEmitters; ++i)
        {
            auto& emitter = emitters[i];
            visible.setFillColor({125, 0, 0});
            visible.setRadius(emitter.radiusSprite);
            if(emitter.hovering)
            {
                visible.setFillColor({255, 0, 0});
            }
            visible.setOrigin({emitter.radiusSprite / 2, emitter.radiusSprite / 2});
            visible.setPosition(toSFML(emitter.position, windowParameters));
            
            window.draw(visible);

        }
        
        // Update the window
        window.display();
    }
    free(particles);
    free(emitters);
    return EXIT_SUCCESS;
}
