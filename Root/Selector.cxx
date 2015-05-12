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
                m_minPtBaseLep   = 10; // GeV
                m_minBaseLepIncl = 1;
                m_oppositeSign   = true;
                m_nBaseLep       = 2;
                m_nSigLep        = 2;
            }
            else if(ireg==1) // SS
            {
                m_minPtBaseLep   = 10; // GeV
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
/* ================================================ */
//  Pass minimum baseline lepton multiplicity
/* ================================================ */
bool Selector::pass_minNBase(Link* link)
{
    bool pass_minLep = true;
    bool pass_minPt  = true;
    
    if(!((int)link->baseLeptons->size() >= m_minBaseLepIncl)) pass_minLep = false;
    for(uint ilep=0; ilep<link->baseLeptons->size(); ilep++)
    {
        if( link->baseLeptons->at(ilep)->Pt() < m_minPtBaseLep ) pass_minPt = false;
    }
    return (pass_minLep && pass_minPt);
}
/* ================================================ */
//  Return the sign of the product of lepton charges
/* ================================================ */
int Selector::leptonSign(Link* link)
{
    int q=1;
    for(uint i=0; i<link->baseLeptons->size(); ++i)
    {
        q *= link->baseLeptons->at(i)->q;
    }
    if(link->baseLeptons->size()==1) return 1;
    else if(q>0)                     return 1;
    else if(q<0)                     return -1;
}
