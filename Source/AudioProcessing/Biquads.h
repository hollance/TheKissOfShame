#ifndef BIQUADS_H
#define BIQUADS_H

#include <stdlib.h>
#include "../../JuceLibraryCode/JuceHeader.h"
#include <time.h>

using namespace juce;

/*
 Implementation of filter designes based on the biquad setup with difference equation:

 y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b1*y(n-1) - b2*y(n-2)

 Coefficients a0, a1, b0, and b1 are defined for the following setups:
 
 LowPass, HighPass, BandPass, BandStop, AllPass, Buttersworth (LP and HP), Linkwitz-riley (LP and HP)
 */

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!! NOTE !!!!! Need to restrict some of the filter coefficients. Review the design notes for all the filters!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#define PI 3.14159265359
#define SAMPLE_RATE 44100

#define MAX_CHANNELS 2


class Biquads
{
public:
    
    Biquads() : a0(0), a1(0), a2(0), b1(0), b2(0), c0(0), d0(0), isModifiedBiquad(false)
    {
        
        for(int i = 0; i < MAX_CHANNELS; i++)
        {
            priorIn_2[i] = 0;
            priorIn_1[i] = 0;
            curInSample[i] = 0;
            
            priorOut_2[i] = 0;
            priorOut_1[i] = 0;
            curOutSample[i] = 0;
        }

    }
    
    ~Biquads(){}
    

//    float process(float inSample)
//    {
//        priorIn_2 = priorIn_1;
//        priorIn_1 = curInSample;
//        curInSample = inSample;
//        
//        curOutSample = a0*curInSample + a1*priorIn_1 + a2*priorIn_2 - b1*priorOut_1 - b2*priorOut_2;
//        
//        if(isModifiedBiquad)
//            curOutSample = c0*curOutSample + d0*inSample;
//
//        
//        priorOut_2 = priorOut_1;
//        priorOut_1 = curOutSample;
//        
//
//        return curOutSample;
//    }
    
    float process(float inSample, int channel)
    {
        priorIn_2[channel] = priorIn_1[channel];
        priorIn_1[channel] = curInSample[channel];
        curInSample[channel] = inSample;
        
        curOutSample[channel] = a0*curInSample[channel] + a1*priorIn_1[channel] + a2*priorIn_2[channel] - b1*priorOut_1[channel] - b2*priorOut_2[channel];
        
        if(isModifiedBiquad)
            curOutSample[channel] = c0*curOutSample[channel] + d0*inSample;
        
        
        priorOut_2[channel] = priorOut_1[channel];
        priorOut_1[channel] = curOutSample[channel];
        
        
        return curOutSample[channel];
    }

    
    
    void setLowPass(float fc, float Q, bool firsOrder)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;
        
        if(firsOrder)
        {
            float theta = 2*PI*fc/SAMPLE_RATE;
            float y = cosf(theta) / (1+sinf(theta));
            
            a0 = (1 - y) / 2;
            a1 = (1 - y) / 2;
            a2 = 0.0;
            b1 = -y;
            b2 = 0.0;
        }
        else //second order
        {
            float theta = 2*PI*fc/SAMPLE_RATE;
            float d = 1/Q;
            float beta = 0.5 * (1 - 0.5*d*sinf(theta)) / (1 + 0.5*d*sinf(theta));
            float y = (0.5 + beta) * cosf(theta);
            
            a0 = (0.5 + beta - y) / 2.0;
            a1 = (0.5 + beta - y);
            a2 = (0.5 + beta - y) / 2.0;
            b1 = -2*y;
            b2 = 2*beta;
        }
    }
    
    
    void setHighPass(float fc, float Q, bool firsOrder)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;

        if(firsOrder)
        {
            float theta = 2*PI*fc/SAMPLE_RATE;
            float y = cosf(theta) / (1+sinf(theta));

            a0 = (1 + y) / 2;
            a1 = -(1 + y) / 2;
            a2 = 0.0;
            b1 = -y;
            b2 = 0.0;
        }
        else
        {
            float theta = 2*PI*fc/SAMPLE_RATE;
            float d = 1/Q;
            float beta = 0.5 * (1 - 0.5*d*sinf(theta)) / (1 + 0.5*d*sinf(theta));
            float y = (0.5 + beta) * cosf(theta);
            
            a0 = (0.5 + beta + y) / 2.0;
            a1 = -(0.5 + beta + y);
            a2 = (0.5 + beta + y) / 2.0;
            b1 = -2*y;
            b2 = 2*beta;
        }
    }

    //2nd order band pass
    void setBandPass(float fc, float Q)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;

        float theta = 2*PI*fc/SAMPLE_RATE;
        float beta = 0.5 * (1 - tanf(theta/(2*Q))) / (1 + tanf(theta/(2*Q)));
        float y = (0.5 + beta) * cosf(theta);
        
        a0 = 0.5 - beta;
        a1 = 0.0;
        a2 = -(0.5 - beta);
        b1 = -2*y;
        b2 = 2*beta;
    }

    //2nd order band stop
    void setBandStop(float fc, float Q)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;

        float theta = 2*PI*fc/SAMPLE_RATE;
        float beta = 0.5 * (1 - tanf(theta/(2*Q))) / (1 + tanf(theta/(2*Q)));
        float y = (0.5 + beta) * cosf(theta);
        
        a0 = 0.5 + beta;
        a1 = -2*y;
        a2 = 0.5 + beta;
        b1 = -2*y;
        b2 = 2*beta;
    }
    
    
    void setAllPass(float fc, float Q, bool isFirstOrder)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;

        if(isFirstOrder)
        {
            float alpha = (tanf(PI*fc / SAMPLE_RATE) - 1) / (tanf(PI*fc / SAMPLE_RATE) + 1);
            
            a0 = alpha;
            a1 = 1.0;
            a2 = 0.0;
            b1 = alpha;
            b2 = 0.0;
        }
        else
        {
            float alpha = (tanf(PI*Q / SAMPLE_RATE) - 1) / (tanf(PI*Q / SAMPLE_RATE) + 1);
            float beta = -cosf(2*PI*fc/SAMPLE_RATE);
            
            a0 = -alpha;
            a1 = beta * (1 - alpha);
            a2 = 1.0;
            b1 = beta * (1 - alpha);
            b2 = -alpha;
        }
    }

    //second order butterworth: lowpass or highpass
    void setButterworth_LowHighPass(float fc, float Q, bool isLowPass)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;
        
        if(isLowPass)
        {
            float tanInput = (fc*PI/SAMPLE_RATE >= PI/2) ? PI/2 - .001 : fc*PI/SAMPLE_RATE;
            float C = 1 / tanf(tanInput);
            
            a0 = 1 / (1 + powf(2.0, 0.5)*C + powf(C, 2.0));
            a1 = 2*a0;
            a2 = a0;
            b1 = 2*a0 * (1 - powf(C, 2.0));
            b2 = a0 * (1 - powf(2.0, 0.5)*C + powf(C, 2.0));
        }
        else
        {
            float tanInput = (fc*PI/SAMPLE_RATE >= PI/2) ? PI/2 - .001 : fc*PI/SAMPLE_RATE;
            float C = tanf(tanInput);
            
            a0 = 1 / (1 + powf(2.0, 0.5)*C + powf(C, 2.0));
            a1 = -2*a0;
            a2 = a0;
            b1 = 2*a0 * (powf(C, 2.0) - 1);
            b2 = a0 * (1 - powf(2.0, 0.5)*C + powf(C, 2.0));
        }
    }
    
    
    //second order butterworth: bandpass or bandstop
    void setButterworth_BandPass(float fc, float Q, bool isBandPass)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;

        float bandwidth = fc/Q;
        
        //NOTE: Need to restrict the bandwidth..... review notes in book...
        
        if(isBandPass)
        {
            float C = 1 / (tanf(fc*PI*bandwidth/SAMPLE_RATE));
            float D = 2*cosf(2*PI*fc/SAMPLE_RATE);

            a0 = 1 / (1 + C);
            a1 = 0.0;
            a2 = -a0;
            b1 = -a0 * C * D;
            b2 = a0 * (C - 1);
        }
        else
        {
            float C = tanf(fc*PI*bandwidth/SAMPLE_RATE);
            float D = 2*cosf(2*PI*fc/SAMPLE_RATE);
            
            a0 = 1 / (1 + C);
            a1 = -a0 * D;
            a2 = a0;
            b1 = -a0 * D;
            b2 = a0 * (1 - C);
        }
    }


    void setLinkwitzRiley(float fc, float Q, bool isLowPass)
    {
        isModifiedBiquad = false;
        c0 = 1;
        d0 = 0;

        float theta = 2*PI*fc/SAMPLE_RATE;
        float omega = PI*fc;
        float kappa = omega / tanf(theta);
        float delta = powf(kappa, 2.0) + powf(omega, 2.0) + 2*kappa*omega;
        
        if(isLowPass)
        {
            a0 = powf(omega, 2.0) / delta;
            a1 = 2*a0;
            a2 = a0;
            b1 = (-2*powf(kappa, 2.0) + 2*powf(omega, 2.0)) / delta;
            b2 = (-2*kappa*omega + powf(kappa, 2.0) + powf(omega, 2.0)) / delta;
        }
        else
        {
            a0 = powf(kappa, 2.0) / delta;
            a1 = -2*a0;
            a2 = a0;
            b1 = (-2*powf(kappa, 2.0) + 2*powf(omega, 2.0)) / delta;
            b2 = (-2*kappa*omega + powf(kappa, 2.0) + powf(omega, 2.0)) / delta;
        }
     }
    
    void setShelve(float fc, float gain_dB, bool isLowShelf)
    {
        isModifiedBiquad = true;
        
        if(isLowShelf)
        {
            float theta = 2*PI*fc/SAMPLE_RATE;
            float mu = powf(10.0, gain_dB/20);
            float beta = 4 / (1 + mu);
            float delta = beta * tanf(theta/2);
            float gamma = (1 - delta) / (1 + delta);

            a0 = (1 - gamma) / 2;
            a1 = a0;
            a2 = 0.0;
            b1 = -gamma;
            b2 = 0.0;
            c0 = mu - 1.0;
            d0 = 1.0;
        }
        else
        {
            float theta = 2*PI*fc/SAMPLE_RATE;
            float mu = powf(10.0, gain_dB/20);
            float beta = (1 + mu) / 4;
            float delta = beta * tanf(theta/2);
            float gamma = (1 - delta) / (1 + delta);
            
            a0 = (1 + gamma) / 2;
            a1 = -(1 + gamma) / 2;
            a2 = 0.0;
            b1 = -gamma;
            b2 = 0.0;
            c0 = mu - 1.0;
            d0 = 1.0;
        }
    }
    
    void setPeak(float fc, float Q, float gain_dB, bool isConstantQ, bool isBoost)
    {
        isModifiedBiquad = true;
        
        if(isConstantQ)
        {
            float K = tanf(PI*fc/SAMPLE_RATE);
            float V0 = powf(10.0, gain_dB/20);
            float D0 = 1 + K / Q + powf(K, 2.0);
            float E0 = 1 + K / (V0*Q) + powf(K, 2.0);
            
            float alpha = 1 + V0/Q * K + powf(K, 2.0);
            float beta = 2 * (powf(K, 2.0) - 1);
            float gamma = 1 - V0/Q * K + powf(K, 2.0);
            float delta = 1 - K / Q + powf(K, 2.0);
            float eta = 1 - K / (V0*Q) + powf(K, 2.0);
            
            if(isBoost)
            {
                a0 = alpha / D0;
                a1 = beta / D0;
                a2 = gamma / D0;
                b1 = beta / D0;
                b2 = delta / D0;
                c0 = 1.0;
                d0 = 0.0;
            }
            else
            {
                a0 = D0 / E0;
                a1 = beta / E0;
                a2 = delta / E0;
                b1 = beta / E0;
                b2 = eta / E0;
                c0 = 1.0;
                d0 = 0.0;
            }
        }
        else
        {
            float theta = 2*PI*fc/SAMPLE_RATE;
            float mu = powf(10.0, gain_dB/20);
            float zeta = 4 / (1 + mu);
            float beta = 0.5 * (1 - zeta*tanf(theta / (2*Q))) / (1 + zeta*tanf(theta / (2*Q)));
            float gamma = (05 + beta) * cosf(theta);
            
            a0 = 0.5 - beta;
            a1 = 0.0;
            a2 = -(0.5 - beta);
            b1 = -2*gamma;
            b2 = 2*beta;
            c0 = mu - 1.0;
            d0 = 1.0;
        }

    }

    
private:
    
    float priorIn_2[MAX_CHANNELS];
    float priorIn_1[MAX_CHANNELS];
    float curInSample[MAX_CHANNELS];
    
    float priorOut_2[MAX_CHANNELS];
    float priorOut_1[MAX_CHANNELS];
    float curOutSample[MAX_CHANNELS];
    
    float a0, a1, a2, b1, b2;
    float c0, d0;
    bool isModifiedBiquad;
    
};


#endif
