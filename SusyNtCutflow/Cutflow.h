#ifndef SusyNtCutflow_Cutflow_h
#define SusyNtCutflow_Cutflow_h

#include <string>


namespace Susy
{
    enum class Cutflow {
        Stop2l_ME=0
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
