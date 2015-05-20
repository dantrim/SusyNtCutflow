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
    // analysis type
    m_anatype(AnalysisType::kUnknown),
    // cutflow type
    m_cutflow(Cutflow::kUnknown),
    // final state topologies
    m_dilepton(false),
    m_singleLep(false),
    m_dummy(0)
{
}
/* =================================================== */
//  Set the analysis
/* =================================================== */
Counter& Counter::setAnalysis(const AnalysisType &a)
{
    m_anatype = a;
    switch(a) {
        case(AnalysisType::Ana_2Lep) :
        case(AnalysisType::Ana_2LepWH) : {
            m_dilepton = true;
            break;
        }
        case(AnalysisType::Ana_3Lep) : {
            m_triLep = true;
            break;
        }
        case(AnalysisType::kUnknown) : {
            cout << "WARNING Counter::setAnalysis: AnalysisType::kUnknown" << endl;
            cout << "WARNING    --> Exitting." << endl;
            exit(1);
            break;
        }  
    } // end switch

    return *this;
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
        os_labels.push_back("mll > 20 GeV");
        os_labels.push_back("met > 40 GeV");
        labels.push_back(os_labels);
        //
        // SS selection
        //
        m_regions.push_back("Stop2L-SS");
        std::vector<std::string> ss_labels;
        ss_labels.push_back(">= 1 baseline lepton");
        ss_labels.push_back("== 2 baseline leptons");
        ss_labels.push_back("== 2 signal leptons");
        ss_labels.push_back("mll > 20 GeV");
        ss_labels.push_back("met > 40 GeV");
        labels.push_back(ss_labels);
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
            //
            // set dilepton counters
            //  
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
    //debug
    //Counter::dumpThisInfo(link);

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
//  Analysis-level selections
/* =================================================== */
bool Counter::pass_cutflow(Link* link)
{
    bool pass = true;
    switch(m_cutflow) {
        ////////////////////////////////
        // Stop-2L ME
        ////////////////////////////////
        case(Cutflow::Stop2l_ME) : {
            if(!pass_Stop2l_ME(link)) pass = false;
            break;
        }
        //////////////////////////////////////
        // Unknown
        //////////////////////////////////////
        case(Cutflow::kUnknown) : {
            cout << "WARNING Counter::pass_cutflow: error. Uknown cutflow type." << endl;
            cout << "WARNING    --> Exitting." << endl;
            exit(1);
            break;
        }
    } // end switch

    return pass;

}
/* =================================================== */
//  Cuts for Stop2l_ME
/* =================================================== */
bool Counter::pass_Stop2l_ME(Link* link)
{
    for(uint ireg=0; ireg < cutflowLabels(m_cutflow).size(); ireg++){
        int iCut=0;
        m_selector.setCutflow(m_cutflow).buildRegion(ireg);
        if(ireg==0) { 
            if(!m_selector.pass_minNBase(link)) return false; 
            getLeptonFlavor(link);
            int sign = m_selector.leptonSign(link);
     //       m_dileptonCounters[0][iCut][LeptonChan2idx(LeptonChan::SF)]++;
            if(sign>0)      m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            else if(sign<0) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
            //debug
            if(sign<0){
                dumpThisCut(link);
            }
     //   }
            iCut++;
            if(!m_selector.pass_baseNLep(link)) return false;
            getLeptonFlavor(link);
            int sign2 = m_selector.leptonSign(link);
     //       m_dileptonCounters[0][iCut][LeptonChan2idx(LeptonChan::SF)]++;
        if(sign2>0) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
        if(sign2<0) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
            iCut++;
            if(!m_selector.pass_sigNLep(link)) return false;
            if(sign2>0) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            if(sign2<0) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
         //   m_dileptonCounters[0][iCut][LeptonChan2idx(LeptonChan::SF)]++;
        // debug
      //  dumpThisCut(link, entry);
      //  m_dummy++;
            iCut++;
            if(!m_selector.pass_minMll(link)) return false;
            if(sign2>0) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            if(sign2<0) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
        //    m_dileptonCounters[0][iCut][LeptonChan2idx(LeptonChan::SF)]++;
            iCut++;
            if(!m_selector.pass_minMet(link)) return false;
            if(sign2>0) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            if(sign2<0) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
        //    m_dileptonCounters[0][iCut][LeptonChan2idx(LeptonChan::SF)]++;
            iCut++;

        } // ireg==0
    }
    return true;
}

/* =================================================== */
//  Get the "dilepton" flavor for >= 1 baselepton
/* =================================================== */
void Counter::getLeptonFlavor(Link* link)
{
    int ne=0;
    int nm=0;
    for(uint i=0; i<link->baseLeptons->size(); ++i)
    {
        bool isEle = link->baseLeptons->at(i)->isEle();
        if(isEle) { ne++; }
        else      { nm++; }
    }
    if( (ne==1 && nm==0) || (ne==0 && nm==1) ) m_lepchan = LeptonChan::SF;
    else if( (ne==0 && nm>1) || (ne>1 && nm==0) ) m_lepchan = LeptonChan::SF;
    else if( link->baseLeptons->size() > 1 ) m_lepchan = LeptonChan::DF; 
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
/* =================================================== */
//  Get the counters for each ana
/* =================================================== */
std::string Counter::retrieveAnaCounters()
{
    ostringstream oss;
    vector<vector<string>> labels = Counter::cutflowLabels(m_cutflow);
    struct shorter { bool operator()(const std::string &a, const std::string &b){
                        return a.size() < b.size(); } };
    if(m_dilepton)
    {
    for(uint ireg=0; ireg<labels.size(); ireg++){
        
        size_t max_label_length = max_element(labels[ireg].begin(), labels[ireg].end(),
                                shorter())->size();
        oss<<std::left<<setw(max_label_length+2) << m_regions[ireg] << endl;
        oss<<setw(max_label_length+4)<<"  "<<setw(6)<<"SF" <<"  "<< "DF" << endl;
      //  oss<<setw(max_label_length+2)<<" "
      //     <<"SF"<<setw(8)<<"DF"<<endl;
        for(uint icut=0; icut<labels[ireg].size(); icut++){
            oss<<"  "<<setw(max_label_length+2)<<std::left<<labels[ireg][icut]
               <<setw(8)<<m_dileptonCounters[ireg][icut][LeptonChan2idx(LeptonChan::SF)]
               <<setw(8)<<m_dileptonCounters[ireg][icut][LeptonChan2idx(LeptonChan::DF)]
               <<endl;
        } // icut
        oss<<"  "<<setw(8)<<endl;
    } // ireg
    } // if dilepton
    else { oss << "INVALID" << endl; }
    oss<<endl;
    
    return oss.str();
}




/* =================================================== */
//  Dump some basic information at the cut level 
//  (i.e. no specific event number in mind
/* =================================================== */
void Counter::dumpThisCut(Link* link)
{
    outfile.open(debug_name.c_str(), ios::app | ios::out);
    int eventno = link->nt->evt()->eventNumber();
    outfile<<"EventNumber: " << eventno;
    for(uint il = 0; il<link->baseLeptons->size(); il++){
        outfile << "          lep("  << il << ")  id: " << (link->baseLeptons->at(il)->isEle() ? 11 : 13) << "  q: " << link->baseLeptons->at(il)->q;
    //    outfile << "  pt: " << link->baseLeptons->at(il)->Pt() << "  eta: " << link->baseLeptons->at(il)->Eta() << "  phi: " << link->baseLeptons->at(il)->Phi();
    }
    outfile << "        sign: " << m_selector.leptonSign(link);
    outfile << endl;
    outfile.close();
    
/*    lept1Pt = link->leptons ->at(0)->Pt();
    lept1Eta = link->leptons->at(0)->Eta();
    lept1Phi = link->leptons->at(0)->Phi();
    lept2Pt = link->leptons ->at(1)->Pt();
    lept2Eta = link->leptons->at(1)->Eta();
    lept2Phi = link->leptons->at(1)->Phi();

    id0 = link->leptons ->at(0)->isEle() ? 11 : 13;
    id1 = link->leptons ->at(1)->isEle() ? 11 : 13;
    
    id0 *= link->leptons->at(0)->q;
    id1 *= link->leptons->at(1)->q;
    
    
    outfile<<"EventNumber: " << eventno;
         //  <<"  (pt0,pt1)=("<<lept1Pt<<", "<<lept2Pt<<")"
         //  <<"  (eta0,eta1)=("<<lept1Eta<<", "<<lept2Eta<<")"
         //  <<"  (phi0,phi1)=("<<lept1Phi<<", "<<lept2Phi<<")"
         //  <<"  (id0,id1)=("<<id0<<", "<<id1<<")";
    outfile<<endl;
    outfile.close();
*/
}
void Counter::dumpThisInfo(Link* link)
{
//    vector<int> me_events = { 1405480, 1406138, 458522, 1405485, 1405423,
//                                671716, 458577, 458557, 1406102, 1405606,
//                                1405608, 1405694,  671755 };
//    
    outfile.open(debug_name.c_str(), ios::app | ios::out);
    int eventno = link->nt->evt()->eventNumber();
     //   outfile<<eventno<<endl;
//    for(uint iev=0; iev<me_events.size(); iev++){
//        if(eventno==me_events[iev]){
//  //  if(find(me_events.begin(), me_events.end(), eventno)!=me_events.end()){
    float muPt, muEta, muPhi;
    for(uint imu=0; imu<link->preMuons->size(); imu++){
        Susy::Muon* mu = link->preMuons->at(imu);
      //  if(mu->isBadMuon){
            int isbad = mu->isBadMuon ? 1 : 0;
      //      int nmu = link->baseMuons->size();
     //       outfile << "nmu: " << nmu << "  ";
            muPt = mu->Pt();
            muEta = mu->Eta();
            muPhi = mu->Phi();
            outfile<<"Event: " << eventno <<"  muon pt: " << muPt<<"  muon phi: " << muPhi<<"  muon eta: " << muEta << "  isBad: "<<isbad << endl;;
      }
//    }
//    }
            outfile.close();
}
        


