#pragma once

#include "../shameConfig.h"

/**
  Envelope generator with linear segments.
 */
class Envelope
{
public:
    // TODO: make independent of sample rate!
    Envelope(int domainMS) : incr(0.0f), domain(domainMS*44.1f), loopDuration(domainMS*44.1f)
    {
        juce::Point<float> pStart(0.0f, 0.0f);
        juce::Point<float> pEnd(1.0f, 0.0f);
        points.add(pStart);
        points.add(pEnd);
    }

    void addEnvelopePoint(float x, float y) noexcept
    {
        juce::Point<float> pt(x, y);
        points.insert(points.size() - 1, pt);

        //for(int i = 0; i < points.size(); i++)
        //{
        //    std::cout << "X Val: " << points[i].getX() << "Y Val: " << points[i].getY() << std::endl;
        //}
    }

    void setDomainMS(float d) noexcept  // domain is set in milliseconds
    {
        domain = d * 44.1f;

        if (domain > loopDuration) {
            loopDuration = domain;
        }
    }

    void setLoopDurationMS(float dur) noexcept
    {
        loopDuration = dur * 44.1f;

        if (loopDuration < domain) {
            domain = loopDuration;
        }
    }

    float processEnvelope() noexcept
    {
        float interpolatedValue = 0.0f;

        if (incr < domain) {
            float curPos = incr / domain;

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
            interpolatedValue = (1.0f - distance)*prevY + distance*nextY;
        }

        // Increment through domain
        incr++;
        if (incr >= loopDuration) {
            incr = 0.0f;
        }

        return interpolatedValue;
    }

private:
    float incr;
    float domain;
    float loopDuration;

    juce::Array<juce::Point<float>> points;
};
