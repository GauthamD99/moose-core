#ifndef GRADSYN_H
#define GRADSYN_H

#include "ChanCommon.h"
#include "ChanBase.h"

class GradSyn : public ChanCommon
{
public :
    GradSyn();
    ~GradSyn();
    /////////////////////////////////////////////////////////////
        // Value field access function definitions
        /////////////////////////////////////////////////////////////
    void setTau(float Tau);
    float getTau() const;

    void setVmid(float Vmid);
    float getVmid() const;

    void setSlope(float Slope);
    float getSlope() const;

    void vSetGbar( const Eref& e, double Gbar ); 

    double calcGk();


    void vProcess( const Eref& e, ProcPtr p );
    void vReinit( const Eref& e, ProcPtr p );

    void Vpre( double val );


    static const Cinfo* initCinfo();


    protected:
        float Tau_;
        float Vmid_;
        float Slope_;
        double Vpre_;
        double dt_;
        double s_inf;
        double s_;





};

#endif