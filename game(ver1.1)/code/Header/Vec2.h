#pragma once
#include<math.h>
class Vec2
{
public:

    float   x   =   0.0;
    float   y   =   0.0;

    Vec2    ();
    Vec2    (float a,   float b);

    const bool    operator == (const Vec2 &rhs) const;
    const bool    operator != (const Vec2 &rhs) const;

    const Vec2    operator +  (const Vec2 &rhs) const;
    const Vec2    operator -  (const Vec2 &rhs) const;   
    const Vec2    operator *  (const float rhs) const;
    const Vec2    operator /  (const float rhs) const;

    const Vec2    operator +  (float rhs) const;
    const Vec2    operator -  (float rhs) const;   
    
    void    operator += (const Vec2 &rhs);
    void    operator -= (const Vec2 &rhs);
    void    operator *= (const float rhs);
    void    operator /= (const float rhs);

    void    normalize       ();
    void    directionalize  ();
    
    const Vec2  add_angle(float angle);
    
    const float distance            (const Vec2 &rhs) const;
    const float distance_sq         (const Vec2 &rhs) const;
    const float angle       ();
    const float magnitude   ();

    //  Static Members
    const static float distance         (const Vec2 &v1, const Vec2 &v2);
    const static float distance_sq      (const Vec2 &v1, const Vec2 &v2);
    const static float angle            (const Vec2 &obj);
    const static float magnitude        (const Vec2 &obj);
};