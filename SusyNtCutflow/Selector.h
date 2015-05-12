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

        protected :
        Cutflow m_cutflow;
        
        
        // selection requirements
        int     m_minBaseLepIncl;       ///< minimum number of baseline leptons (greater than or equal to)
        int     m_minBaseLepExcl;       ///< minimum number of baseline leptons (greater than)
        int     m_nBaseLep;             ///< number of baseline leptons (==)
        int     m_nSigLep;              ///< number of signal leptons (==)
        bool    m_oppositeSign;         ///< require dilpeon event to be opposite sign

    }; // class Selector


} // namespace Susy








#endif
