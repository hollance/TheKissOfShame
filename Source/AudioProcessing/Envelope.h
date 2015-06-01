#ifndef ENVELOPE_H
#define ENVELOPE_H


//#include "Generator.h"
//#include <stdlib.h>
//#include "../../JuceLibraryCode/JuceHeader.h"
#include "../shameConfig.h"


class Envelope
{
public:
    
    Envelope(int domainMS) : incr(0.0), domain(domainMS*44.1), loopDuration(domainMS*44.1)
    {
        Point<float> pStart(0.0, 0.0);
        Point<float> pEnd(1.0, 0.0);
        points.add(pStart);
        points.add(pEnd);
    }
    
    ~Envelope(){}
    
    
    void addEvelopePoint(float x, float y)
    {
        //std::cout << "Dyn Intensity: " << dynamicExtremity << std::endl;
        
        Point<float> pt(x, y);
        points.insert(points.size() - 1, pt);
        
        //for(int i = 0; i < points.size(); i++)
        //{
        //    std::cout << "X Val: " << points[i].getX() << "Y Val: " << points[i].getY() << std::endl;
        //}
    }
    
    void setDomainMS(float d) //NOTE: domain is set in milleseconds
    {
        domain = d * 44.1;
        
        if(domain > loopDuration) loopDuration = domain;
    }
    
    void setLoopDurationMS(float dur)
    {
        loopDuration = dur * 44.1;
        
        if(loopDuration < domain) domain = loopDuration;
    }
    
    float processEnvelope()
    {
        float interpolatedValue = 0.0;
        
        if(incr < domain)
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
            interpolatedValue = (1- distFromPrior)*priorY + (distFromPrior)*nxtY;
        }
        else
        {
            interpolatedValue = 0.0;
        }
        
        
        //std::cout << "CurPos: " << curPos << ", prY: " << priorY << ", NxtY: "<< nxtY << ", Value: " << interpolatedValue << std::endl;

        
        //increment through domain.
        incr++;
        if(incr >= loopDuration)
        {
            incr = 0;
        }
        
        return interpolatedValue;
    }
    
    
    
private:
    
    float incr;
    float domain;
    float loopDuration;
    
    Array<Point<float>> points;
    
};




#endif
