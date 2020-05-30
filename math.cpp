#ifndef MATH_H
#define MATH_H
/*
 * =====================================================================================
 *
 *       Filename:  math.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30/05/2020 18:58:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jacob Korba (), 
 *   Organization:  
 *
 * =====================================================================================
 */

sf::Vector2f toNDC(const sf::Vector2f& v, WindowParameters windowParameters)
{
    float newX = 2 * v.x / windowParameters.width - 1;
    float newY = 2 * v.y / windowParameters.height - 1; newY *= -1;
    
    return sf::Vector2f(newX, newY);
}


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

#endif
