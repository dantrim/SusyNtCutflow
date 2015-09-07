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
        case(AnalysisType::Ana_Stop2L) : {
            m_dilepton = true;
            break;
        }
        case(AnalysisType::kUnknown) : {
            cout << "WARNING Counter::setAnalysis: AnalysisType::kUnknown" << endl;
            cout << "WARNING    --> Exitting." << endl;
            exit(1);
            break;
        }  
    } // end switch

    m_anatype = a;

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
    // good vtx
    labels.push_back("good vertex");
    // bad muon
    labels.push_back("bad muon");
    // cosmic muons
    labels.push_back("cosmic muon");
    // jet cleaning
    labels.push_back("jet cleaning");

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
    // SerhanCheck
    // (Cutflow with Serhan)
    //////////////////////////////////////
    case(Cutflow::Serhan) : {
        //
        // OS selection
        //
        m_regions.push_back("Serhan-OS");
        std::vector<std::string> os_labels;
        os_labels.push_back(">=1 baseline lepton");
        os_labels.push_back("==2 baseline leptons");
        os_labels.push_back("==2 signal leptons");
        os_labels.push_back("mll>20 GeV");
        os_labels.push_back("met>40 GeV");
        os_labels.push_back("R1>0.2");
        os_labels.push_back("mT2>20.0 GeV");
        os_labels.push_back("|deltaX|<0.05");
        os_labels.push_back("R2>0.5");
        os_labels.push_back("|cosThetaB|<0.8");
        labels.push_back(os_labels);
        //
        // SS selection
        //
        m_regions.push_back("Serhan-SS");
        std::vector<std::string> ss_labels;
        ss_labels.push_back(">=1 baseline lepton");
        ss_labels.push_back("==2 baseline leptons");
        ss_labels.push_back("==2 signal leptons");
        ss_labels.push_back("mll>20 GeV");
        ss_labels.push_back("met>40 GeV");
        ss_labels.push_back("R1>0.2");
        ss_labels.push_back("mT2>20.0 GeV");
        ss_labels.push_back("|deltaX|<0.05");
        ss_labels.push_back("R2>0.5");
        ss_labels.push_back("|cosThetaB|<0.8");
        labels.push_back(ss_labels);
        break;
    } // end case Serhan

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
    // SerhanCheck
    // Cutflow with Serhan
    //////////////////////////////////////
        case(Cutflow::Serhan) : {
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
    // Counter::dumpThisCut(link);

    int flags = link->nt->evt()->cutFlags[NtSys::SusyNtSys::NOM]; 
    // grl
    if(!link->tools->passGRL(flags))                    return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;
    // lar error
    if(!link->tools->passLarErr(flags))                 return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;
    // tile error
    if(!link->tools->passTileErr(flags))                return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;
    // ttc veto
    if(!link->tools->passTTC(flags))                    return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;
    // good vertex
    if(!link->tools->passGoodVtx(flags))                return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;
    // bad muon
    if(!link->tools->passBadMuon(*link->preMuons))       return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;
    // cosmic muon
    if(!link->tools->passCosmicMuon(*link->baseMuons))   return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;
    // jet cleaning
    if(!link->tools->passJetCleaning(*link->baseJets))    return false;
    m_cleaningCounters[iEventCut]++;                    iEventCut++;

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
        // Serhan
        //////////////////////////////////////
        case(Cutflow::Serhan) : {
            if(!pass_SerhanCutflow(link)) pass = false;
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
    MuonVector tmpMuons;
    for(uint im=0; im<link->baseMuons->size(); im++){
        if(fabs(link->baseMuons->at(im)->Eta())<2.4) tmpMuons.push_back(link->baseMuons->at(im));
    }
    std::sort(tmpMuons.begin(), tmpMuons.end(), comparePt);
    link->baseMuons->clear();
    link->baseMuons = &tmpMuons;

    MuonVector tmpSigMuons;
    for(uint im=0; im<link->muons->size(); im++){
        if(fabs(link->muons->at(im)->Eta())<2.4) tmpSigMuons.push_back(link->muons->at(im));
    }
    std::sort(tmpSigMuons.begin(), tmpSigMuons.end(), comparePt);
    link->muons->clear();
    link->muons = &tmpSigMuons;

    LeptonVector tmpBaseLeps;
    LeptonVector tmpSigLeps;
    link->tools->buildLeptons(tmpBaseLeps, *link->baseElectrons, *link->baseMuons);
    link->tools->buildLeptons(tmpSigLeps, *link->electrons, *link->muons);
    link->baseLeptons->clear();
    link->baseLeptons = &tmpBaseLeps;
    link->leptons->clear();
    link->leptons = &tmpSigLeps;

    for(uint ireg=0; ireg < cutflowLabels(m_cutflow).size(); ireg++){
        int iCut=0;
        m_selector.setCutflow(m_cutflow).buildRegion(ireg);
        if(ireg==0) { // bypass the fact that the OS and SS are different regions and just fill their respective counters based on the signedness 

            //Counter::dumpThisCut(link);

            if(!m_selector.pass_minNBase(link)) return false; 
            getLeptonFlavor(link);
            int same_sign = m_selector.leptonSign(link);
            if(same_sign)          m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            else if(!same_sign)    m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
            iCut++;
            //if(!same_sign && m_lepchan==LeptonChan::DF)
            //Counter::dumpThisCut(link);
            
            if(!m_selector.pass_baseNLep(link)) return false;
            getLeptonFlavor(link);
            same_sign = m_selector.leptonSign(link);
            if(same_sign)  m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
            iCut++;

            if(!m_selector.pass_sigNLep(link)) return false;
            if(same_sign)  m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
            iCut++;

            if(!m_selector.pass_minMll(link)) return false;
            if(same_sign)  m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
            iCut++;

            if(!m_selector.pass_minMet(link)) return false;
            if(same_sign)  m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
            if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
            iCut++;
           
            /// dump with Serhan Sep 3 
            //if(m_lepchan==LeptonChan::DF && !same_sign) Counter::dumpThisCut(link);

        } // ireg==0
    }
    return true;
}
/* =================================================== */
//  Cuts for cutflow with Serhan
/* =================================================== */
bool Counter::pass_SerhanCutflow(Link* link)
{
    MuonVector tmpMuons;
    for(uint im=0; im<link->baseMuons->size(); im++){
        if(fabs(link->baseMuons->at(im)->Eta())<2.4) tmpMuons.push_back(link->baseMuons->at(im));
    }
    std::sort(tmpMuons.begin(), tmpMuons.end(), comparePt);
    link->baseMuons->clear();
    link->baseMuons = &tmpMuons;

    MuonVector tmpSigMuons;
    for(uint im=0; im<link->muons->size(); im++){
        if(fabs(link->muons->at(im)->Eta())<2.4) tmpSigMuons.push_back(link->muons->at(im));
    }
    std::sort(tmpSigMuons.begin(), tmpSigMuons.end(), comparePt);
    link->muons->clear();
    link->muons = &tmpSigMuons;

    LeptonVector tmpBaseLeps;
    LeptonVector tmpSigLeps;
    link->tools->buildLeptons(tmpBaseLeps, *link->baseElectrons, *link->baseMuons);
    link->tools->buildLeptons(tmpSigLeps, *link->electrons, *link->muons);
    link->baseLeptons->clear();
    link->baseLeptons = &tmpBaseLeps;
    link->leptons->clear();
    link->leptons = &tmpSigLeps;

    for(uint ireg=0; ireg < cutflowLabels(m_cutflow).size(); ireg++) {
        int iCut=0;
        m_selector.setCutflow(m_cutflow).buildRegion(ireg);
        if(ireg==0) { // bypass the fact that the OS and SS are different regions
                if(!m_selector.pass_minNBase(link)) return false; 
                getLeptonFlavor(link, false);
                int same_sign = m_selector.leptonSign(link);
                if(same_sign)          m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                else if(!same_sign)    m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;

                if(!m_selector.pass_baseNLep(link)) return false;
                getLeptonFlavor(link, false);
                same_sign = m_selector.leptonSign(link);
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;
                
                if(!m_selector.pass_sigNLep(link)) return false;
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;

                if(!m_selector.pass_minMll(link)) return false;
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++; 

                if(!m_selector.pass_minMet(link)) return false;
                if(same_sign)  m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;

                if(!m_selector.pass_R1(link, 0.2, true)) return false;
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;

                if(!m_selector.pass_mt2(link, 20, true)) return false;
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;

                if(!m_selector.pass_deltaX(link, 0.05, false)) return false;
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;

                if(!m_selector.pass_R2(link, 0.5, true)) return false;
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;

                if(!m_selector.pass_cosThetaB(link, 0.8, false)) return false;
                if(same_sign) m_dileptonCounters[1][iCut][LeptonChan2idx(m_lepchan)]++;
                if(!same_sign) m_dileptonCounters[0][iCut][LeptonChan2idx(m_lepchan)]++;
                iCut++;
        } // if ireg == 0
    } // ireg

    return true;
}
/* =================================================== */
//  Get the "dilepton" flavor for >= 1 baselepton
/* =================================================== */
void Counter::getLeptonFlavor(Link* link, bool useSignal)
{
    if(!useSignal) {
        if(link->baseLeptons->size()==1) m_lepchan = LeptonChan::SF;
        else{
            int nlep = link->baseLeptons->size();
            int ne   = link->baseElectrons->size();
            int nm   = link->baseMuons->size();
            if(ne==0 && nlep>1) m_lepchan = LeptonChan::SF;
            else if(nm==0 && nlep>1) m_lepchan = LeptonChan::SF;
            else if(nlep>1) m_lepchan = LeptonChan::DF;
        }
    } // !useSignal
    else {
        if(link->leptons->size()==1) m_lepchan = LeptonChan::SF;
        else {
            int nlep = link->leptons->size();
            int ne = link->electrons->size();
            int nm = link->muons->size();
            if(ne==0 && nlep>1) m_lepchan = LeptonChan::SF;
            else if(nm==0 && nlep>1) m_lepchan = LeptonChan::SF;
            else if(nlep>1) m_lepchan = LeptonChan::DF;
        }
    } // useSignal
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
    int eventno = link->nt->evt()->eventNumber;
    if(eventno== 7631994) {
        outfile << eventno << "    " << endl;
        for(uint i = 0; i < link->baseJets->size(); i++){
            outfile << "    " << "j[" << i << "]   pt: " << link->baseJets->at(i)->Pt() << "  eta: " << link->baseJets->at(i)->Eta() << "  phi: " << link->baseJets->at(i)->Phi() << endl;
        }
    /*
        for(uint il=0; il<link->preLeptons->size(); il++){
            Susy::Lepton ilep = *link->preLeptons->at(il);
            int iid = ilep.isEle() ? 11 : 13;
            int iq = ilep.q;
            iid *= iq;
            for(uint jl=il+1; jl<link->preLeptons->size(); jl++){        
                Susy::Lepton jlep = *link->preLeptons->at(jl);
                int jid = jlep.isEle() ? 11 : 13;
                int jq = jlep.q;
                jid *= jq;
                outfile << "    " << "lep["<<il<<","<<jl<<"]  dRy=" << ilep.DeltaRy(jlep) << "  (id1, id2) = (" << iid << "," << jid<<")" <<endl; 
            } // jl
            for(uint ijet=0; ijet<link->preJets->size(); ijet++){
                Susy::Jet jet = *link->preJets->at(ijet);
                outfile << "    " << "lepjet["<<il<<","<<ijet<<"]  dRy=" << ilep.DeltaRy(jet) << "  (idlep, jpt) = (" << iid << ", " << jet.Pt() << ")" << endl;
            } // ijet
        } // il
    */

   //     for(uint il = 0; il<link->baseLeptons->size(); il++){ 
   //         int id = link->baseLeptons->at(il)->isEle() ? 11 : 13;
   //         int q = link->baseLeptons->at(il)->q;
   //         id *= q;
   //         float pt = link->baseLeptons->at(il)->Pt();
   //         float eta = link->baseLeptons->at(il)->Eta();
   //         outfile << "lep("<<il<<") id: " << id<<"  (pt,eta):("<<pt<<", "<<eta <<")  | ";
   //     }
        outfile << endl;
    }
 //   for(uint il = 0; il<link->baseLeptons->size(); il++){
 //       int id = link->baseLeptons->at(il)->isEle() ? 11 : 13;
 //       int q = link->baseLeptons->at(il)->q;
 //       id *= q;
 //       outfile << " lep("<<il<<") id: "<<id<<"  (pt,eta):("<<link->baseLeptons->at(il)->Pt()<<","<<link->baseLeptons->at(il)->Eta()<<") | ";
 //   }
 //   outfile << endl;
    outfile.close();
    
}
void Counter::dumpThisInfo(Link* link)
{
//    vector<int> me_events = { 8058946, 8028797, 8029450 };
// 
//    outfile.open(debug_name.c_str(), ios::app | ios::out);
//    int eventno = link->nt->evt()->eventNumber;
//    for(uint iev=0; iev<me_events.size(); iev++){
//        if(eventno == me_events[iev]){
//            outfile << "eventno: " << eventno << endl;
//            for(uint imu = 0; imu<link->preMuons->size(); imu++){
//                Susy::Muon* mu = link->preMuons->at(imu);
//                float pt, eta, phi;
//                bool base, bad, cosmic;
//                pt = mu->Pt();
//                eta = mu->Eta();
//                phi = mu->Phi();
//                base = mu->isBaseline;
//                bad = mu->isBadMuon;
//                cosmic = mu->isCosmic;
//                outfile << "      mu[" << imu << "]  pt: " << pt << "  eta: " << eta;
//                outfile        << "  phi: " << phi << "  baseline: " << base << "  bad: " << bad;
//                outfile        << "  cosmic: " << cosmic << endl;
//            } //imu
//        }
//    } // ev

    //vector<int> me_events = {8028653, 7586123};
    vector<int> me_events = {7631994};
    int eventno = link->nt->evt()->eventNumber;
    for(uint iev=0; iev<me_events.size(); iev++){
    if(eventno == me_events[iev]){
        outfile.open(debug_name.c_str(), ios::app | ios::out); 
        outfile<<"eventno: " << eventno << endl;

        // get the dudes prior to OR
        ElectronVector preElectrons = *link->preElectrons;
        MuonVector preMuons         = *link->preMuons;
        JetVector preJets           = *link->preJets;
        TauVector preTaus           ;

        ElectronVector baseElectrons    ;
        MuonVector     baseMuons        ;
        JetVector baseJets              ;
        TauVector baseTaus              ;

        link->tools->getBaselineObjects(preElectrons, preMuons, preJets, preTaus,
                                        baseElectrons, baseMuons, baseJets, baseTaus);

        LeptonVector baseLeps;
        link->tools->buildLeptons(baseLeps, baseElectrons, baseMuons);
        for(uint im=0; im<preMuons.size();im++){
            Susy::Muon* mu = preMuons[im];
            float pt = mu->Pt();
            float eta = mu->Eta();
            float phi = mu->Phi();
            bool isBase = link->tools->m_muonSelector.isBaselineMuon(mu);
            outfile<<"       mu["<<im<<"]   pt: " << pt << "  eta: " << eta << "  phi: " << phi << "  base: " << isBase << endl; 
        } // im

//        for(uint il=0; il<baseLeps.size(); il++){
//            Susy::Lepton* ilep = baseLeps[il];
//            int iid = ilep->isEle() ? 11 : 13;
//            iid *= ilep->q;
//            float ipt = ilep->Pt();
//            float ieta = ilep->Eta(); 
//            float iphi = ilep->Phi();
//            float irap = ilep->Rapidity();
//        //    for(uint ij=0; ij<baseJets.size(); ij++){
//        //        Susy::Jet* j = baseJets[ij];
//        //        float jpt = j->Pt();
//        //        float jeta = j->Eta();
//        //        float jphi = j->Phi();
//        //        float jrap = j->Rapidity();
//        //        float dR = ilep->DeltaR(*j);
//        //        float dphi = TVector2::Phi_mpi_pi(jphi-iphi);
//        //        float drap = jrap - irap;
//        //        float dRy = TMath::Sqrt(dphi*dphi + drap*drap);
//        //        outfile<<"    deltaRy(lep["<<il<<"],jet["<<ij<<"]) = "<<dRy<<endl;
//        //        outfile<<"                lep id: " << iid << " (pt,eta,phi)=("<<ipt<<","<<ieta<<","<<iphi<<")   jet (pt,eta,phi)=("<<jpt<<","<<jeta<<","<<jphi<<")"<<endl;
//            for(uint jl=il+1; jl<baseLeps.size(); jl++){
//                Susy::Lepton* jlep = baseLeps[jl];
//                int jid = jlep->isEle() ? 11 : 13;
//                jid *= jlep->q;
//                float jpt = jlep->Pt();
//                float jeta = jlep->Eta();
//                float jphi = jlep->Phi();
//                float jrap = jlep->Rapidity();
//
//               // float dphi = iphi - jphi;
//                float dphi = TVector2::Phi_mpi_pi(iphi-jphi);
//                float drap = irap - jrap;
//                float dRy = TMath::Sqrt(dphi*dphi + drap*drap);
//
//                float dR = ilep->DeltaR(*jlep);
//                outfile<<"    lep("<<il<<"):  pt: " << ipt << "  eta: " << ieta << "  phi: " << iphi << "  rap: "<< irap <<"  id: " << iid << endl;
//                outfile<<"    lep("<<jl<<"):  pt: " << jpt << "  eta: " << jeta << "  phi: " << jphi << "  rap: "<<jrap <<"  id: " << jid << endl;
//                outfile<<"    deltaR("<<il<<","<<jl<<") = "<<dR<<"  deltaRy = " << dRy << endl;
//            } // jl
//        } // il
//
//        link->tools->m_overlapTool.performOverlap(baseElectrons, baseMuons,baseTaus, baseJets);
//        for(uint ie=0; ie<baseElectrons.size(); ie++){
//            outfile << "POST OR"<<endl;
//            outfile << "POST OR"<<endl;
//            outfile << "POST OR"<<endl;
//            outfile<<"     ele["<<ie<<"]   (pt,eta,phi) = (" << baseElectrons[ie]->Pt() << ","<<baseElectrons[ie]->Eta()<<","<<baseElectrons[ie]->Phi() <<")" << endl;
//        }
//        for(uint ij=0;ij<baseJets.size();ij++){
//            outfile <<"     jet["<<ij<<"]   (pt,eta,phi) = (" << baseJets[ij]->Pt() << ","<<baseJets[ij]->Eta()<<","<<baseJets[ij]->Phi() <<")" << endl;
//        }
                
        
        outfile.close();
    } // eventno
    } // iev
}
        


