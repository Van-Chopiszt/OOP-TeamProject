#ifndef COMMAND_H
#define COMMAND_H

#include "Point.h"
#include "Chip.h"
#include "stepinfo.h"
#include<vector>
/*
bool buffer,sample;
int merge1,merge2;
int waste;
*/

/*
(1,1):sample
(1,n):buffer
(1,(n+1)/2):waste
*/


class Command
{
private:
    const int m,n,d;
    const Point samplePoint,bufferPoint,wastePoint;
    int nowStep;
    Point nowTo;
    void next(); //the next Point after nowto
    Point nextPlace(); //where we should insert new droplet
    Point getPoint(int cc);
    std::vector<Point> spares;
    std::vector<StepInfo> steps;
    std::vector<Point>* places; 
public:
    Command(int M,int N,int D):m(M),n(N),d(D),samplePoint({1,1}),bufferPoint({1,n}),wastePoint({1,(n+1)/2}),nowTo({2,(n-1)/2})
    {
        places=new std::vector<Point>[(1<<d)+1];
    }
    ~Command()
    {
        spares.resize(0);
        steps.resize(0);
    }
    void initialize(std::vector<StepInfo>&STEPS)
    {
        steps.resize(0);
        steps=STEPS;
        nowStep=0;
    }
    bool update(Chip &chip);
};

#endif //COMMAND_H