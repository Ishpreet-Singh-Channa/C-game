#pragma once
#include<SFML/Graphics.hpp>
#include"Vec2.h"
class CTransform
{
public:

    Vec2    normal_vec; //  Angle
    float   speed;      //  Speed
    CTransform(){}
    CTransform(const Vec2 &n)
        :   normal_vec(n)
    {
        speed = Vec2::distance(normal_vec,Vec2(0.0,0.0));
        normal_vec.normalize();
    }
    CTransform(const Vec2 &n, float s)
        :   normal_vec(n)
        ,   speed(s)
    {
        normal_vec.normalize();
    }
};
class CCollision
{
public:
    float radius = 0.0;
    CCollision(float r)
        :   radius(r)
    {}
};
class CShape
{
public:
    float radius;
    sf::CircleShape circle;
    Vec2 position;
    CShape(const float r, const int point, const sf::Color &colour_fil, const sf::Color &colour_outline, const float thickness, const Vec2 &p)
        :   circle(r, point)
        ,   radius(r)
        ,   position(p)
    {
        circle.setFillColor(colour_fil);
        circle.setOutlineThickness(thickness);
        circle.setOutlineColor(colour_outline);
        circle.setOrigin(radius, radius);
        circle.setPosition(position.x, position.y);
    }
};
class CLifespan
{
public:
    float lifespan = 0;
    float original_lifespan = 0;
    CLifespan(const float frames)
        :   original_lifespan(frames)
        ,   lifespan(frames)
    {}
};
class CInput
{
public:
    bool up      = false;
    bool down    = false;
    bool left    = false;
    bool right   = false;
    bool shoot   = false;
    bool special = false;
    CInput()
    {}
};
class CAi
{
public:
    int shooting_interval = 0;
    int ticks = 0;
    CAi(int interval)
        :   shooting_interval(interval)
        ,   ticks(interval)
    {}
};
class CPoints
{
public:
    int points;
    CPoints():
        points(0)
    {}
    CPoints(int score):
        points(score)
    {}
};
//  S-221/58 Vishnu Garden