#ifndef ENVELOPEDIPS_H
#define ENVELOPEDIPS_H


//#include "Generator.h"
//#include <stdlib.h>
//#include "../../JuceLibraryCode/JuceHeader.h"
#include "../shameConfig.h"
#include <time.h>

using namespace juce;


//recalculate random points for every loop
//ability to reset domain, and reset it for every loop
//ability to set fixed number of points. NOTE: random number of points is calculated as input to setting number of points.
//ability to have different starting values. NOTE: need to interpolate to next starting value.

class EnvelopeDips
{
public:
    
    EnvelopeDips() : incr(0.0), domain(44100), dynamicExtremity(0.0), numPointRandomness(0.0), numPoints(5)
    {
        srand (time(NULL));
        
        calculateDipPoints();
        
    }
    
    ~EnvelopeDips(){}
    
    
    void calculateDipPoints()
    {
        //std::cout << "Dyn Intensity: " << dynamicExtremity << std::endl;
        
        points.clear();
        
        float startingValue = 1.0;
        
        int numRandPoints = numPoints * (1.0 - numPointRandomness*(float)(rand() % 1000)/1000) + 1;
        float partitionSize = 1/((float)numRandPoints + 1);
        
        //create set of points, where the beginning and end are always a value of 1.
        Point<float> p0(0.0, startingValue);
        points.add(p0);
        for(int i = 0; i < numRandPoints; i++)
        {
            float xInit = ((float)i+1)/(numRandPoints+1);
            float xDeviation = (float)(rand() % 1000)/1000 * partitionSize / 2.5f; //why is 2.5 in the denominator??
            xDeviation = xDeviation * powf(-1, rand()%2);
            
            Point<float> pRand(xInit + xDeviation, 1.0f - dynamicExtremity*(float)(rand() % 1000)/1000);
            points.add(pRand);
            
            //std::cout << "x deviation: " << xDeviation << std::endl;
        }
        Point<float> p1(1.0, startingValue);
        points.add(p1);
        
        
//        for(int i = 0; i < points.size(); i++)
//        {
//            std::cout << "RandPoint: " << i << ". x, y: " << points[i].getX() << ", " << points[i].getY() << std::endl;
//        }
    }
    
    
    void setDomainMS(float d){ domain = d * 44.1; } //NOTE: domain is set in milleseconds
    void setDynamicExtremity(float dE){dynamicExtremity = dE;}
    void setNumPoints(int nP){numPoints = nP;}
    void setNumPointRandomness(float nPR){numPointRandomness = nPR;}
    
    
    float processEnvelopeDips()
    {
        float curPos = incr/domain;
        
        //find the two points the curPos is between.
        float priorX, nxtX;
        float priorY, nxtY;
        for(int i = points.size() - 1; i >= 0; i--)
        {
            if(curPos >= points[i].getX())
            {
                priorX = points[i].getX();
                priorY = points[i].getY();
                nxtX = points[i+1].getX();
                nxtY = points[i+1].getY();
                break;
            }
        }
        
        //Calculate the distance from the prior point relative to the next point.
        float distFromPrior = (curPos - priorX)/(nxtX - priorX);
        
        //The output value is the weighted average between the two points.
        float interpolatedValue = (1- distFromPrior)*priorY + (distFromPrior)*nxtY;
        
        
        //std::cout << "CurPos: " << curPos << ", prY: " << priorY << ", NxtY: "<< nxtY << ", Value: " << interpolatedValue << std::endl;
        
        //increment through domain.
        incr++;
        if(incr >= domain)
        {
            calculateDipPoints();   //recalculate the dip points
            incr = 0;               //set the increment to 0
        }
        
        return interpolatedValue;
    }
    
    
    
private:
    
    float incr;
    float domain;
    float dynamicExtremity;
    float numPointRandomness;
    int numPoints;
    
    Array<Point<float>> points;
    
};




#endif
