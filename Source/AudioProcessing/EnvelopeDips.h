#pragma once

#include "../shameConfig.h"

/**
  Envelope generator that creates random segments that extend downwards
  from 1.0 to 0.5 (dynamicExtremity).
 */
class EnvelopeDips
{
public:
    EnvelopeDips() : incr(0.0f), domain(44100.0f), dynamicExtremity(0.0f),
                     numPointRandomness(0.0f), numPoints(5),
                     rng(juce::Random::getSystemRandom())
    {
        rng.setSeed(time(nullptr));

        calculateDipPoints();

        // TODO: have a reset() function that sets incr back to 0.0f
        // and re-seeds the random generator
    }

    // TODO: make independent of sampling rate
    void setDomainMS(float d) noexcept  // domain is set in milliseconds
    {
        domain = d * 44.1f;
    }
    
    void setDynamicExtremity(float dE) noexcept
    {
        dynamicExtremity = dE;
    }

    void setNumPoints(int nP) noexcept
    {
        numPoints = nP;
    }

    void setNumPointRandomness(float nPR) noexcept
    {
        numPointRandomness = nPR;
    }

    float processEnvelopeDips() noexcept
    {
        float curPos = incr / domain;

        // TODO: rather than precomputing a bunch of points and interpolating
        // between them, why not just calculate one new point at a time?

        // Find the two points the curPos is between.
        // TODO: don't like this loop
        float prevX, nextX;
        float prevY, nextY;
        for (int i = points.size() - 1; i >= 0; i--) {
            if (curPos >= points[i].getX()) {
                prevX = points[i].getX();
                prevY = points[i].getY();
                nextX = points[i + 1].getX();
                nextY = points[i + 1].getY();
                break;
            }
        }

        // Calculate the distance from the prior point relative to the next point.
        float distance = (curPos - prevX) / (nextX - prevX);

        // The output value is the weighted average between the two points.
        float interpolatedValue = (1.0f - distance)*prevY + distance*nextY;

        // Increment through domain
        incr++;
        if (incr >= domain) {
            calculateDipPoints();   // recalculate the dip points
            incr = 0.0f;
        }

        return interpolatedValue;
    }

    void calculateDipPoints() noexcept
    {
        points.clear();

        int numRandPoints = int(numPoints * (1.0f - numPointRandomness * random())) + 1;
        float partitionSize = 1.0f / float(numRandPoints + 1);

        // Create set of points, where the beginning and end are always 1.
        points.add({ 0.0f, 1.0f });

        for (int i = 0; i < numRandPoints; i++) {
            float xInit = float(i + 1) / float(numRandPoints + 1);
            float xDeviation = random() * partitionSize * 0.4f;
            
            if (rng.nextBool()) {
                xDeviation = -xDeviation;
            }
            points.add({ xInit + xDeviation, 1.0f - dynamicExtremity*random() });
        }

        points.add({ 1.0f, 1.0f });
    }

private:
    float random() const noexcept
    {
        return rng.nextFloat();
    }

    float incr;
    float domain;
    float dynamicExtremity;
    float numPointRandomness;
    int numPoints;

    juce::Random &rng;
    
    juce::Array<juce::Point<float>> points;
};
