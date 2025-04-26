/**********************************************************************
** This program is part of 'MOOSE', the
** Messaging Object Oriented Simulation Environment.
**           Copyright (C) 2003-2007 Upinder S. Bhalla. and NCBS
** It is made available under the terms of the
** GNU Lesser General Public License version 2.1
** See the file COPYING.LIB for the full notice.
**********************************************************************/
#include "../basecode/header.h"
#include "ChanBase.h"
#include "ChanCommon.h"
#include "GradSyn.h" 

const Cinfo* GradSyn::initCinfo()
{
    static ValueFinfo<GradSyn, double> Tau(
        "tau",
        "Decay time constant for the synaptic conductance",
        &GradSyn::setTau, 
        &GradSyn::getTau
    );

    static ValueFinfo<GradSyn, double> Vmid(
        "Vmid",
        "The voltage at which s_inf becomes 0.5",
        &GradSyn::setVmid,
        &GradSyn::getVmid
    );    

    static ValueFinfo<GradSyn, double> Slope(
        "Slope",
        "It decides the slope of the sigmoid",
        &GradSyn::setSlope,
        &GradSyn::getSlope
    );

    static DestFinfo Vpre(
        "Vpre",
        "VmOut from the pre synaptic compartment", 
        new OpFunc1<GradSyn, double>(&GradSyn::Vpre)
    );

    static Finfo* GradSynFinfos[] =
    {
        &Tau,    // Value
        &Vmid,   // Value
        &Slope,  // Value
        &Vpre    // Dest
    };

    static string doc[] =
    {
        "Name", "GradSyn",
        "Author", "Upinder S. Bhalla, 2007, 2014, NCBS",
        "Description", 
        "GradSyn: Graded synaptic channel model.\n"
        "Accepts continuous voltage input from a presynaptic neuron\n"
        "and computes a conductance based on sigmoid activation and tau filtering."
    };

    static Dinfo<GradSyn> dinfo;

    static Cinfo GradSynCinfo(
        "GradSyn",                     // Name of the class
        ChanBase::initCinfo(),         // Parent class Cinfo
        GradSynFinfos,                 // Array of finfos
        sizeof(GradSynFinfos)/sizeof(Finfo*), // Number of finfos
        &dinfo,                        // Dinfo pointer
        doc,                           // Documentation
        sizeof(doc)/sizeof(string)     // Number of doc strings
    );

    return &GradSynCinfo;
}

static const Cinfo* GradSynCinfo = GradSyn::initCinfo();

GradSyn::GradSyn()
    : Tau_(1e-3),
      Vmid_(1e-3), 
      Slope_(1e-3),
      Vpre_(0.0),
      dt_(25.0e-6)
{ ; }

GradSyn::~GradSyn()
{;}
 
void GradSyn::vSetGbar(const Eref& e, double Gbar)
{
    ChanCommon::vSetGbar(e, Gbar);
}

void GradSyn::setTau(double Tau) 
{
    Tau_ = Tau;
}

double GradSyn::getTau() const 
{
    return Tau_;
}

void GradSyn::setVmid(double Vmid) 
{
    Vmid_ = Vmid;
}

double GradSyn::getVmid() const 
{
    return Vmid_;
}

void GradSyn::setSlope(double Slope) 
{
    Slope_ = Slope;
}

double GradSyn::getSlope() const 
{
    return Slope_;
}

double GradSyn::calcGk()
{
    s_inf = 1 / (1 + exp((Vmid_ - Vpre_) / Slope_));
    s_ = s_inf + (s_ - s_inf) * exp(-dt_ / Tau_);

    return s_ * getModulation();
}

void GradSyn::vProcess(const Eref& e, ProcPtr info)
{
    setGk(e, calcGk());
    updateIk();
    sendProcessMsgs(e, info);
}

void GradSyn::vReinit(const Eref& e, ProcPtr info)
{
    dt_ = info->dt;
    Vpre_=0.0;
    ChanBase::setGk(e,0.0);
    ChanBase::setIk(e,0.0);
    s_inf=0.0;
    s_=0.0;
    sendReinitMsgs(e,info);
}
void GradSyn:: Vpre(double val)
{
    Vpre_=val;
}
