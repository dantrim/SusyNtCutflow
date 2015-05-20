#ifndef SusyNtCutflow_Selector_h
#define SusyNtCutflow_Selector_h

// Standard
#include <iostream>
#include <string>
#include <vector>

// SusyNtCutflow
#include "SusyNtCutflow/Link.h"
#include "SusyNtCutflow/Cutflow.h"

// SusyNtuple
#include "SusyNtuple/SusyDefs.h"


namespace Susy {
    
    class Selector
    {
        public :
        Selector();
        Selector& setCutflow(const Cutflow &c);
        Selector& buildRegion(int ireg);

        
        /**
        Region groupings
        */
        bool pass_Stop2l_ME(Link* link);

        /**
            Return the sign of the product of lepton charges
        */
        int leptonSign(Link* link);

        // pass min base w/ 10 GeV pt requirement
        bool pass_minNBase(Link* link);
        // pass exactly N base leptons
        bool pass_baseNLep(Link* link);
        // pass exactly N signal leptons
        bool pass_sigNLep(Link* link);
        
        bool pass_minMll(Link* link);
        bool pass_minMet(Link* link);
    

        protected :
        Cutflow m_cutflow;
        
        
        // selection requirements
        float   m_minPtBaseLep;         ///< minimum pT of baseline leptons (greater than)
        int     m_minBaseLepIncl;       ///< minimum number of baseline leptons (greater than or equal to)
        int     m_minBaseLepExcl;       ///< minimum number of baseline leptons (greater than)
        int     m_nBaseLep;             ///< number of baseline leptons (==)
        int     m_nSigLep;              ///< number of signal leptons (==)
        bool    m_oppositeSign;         ///< require dilpeon event to be opposite sign

    }; // class Selector


} // namespace Susy








#endif
