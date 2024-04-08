#pragma once

#define MAX_CHANNELS 2

class Biquads
{
public:
    Biquads() : a0(0.0f), a1(0.0f), a2(0.0f), b1(0.0f), b2(0.0f), c0(0.0f), d0(0.0f), isModifiedBiquad(false)
    {
        // TODO: don't hardcode the SAMPLE_RATE!

        // TODO: this should be in a reset() function
        for (int i = 0; i < MAX_CHANNELS; i++) {
            priorIn_2[i] = 0.0f;
            priorIn_1[i] = 0.0f;
            curInSample[i] = 0.0f;

            priorOut_2[i] = 0.0f;
            priorOut_1[i] = 0.0f;
            curOutSample[i] = 0.0f;
        }
    }

    float process(float inSample, int channel)
    {
        // Direct Form I implementation, which isn't ideal for floating-point.
        // y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b1*y(n-1) - b2*y(n-2)

        priorIn_2[channel] = priorIn_1[channel];
        priorIn_1[channel] = curInSample[channel];
        curInSample[channel] = inSample;

        curOutSample[channel] = a0*curInSample[channel] + a1*priorIn_1[channel] + a2*priorIn_2[channel] - b1*priorOut_1[channel] - b2*priorOut_2[channel];

        if (isModifiedBiquad) {
            curOutSample[channel] = c0*curOutSample[channel] + d0*inSample;
        }

        priorOut_2[channel] = priorOut_1[channel];
        priorOut_1[channel] = curOutSample[channel];

        return curOutSample[channel];
    }

    // Second order butterworth: lowpass or highpass
    void setButterworth_LowHighPass(float fc, bool isLowPass)
    {
        isModifiedBiquad = false;
        c0 = 1.0f;
        d0 = 0.0f;

        float theta = fc * PI / SAMPLE_RATE;
        if (theta >= PI/2.0f) {
            theta = PI/2.0f - 0.001f;
        }

        const float sqrt2 = 1.41421356237309504880168872420969808f;

        if (isLowPass) {
            float C = 1.0f / std::tan(theta);
            float CC = C * C;

            a0 = 1.0f / (1.0f + sqrt2*C + CC);
            a1 = 2.0f * a0;
            a2 = a0;
            b1 = 2.0f * a0 * (1.0f - CC);
            b2 = a0 * (1.0f - sqrt2*C + CC);
        } else {
            float C = std::tan(theta);
            float CC = C * C;

            a0 = 1.0f / (1.0f + sqrt2*C + CC);
            a1 = -2.0f * a0;
            a2 = a0;
            b1 = 2.0f * a0 * (CC - 1.0f);
            b2 = a0 * (1.0f - sqrt2*C + CC);
        }
    }

private:
    // Filter state:
    float priorIn_2[MAX_CHANNELS];     // x[n - 2]
    float priorIn_1[MAX_CHANNELS];     // x[n - 1]
    float curInSample[MAX_CHANNELS];   // x[n]

    float priorOut_2[MAX_CHANNELS];    // y[n - 2]
    float priorOut_1[MAX_CHANNELS];    // y[n - 1]
    float curOutSample[MAX_CHANNELS];  // y[n]

    // Filter coefficients:
    float a0, a1, a2, b1, b2;
    float c0, d0;
    bool isModifiedBiquad;  // whether to use c0 and d0
};
