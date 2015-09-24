#ifndef SusyNtCutflow_Cutflow_h
#define SusyNtCutflow_Cutflow_h

#include <string>


namespace Susy
{
    enum class Cutflow {
        Stop2l_ME=0 // cutflow with Marialea 
        ,Serhan // cutflow with Serhan
        ,SUSY // cutflow for SUSY Analsysis at [1]
        ,kUnknown
    };
    std::string Cutflow2str(const Cutflow &c);
    size_t Cutflow2idx(const Cutflow &c);

    enum class LeptonChan {
        E=0,
        M,
        EE,
        MM,
        EM,
        ME,
        SF,
        DF,
        EVT,
        kUnknown
    };
    std::string LeptonChan2str(const LeptonChan &c);
    size_t  LeptonChan2idx(const LeptonChan &c);
        
}

#endif


// [1] https://docs.google.com/spreadsheets/d/1LNAzqtr4PY1OGIRgYxDQ6WJERN5_AdSF3oYB1GeJI0M/edit?pli=1#gid=1666199485
