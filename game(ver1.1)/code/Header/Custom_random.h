#pragma once
#include<iostream>
#include<cstdlib>
#include<ctime>
class Custom_random
{
public:
    Custom_random(){
        srand(time(0));
    }
    static int int_random(int min, int max)
    {
        int i = (rand()%(max - min)) + min;
        if (i > max)
        {
            i = max;
        }
        return i;
    }
    ~Custom_random(){}
};