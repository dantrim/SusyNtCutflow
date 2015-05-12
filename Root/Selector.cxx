// SusyNtCutflow
#include "SusyNtCutflow/Selector.h"


using namespace std;
using namespace Susy;

/* ================================================ */
//  Constructor
/* ================================================ */
Selector::Selector() :
    // cutflow type
    m_cutflow(Cutflow::kUnknown),
    // selection requirements
    m_minBaseLepIncl(0),
    m_minBaseLepExcl(0),
    m_nBaseLep(0),
    m_nSigLep(0),
    m_oppositeSign(false)
{
}
/* ================================================ */
//  Set cutflow
/* ================================================ */
Selector& Selector::setCutflow(const Cutflow &c)
{
    m_cutflow = c;
    return *this;
}
/* ================================================ */
//  Build the regions
/* ================================================ */
Selector& Selector::buildRegion(int ireg)
{
    switch(m_cutflow) {
        /////////////////////////
        // Stop-2L ME
        /////////////////////////
        case(Cutflow::Stop2l_ME) : {
            if(ireg==0) // OS
            {
                m_minBaseLepIncl = 1;
                m_oppositeSign   = true;
                m_nBaseLep       = 2;
                m_nSigLep        = 2;
            }
            else if(ireg==1) // SS
            {
                m_minBaseLepIncl = 1;
                m_oppositeSign   = false;
                m_nBaseLep       = 2;
                m_nSigLep        = 2;
            }
            break;
        }
        /////////////////////////
        // Unknown
        /////////////////////////
        case(Cutflow::kUnknown) : {
            cout << "WARNING Selector::buildRegion: unknown cutflow type!" << endl;
            cout << "WARNING    --> Exitting." << endl;
            exit(1);
            break;
        }
    } // end swtich

    return *this;
}
