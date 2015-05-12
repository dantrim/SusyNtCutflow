// Standard
#include <iomanip>
#include <algorithm> // max_element
#include <sstream>   //ostringstream

// SusyNtCutflow
#include "SusyNtCutflow/Counter.h"

using namespace Susy;
using namespace std;


/* =================================================== */
//  Constructor 
/* =================================================== */
Counter::Counter() :
    // cutflow type
    m_cutflow(Cutflow::kUnknown),
    // final state topologies
    m_dilepton(false),
    m_singleLep(false),
    // cut requirements
    m_minBaseLepIncl(0),
    m_minBaseLepExcl(0),
    m_nBaseLep(0),
    m_nSigLep(0)
{
}
/* =================================================== */
//  Set the cutflow
/* =================================================== */
Counter& Counter::setCutflow(const Cutflow &c)
{

    cout << "setting cutflow: " << Cutflow2str(c) << endl;
    m_cutflow = c;
    Counter::eventCutflowLabels();
    Counter::constructEventCleaningCounters();
    Counter::constructCounters(c);


    return *this;
}


/* =================================================== */
//  Cutflow labels (event cleaning)
/* =================================================== */
const vector<string> Counter::eventCutflowLabels()
{
    std::vector<std::string> labels;
    // read-in
    labels.push_back("Read-in");
    // grl
    labels.push_back("grl");
    // lar error
    labels.push_back("lar err");
    // Tile error
    labels.push_back("tile err");
    // TTC
    labels.push_back("ttc veto");   
    // bad muon
    labels.push_back("bad muon");
    // jet cleaning
    labels.push_back("jet cleaning");
    // good vtx
    labels.push_back("good vertex");
    // cosmic muons
    labels.push_back("cosmic muon");

    return labels;
}

/* =================================================== */
//  Cutflow labels (per region)
/* =================================================== */
const vector<vector<string>> Counter::cutflowLabels(const Cutflow &c)
{
    std::vector< vector<std::string> > labels;
    switch(c) {
    //////////////////////////////////////
    // Stop-2L_ME
    // (Cutflow with Maria-Elena)
    //////////////////////////////////////
    case(Cutflow::Stop2l_ME) : {
        //
        // OS selection
        // 
        m_regions.push_back("Stop2L-OS");
        std::vector<std::string> os_labels;
        os_labels.push_back(">= 1 baseline lepton");
        os_labels.push_back("== 2 baseline leptons");
        os_labels.push_back("== 2 signal leptons");
        labels.push_back(os_labels);
        //
        // SS selection
        //
        m_regions.push_back("Stop2L-SS");
        std::vector<std::string> ss_labels;
        ss_labels.push_back(">= 1 baseline lepton");
        ss_labels.push_back("== 2 baseline leptons");
        ss_labels.push_back("== 2 signal leptons");
        labels.push_back(os_labels);
        break;
    } // end case Stop2L_ME

    //////////////////////////////////////
    // Unknown
    //////////////////////////////////////
    case(Cutflow::kUnknown) : {
        cout << "Cutflow::cutflowLabels error: invalid analysis." << endl;
        cout << "   --> Exitting." << endl;
        exit(1);
        break;
    } // end case unknown
    } // end switch

    return labels;
}
/* =================================================== */
//  Construct the counters on the event cleaners
/* =================================================== */
void Counter::constructEventCleaningCounters()
{
    for(unsigned int iEv=0; iEv<Counter::eventCutflowLabels().size(); ++iEv)
    {
        cout << Counter::eventCutflowLabels()[iEv] << endl;
        cout << Counter::eventCutflowLabels()[iEv] << endl;
        cout << Counter::eventCutflowLabels()[iEv] << endl;
        cout << Counter::eventCutflowLabels()[iEv] << endl;
        m_cleaningCounters[iEv] = 0.0;
    }
}
/* =================================================== */
//  Construct the counters base on cutflow
/* =================================================== */
void Counter::constructCounters(const Cutflow &c)
{
    switch(c) {
    //////////////////////////////////////
    // Stop-2L_ME
    // (Cutflow with Maria-Elena)
    //////////////////////////////////////
        case(Cutflow::Stop2l_ME) : {
            for(unsigned int iReg=0; iReg<cutflowLabels(c).size(); iReg++){
            for(unsigned int iCut=0; iCut<cutflowLabels(c)[iReg].size(); iCut++){
                m_dileptonCounters[iReg][iCut][LeptonChan2idx(LeptonChan::SF)] = 0.0;
                m_dileptonCounters[iReg][iCut][LeptonChan2idx(LeptonChan::DF)] = 0.0;
            } // iCut
            } // iReg
        
            break;
    }
    //////////////////////////////////////
    // Unknown
    //////////////////////////////////////
    case(Cutflow::kUnknown) : {
        cout << "Cutflow::constructCounters error: invalid analysis." << endl;
        cout << "   --> Exitting." << endl;
        exit(1);
        break;
    } // end case unknown
    } // end switch

}

/* =================================================== */
//  Event cleaning cuts
/* =================================================== */
bool Counter::pass_eventCleaning(Link* link)
{
    int iEventCut=0;
    // count number read-in
    m_cleaningCounters[iEventCut]++;    iEventCut++;

    int flags = link->nt->evt()->cutFlags[NtSys::SusyNtSys::NOM]; 
    // grl
    if(!(flags & ECut_GRL))             return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++; 
    // lar error
    if(!(flags & ECut_LarErr))          return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++;
    // tile error
    if(!(flags & ECut_TileErr))         return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++;
    // ttc veto
    if(!(flags & ECut_TTC))             return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++;
    // bad muon
    if(!(flags & ECut_BadMuon))         return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++;
    // jet cleaning
    if(!(flags & ECut_BadJet))          return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++;
    // good vertex
    if(!(flags & ECut_GoodVtx))         return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++;
    // cosmic muons
    if(!(flags & ECut_Cosmic))          return false;
    m_cleaningCounters[iEventCut]++;    iEventCut++;


    // passed event cleaning
    return true;
}
/* =================================================== */
//  Get the event cleaning counters
/* =================================================== */
std::string Counter::retrieveEventCounters()
{
    ostringstream oss;
    oss << " Event counters " << endl;
    vector<string> labels = Counter::eventCutflowLabels();
    struct shorter { bool operator()(const string &a, const string &b)
            { return a.size() < b.size(); } };
    size_t max_label_length = max_element(labels.begin(), labels.end(), shorter())->size();
    for(size_t i=0; i<labels.size(); i++) {
        oss<<"  "<<setw(max_label_length+2)
           <<std::left<<labels[i]<<m_cleaningCounters[i]
           <<endl;
    }
    oss<<endl;
    return oss.str();
}
