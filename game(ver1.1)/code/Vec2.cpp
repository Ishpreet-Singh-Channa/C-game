#include"./Header/Vec2.h"
Vec2::Vec2()
    :   x(0)
    ,   y(0)
{}
Vec2::Vec2(float a, float b)
    :   x(a)
    ,   y(b)
{}

const bool Vec2::operator == (const Vec2 &rhs) const
{
    if (x == rhs.x   &&  y == rhs.y)
        return  true;
    return false;
}
const bool Vec2::operator != (const Vec2 &rhs) const
{
    if (x == rhs.x  &&  y == rhs.y)
        return false;
    return true;
}

const Vec2 Vec2::operator + (const Vec2 &rhs) const
{
    return Vec2(x + rhs.x,  y + rhs.y);
}
const Vec2 Vec2::operator - (const Vec2 &rhs) const
{
    return Vec2(x - rhs.x,  y - rhs.y);
}
const Vec2 Vec2::operator * (const float rhs) const
{
    return Vec2(x * rhs,  y * rhs);
}
const Vec2 Vec2::operator / (const float rhs) const
{
    return Vec2(x / rhs,  y / rhs);
}

const Vec2 Vec2::operator +  (float rhs) const
{
    return Vec2( x+rhs, y+rhs);
}   
const Vec2 Vec2::operator -  (float rhs) const
{
    return Vec2( x-rhs, y-rhs);
}

void Vec2::operator += (const Vec2 &rhs)
{
    x = x+rhs.x;
    y = y+rhs.y;
}
void Vec2::operator -= (const Vec2 &rhs)
{
    x = x-rhs.x;
    y = y-rhs.y;
}
void Vec2::operator *= (const float rhs)
{
    x = x*rhs;
    y = y*rhs;
}
void Vec2::operator /= (const float rhs)
{
    x = x/rhs;
    y = y/rhs;
}

void Vec2::normalize()
{
    if (!(x==0 & y==0))
    {
        float d = sqrt( (x*x) + (y*y) );
        x/=d;
        y/=d;
    }
    else 
    {
        x = 0;
        y = 0;
    }
}
void Vec2::directionalize()
{
    if (x > 0 && y > 0)
    {
        x = 1;
        y = 1;
        this->normalize();
    }
    else if (x > 0 && y < 0)
    {
        x = 1;
        y = -1;
        this->normalize();
    }
    else if (x < 0 && y > 0)
    {
        x = -1;
        y = 1;
        this->normalize();
    }
    else if (x < 0 && y < 0)
    {
        x = -1;
        y = -1;
        this->normalize();
    }
    else if( x != 0 ^ y != 0)
    {
        this->normalize();
    }
}

const Vec2 Vec2::add_angle(float angle)
{
    normalize();
    float radian = angle * (3.141592 / 180);
    float sin_val = sin(radian);
    float cos_val = cos(radian);
    return Vec2(x*cos_val - y*sin_val , x*sin_val + y*cos_val);
}

const float Vec2::distance      (const Vec2 &rhs) const
{
    return sqrt( (x-rhs.x)*(x-rhs.x) + (y-rhs.y)*(y-rhs.y) );
}
const float Vec2::distance_sq   (const Vec2 &rhs) const
{
    return ( (x-rhs.x)*(x-rhs.x) + (y-rhs.y)*(y-rhs.y) );
}
const float Vec2::angle         ()
{
    float a = atan2(y, x)*180/3.141592;
    if (a<0)
    {
        a = 360+a;
    }
    return a;
}
const float Vec2::magnitude     ()
{
    return this->distance(Vec2(0,0));
}

//  Static
const float Vec2::distance      (const Vec2 &v1, const Vec2 &v2)
{
    return sqrt( (v1.x-v2.x)*(v1.x-v2.x) + (v1.y-v2.y)*(v1.y-v2.y) ); 
}
const float Vec2::distance_sq   (const Vec2 &v1, const Vec2 &v2)
{
    return ( (v1.x-v2.x)*(v1.x-v2.x) + (v1.y-v2.y)*(v1.y-v2.y) ); 
}
const float Vec2::angle         (const Vec2 &obj)
{
    float a = atan2(obj.y, obj.x)*180/3.141592;
    if (a<0)
    {
        a = 360+a;
    }
    return a;
}
const float Vec2::magnitude     (const Vec2 &obj)
{
    return obj.distance(Vec2(0,0));
}