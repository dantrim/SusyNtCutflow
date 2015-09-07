// SusyNtCutflow
#include "SusyNtCutflow/Selector.h"
#include "SusyNtuple/SusyKin.h"


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
        // Serhan
        /////////////////////////
        case(Cutflow::Serhan) : {
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
//  Pass exactly N base leptons
/* ================================================ */
bool Selector::pass_baseNLep(Link* link)
{
    return link->baseLeptons->size()==2;
}
/* ================================================ */
//  Pass exactly N signal leptons
/* ================================================ */
bool Selector::pass_sigNLep(Link* link)
{
    return link->leptons->size()==2;
}
/* ================================================ */
//  Return the sign of the product of lepton charges
/* ================================================ */
int Selector::leptonSign(Link* link)
{
    if(m_cutflow == Cutflow::Stop2l_ME || m_cutflow == Cutflow::Serhan) { 
        int same_sign = true;
        for(uint i=1; i!=link->baseLeptons->size(); ++i){
            if(link->baseLeptons->at(0)->q != link->baseLeptons->at(i)->q){
                same_sign = false;
                break;
            }
        }
        return same_sign;
    } else {
        int q=1;
        for(uint i=0; i<link->baseLeptons->size(); ++i)
        {
            q *= link->baseLeptons->at(i)->q;
        }
        if(link->baseLeptons->size()==1) q=1; // return 1;
        else if(q>0)                     q=1;
        else if(q<0)                     q=-1;
        return q;
    }
}
/* ================================================ */
//  Pass mll cut
/* ================================================ */
bool Selector::pass_minMll(Link* link)
{
    return ((*link->leptons->at(0) + *link->leptons->at(1)).M()>20.0);
}
/* ================================================ */
//  Pass met cut
/* ================================================ */
bool Selector::pass_minMet(Link* link)
{
    return link->met->Et>40.0;
}
/* ================================================ */
//  Pass R1
/* ================================================ */
bool Selector::pass_R1(Link* link, float threshold, bool thresh_is_min)
{
    float meff = SusyKin::Meff(*link->leptons, *link->jets, link->met, 0.0);
    float r1 = link->met->Et / meff * 1.0;
    if(thresh_is_min) { return (r1 > threshold); }
    else { return (r1 < threshold); }
}
/* ================================================ */
//  Pass MT2
/* ================================================ */
bool Selector::pass_mt2(Link* link, float threshold, bool thresh_is_min)
{
    float mt2 = SusyKin::getMT2(*link->leptons, link->met);
    if(thresh_is_min) { return (mt2 > threshold); }
    else { return (mt2 < threshold); }
}
/* ================================================ */
//  Pass |deltaX| wwlike ww-like WW-like WWlike
/* ================================================ */
bool Selector::pass_deltaX(Link* link, float threshold, bool thresh_is_min)
{
    float sqrtS = 13000.0;
    float num = link->leptons->at(0)->Pz() + link->leptons->at(1)->Pz();
    float deltaX = num / sqrtS * 1.0;
    if(thresh_is_min) { return (fabs(deltaX) > threshold); }
    else { return (fabs(deltaX) < threshold); }
}
/* ================================================ */
//  Pass R2 wwlike ww-like WW-like WWlike
/* ================================================ */
bool Selector::pass_R2(Link* link, float threshold, bool thresh_is_min)
{
    float denom = link->met->lv().Pt() + link->leptons->at(0)->Pt() + link->leptons->at(1)->Pt();
    float r2 = link->met->lv().Pt() / denom * 1.0;
    if(thresh_is_min) { return (r2 > threshold); }
    else { return (r2 < threshold); }
}
/* ================================================ */
//  Pass |cosThetaB| wwlike ww-like WW-like WWlike
/* ================================================ */
bool Selector::pass_cosThetaB(Link* link, float threshold, bool thresh_is_min)
{
    TLorentzVector lp;
    TLorentzVector lm;
    for(unsigned int iL = 0; iL < link->leptons->size(); iL++) {
        if(link->leptons->at(iL)->q < 0) lm = *link->leptons->at(iL);
        else if(link->leptons->at(iL)->q > 0) lp = *link->leptons->at(iL);
    } // iL
    TLorentzVector ll = lp + lm;
    TVector3 boost = ll.BoostVector();
    lp.Boost(-boost);
    lm.Boost(-boost);
    float cosThetaB = tanh((lp.Eta()-lm.Eta()) / 2.);

    if(thresh_is_min) { return ( fabs(cosThetaB) > threshold); }
    else { return (fabs(cosThetaB) < threshold); }
}
