/**********************************************************************
** This program is part of 'MOOSE', the
** Messaging Object Oriented Simulation Environment,
** also known as GENESIS 3 base code.
**           copyright (C) 2003-2005 Upinder S. Bhalla. and NCBS
** It is made available under the terms of the
** GNU Lesser General Public License version 2.1
** See the file COPYING.LIB for the full notice.
*********************************************************************
*/

#ifndef _HHChannelF2D_h
#define _HHChannelF2D_h

#include "ChanBase.h"
#include "ChanCommon.h"
#include "HHChannelBase.h"

/**
 *
 * In HHChannelF2D, there are three possible arguments to each gate:
 * The Vm, a conc and a conc2. Two are used at a time to look up a
 * 2-D array.
 */

// Ported to asyn13 on 2014-05-30 by Subhasis Ray

class HHGateF2D;

class HHChannelF2D : public HHChannelBase {
    // #ifdef DO_UNIT_TESTS
    //     friend void testHHChannelF2D();
    // #endif  // DO_UNIT_TESTS

public:
    HHChannelF2D();

    /////////////////////////////////////////////////////////////
    // Value field access function definitions
    /////////////////////////////////////////////////////////////
    void setXindex(string index);
    string getXindex() const;
    void setYindex(string index);
    string getYindex() const;
    void setZindex(string index);
    string getZindex() const;
    /// Access function used for the X gate. The index is ignored.
    HHGateF2D* getXgate(unsigned int i);
    /// Access function used for the Y gate. The index is ignored.
    HHGateF2D* getYgate(unsigned int i);
    /// Access function used for the Z gate. The index is ignored.
    HHGateF2D* getZgate(unsigned int i);
    /// Dummy assignment function for the number of gates.
    void setNumGates(unsigned int num);

    /**
     * Access function for the number of Xgates. Gives 1 if present,
     * otherwise 0.
     */
    unsigned int getNumXgates() const;
    /// Returns 1 if Y gate present, otherwise 0
    unsigned int getNumYgates() const;
    /// Returns 1 if Z gate present, otherwise 0
    unsigned int getNumZgates() const;

    /////////////////////////////////////////////////////////////
    // Dest function definitions
    /////////////////////////////////////////////////////////////
    /**
     * processFunc handles the update and calculations every
     * clock tick. It first sends the request for evaluation of
     * the gate variables to the respective gate objects and
     * recieves their response immediately through a return
     * message. This is done so that many channel instances can
     * share the same gate lookup tables, but do so cleanly.
     * Such messages should never go to a remote node.
     * Then the function does its own little calculations to
     * send back to the parent compartment through regular
     * messages.
     */
    void vProcess(const Eref& e, ProcPtr p) override;

    /**
     * Reinitializes the values for the channel. This involves
     * computing the steady-state value for the channel gates
     * using the provided Vm from the parent compartment. It
     * involves a similar cycle through the gates and then
     * updates to the parent compartment as for the processFunc.
     */
    void vReinit(const Eref& e, ProcPtr p) override;
    /**
     * Assign the local conc_ to the incoming conc from the
     * concentration calculations for the compartment. Typically
     * the message source will be a CaConc object, but there
     * are other options for computing the conc.
     */
    void conc1(double conc);
    void conc2(double conc);

    /**
     * Function for safely creating each gate, identified by strings
     * as X, Y and Z. Will only work on a new channel, not on a
     * copy. The idea is that the gates are always referred to the
     * original 'library' channel, and their contents cannot be touched
     * except by the original.
     */
    void vCreateGate(const Eref& e, string gateType) override;

    /// Inner utility function for creating the gate.
    void innerCreateGate(const string& gateName, HHGateF2D** gatePtr, Id chanId,
                         Id gateId);

    /// Returns true if channel is original, false if copy.
    bool checkOriginal(Id chanId) const override;

    /**
     * Utility function for destroying gate. Works only on original
     * HHChannel. Somewhat dangerous, should never be used after a
     * copy has been made as the pointer of the gate will be in use
     * elsewhere.
     */
    void destroyGate(const Eref& e, string gateType) override;

    /**
     * Inner utility for destroying the gate
     */
    void innerDestroyGate(const string& gateName, HHGateF2D** gatePtr,
                          Id chanId);
    static const Cinfo* initCinfo();

private:
    int dependency(string index, unsigned int dim);
    double depValue(int dependency);
    double conc1_;
    double conc2_;

    string Xindex_;
    string Yindex_;
    string Zindex_;

    int Xdep0_;
    int Xdep1_;
    int Ydep0_;
    int Ydep1_;
    int Zdep0_;
    int Zdep1_;

    /**
     * HHGateF2D data structure for the xGate. This is writable only
     * on the HHChannel that originally created the HHGate, for others
     * it must be treated as readonly.
     */
    HHGateF2D* xGate_;
    HHGateF2D* yGate_;  /// HHGateF2D for the yGate
    HHGateF2D* zGate_;  /// HHGateF2D for the zGate
};

#endif  // _HHChannelF2D_h
