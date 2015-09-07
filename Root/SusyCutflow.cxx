#include <iomanip>
#include <iostream>
#include <algorithm>
#include <sstream>

// SusyNtCutflow
#include "SusyNtCutflow/SusyCutflow.h"

using namespace std;
using namespace Susy;

/* ======================================================== */
// Constructor
/* ======================================================== */
SusyCutflow::SusyCutflow() :
    APP_NAME("SusyCutflow"),
    m_anatype(AnalysisType::kUnknown),
    m_cutflow(Cutflow::kUnknown),
    m_verbose(false)
{
}

/* ======================================================== */
//  Set the analysis type
/* ======================================================== */
SusyCutflow& SusyCutflow::setAnalysis(const AnalysisType &a)
{
    // setAnaType
    // m_nttools : SusyNtTools data member of SusyNtAna
    m_anatype = a;
    m_nttools.setAnaType(a);
    switch(a) {
        case(AnalysisType::Ana_2Lep) : {
            cout << "SusyCutflow::setAnalysis: AnalysisType::Ana_2Lep" << endl;
            break;
        }
        case(AnalysisType::Ana_3Lep) : {
            cout << "SusyCutflow::setAnalysis: AnalysisType::Ana_3Lep" << endl;
            break;    
        }
        case(AnalysisType::Ana_2LepWH) : {
            cout << "SusyCutflow::setAnalysis: AnalysisType::Ana_2LepWH" << endl;
            break;
        }
        case(AnalysisType::Ana_SS3L) : {
            cout << "SusyCutflow::setAnalysis: AnalysisType::Ana_SS3L" << endl;
            break;
        }
        case(AnalysisType::Ana_Stop2L) : {
            cout << "SusyCutflow::setAnalysis: AnalysisType::Ana_Stop2L" << endl;
            break;
        }
        case(AnalysisType::kUnknown) : {
            cout << "WARNING SusyCutflow::setAnalysis: AnalysisType::kUnknown" << endl;
            cout << "WARNING    --> Exitting." << endl;
            exit(1);
            break;
        }
    }

    return *this;
}
/* ======================================================== */
//  Set cutflow toggle
/* ======================================================== */
SusyCutflow& SusyCutflow::setCutflow(const Cutflow &a)
{
    m_cutflow = a;
    m_counter.setAnalysis(m_anatype).setCutflow(a);
    return *this;
}

/* ======================================================== */
//  Begin
/* ======================================================== */
void SusyCutflow::Begin(TTree* /*tree*/)
{
    SusyNtAna::Begin(0);
}
/* ======================================================== */
//  Process
/* ======================================================== */
Bool_t SusyCutflow::Process(Long64_t entry)
{
    // current entry in full TChain (SusyNtAna.h)
    m_chainEntry++;
    // Grab the entry in the SusyNtObject 
    GetEntry(entry);
    // Clear the SusyNtuple objects
    clearObjects();
    // Select the SusyNtuple objects
    selectObjects();
    //selectObjects(NtSys::SusyNtSys::NOM, false, TauID_medium);
    // Establish a Link to the SusyNt
    Link* link = new Link();
    SusyCutflow::linkNtObjects(link);

    if(m_dbg || m_chainEntry%5000==0)
    {
        cout << APP_NAME << "    "
             << "Processing entry " << setw(6) << m_chainEntry << "  "
             << "run " << setw(6) << link->nt->evt()->run << "  "
             << "event " << setw(7) << link->nt->evt()->eventNumber 
        << endl;
    }

    // Set up the Counter tool
  //  m_counter.dumpThisInfo(link);
    if(!m_counter.pass_eventCleaning(link))     return false;
   
    if(!m_counter.pass_cutflow(link))           return false; 
   // m_counter.dumpThisCut(link);


    // delete the link here since we re-attach each event
    delete link;
    return kTRUE;
}

/* ======================================================== */
//  Terminate
/* ======================================================== */
void SusyCutflow::Terminate()
{
    // get the event cleaning counter summary
    cout << m_counter.retrieveEventCounters() << endl;
    cout << m_counter.retrieveAnaCounters() << endl;

    // Call SusyNtAna
    SusyNtAna::Terminate();
}


/* ======================================================== */
//  Method to link to the current event's SusyNt objects
/* ======================================================== */
void SusyCutflow::linkNtObjects(Link* link)
{
    link->anatype           = m_anatype;
    link->nt                = &nt;
    link->tools             = &m_nttools;
    
    link->preElectrons      = &m_preElectrons;
    link->baseElectrons     = &m_baseElectrons;
    link->electrons         = &m_signalElectrons;

    link->preMuons          = &m_preMuons;
    link->baseMuons         = &m_baseMuons;
    link->muons             = &m_signalMuons;
        
    link->preLeptons        = &m_preLeptons;
    link->baseLeptons       = &m_baseLeptons;
    link->leptons           = &m_signalLeptons;
    
    link->preJets           = &m_preJets;
    link->baseJets          = &m_baseJets;
    link->jets              = &m_signalJets;
    
    link->baseTaus          = &m_baseTaus;
    link->taus              = &m_signalTaus;
    
    link->met               = m_met;
    
    link->isMC              = nt.evt()->isMC ? true : false;
}
