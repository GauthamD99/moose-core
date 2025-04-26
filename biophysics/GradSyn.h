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
    void setTau(double Tau);
    double getTau() const;

    void setVmid(double Vmid);
    double getVmid() const;

    void setSlope(double Slope);
    double getSlope() const;

    void vSetGbar( const Eref& e, double Gbar ); 

    double calcGk();


    void vProcess( const Eref& e, ProcPtr p );
    void vReinit( const Eref& e, ProcPtr p );

    void Vpre( double val );


    static const Cinfo* initCinfo();


    protected:
        double Tau_;
        double Vmid_;
        double Slope_;
        double Vpre_;
        double dt_;
        double s_inf;
        double s_;





};

#endif