// Standard
#include <cstdlib>
#include <string>

// ROOT
#include "TChain.h"

// SusyNtCutflow
#include "SusyNtCutflow/SusyCutflow.h"

// SusyNtuple
#include "SusyNtuple/ChainHelper.h"
#include "SusyNtuple/AnalysisType.h"

using namespace std;

void help()
{
    cout << "   Options:"                            << endl;
    cout << "       TODO!"                           << endl;
    
    cout << "       -d debug printout level"         << endl;
    cout << "          defaults: 0 (quite) "         << endl; 
    
    cout << "       -i input (file, list, or dir) "  << endl;
    cout << "          defaults: ''"                 << endl;
    
    cout << "       -n number of events to process " << endl;
    cout << "          defaults: -1 (all events)"    << endl;

    cout << "       -c cutflow "                     << endl;
    cout << "          defaults: Stop2l_ME"          << endl;

    cout << "       -h print this help"              << endl;
}

//std::map<std::string, int> buildAnalysisMap()
//{
//    std::map<std::string, int> map;
//    map["stop2L"] = analysis::stop2L;
//    return map;
//}

int main(int argc, char** argv)
{
    int dbg=0;
    int nEvt=-1;
    string cutflow = "Stop2l_ME";
    string sample;
    string input;
    cout << " --------------------------- " << endl;
    cout << "   Running Cutflow           " << endl;
    cout << " --------------------------- " << endl;
    cout << endl;
    /* ============================================ */
    //  Read the user inputs
    /* ============================================ */
    // TODO: make separate function
    for(int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "-d")==0) dbg=atoi(argv[++i]);
        else if (strcmp(argv[i], "-i")==0) input=argv[++i];
        else if (strcmp(argv[i], "-s")==0) sample=argv[++i];
        else if (strcmp(argv[i], "-n")==0) nEvt=atoi(argv[++i]);
        else if (strcmp(argv[i], "-c")==0) cutflow=argv[++i];
        else {
            help();
            return 0;
        }
    }
    if(input.empty()){
        cout << "No input specified. Seek help." << endl;
        help();
        return 1;
    }
    
     
    /* ============================================ */
    //  Build the input TChain
    /* ============================================ */
    TChain* chain = new TChain("susyNt");
    ChainHelper::addInput(chain, input, dbg>0);
    Long64_t nEntries = chain->GetEntries();
    chain->ls();


    /* ============================================ */
    //  Build the Cutflow algorithm
    /* ============================================ */
 //   std::map<std::string, int> anamap = buildAnalysisMap();
    // Build the CutFlow object
    SusyCutflow* susyAna = new SusyCutflow();
  //  susyAna->setDebug(dbg);
  //  susyAna->setSampleName(sample);
   
    /* ============================================ */
    //  Set the analysis type                        
    /* ============================================ */
    susyAna->setAnalysis(AnalysisType::Ana_2Lep);
    cout << "runCutflow: Setting analysis type to " << AnalysisType2str(AnalysisType::Ana_2Lep) << endl;
    /* ============================================ */
    //  Set the cutflow
    /* ============================================ */
    if(cutflow.find("Stop2l_ME")!=string::npos) susyAna->setCutflow(Cutflow::Stop2l_ME);
    if(cutflow.find("Serhan")!=string::npos) susyAna->setCutflow(Cutflow::Serhan);
    if(cutflow.find("SUSY")!=string::npos) susyAna->setCutflow(Cutflow::SUSY);
    if(cutflow.find("EWK2L3L")!=string::npos) susyAna->setCutflow(Cutflow::EWK2L3L);

    // Run job
    if(nEvt<0) nEvt = nEntries;
    /* ============================================ */
    // output the user inputs
    /* ============================================ */  
    cout << endl;
    cout << " =================================" << endl;
    cout << " Debug:            " << dbg        << endl;
    cout << " Input:            " << input      << endl;
    cout << " Sample:           " << sample     << endl;
    cout << " Total n           " << nEntries   << endl;
    cout << " n to process      " << nEvt       << endl; 
    cout << " Cutflow           " << cutflow    << endl;
    cout << " AnalysisType      " << "Ana_2Lep" << endl;
    cout << " =================================" << endl;
    cout << endl;

    if(nEvt>0) chain->Process(susyAna,sample.c_str(), nEvt);

    cout << endl;
    cout << " --------------------------- " << endl;
    cout << "   CutFlow job done          " << endl;
    cout << " --------------------------- " << endl;

    delete chain;
    return 0;
}
    
    

    
