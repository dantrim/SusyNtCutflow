// Standard
#include <iostream>

// SusyNtCutflow
#include "SusyNtCutflow/Cutflow.h"

using namespace std;

namespace Susy {

    string Cutflow2str(const Cutflow &c)
    {
        string s = "Unknown";
        switch(c){
            case Cutflow::Stop2l_ME     : s = "Stop2l_ME"          ; break;
            case Cutflow::Serhan        : s = "Serhan"             ; break;
            case Cutflow::EWK2L3L       : s = "EWK2L3L"            ; break;
            case Cutflow::kUnknown      : s = "Invalid Cutflow"    ; break;
        }
        return s;
    }
    size_t Cutflow2idx(const Cutflow &c)
    {
        size_t idx;
        switch(c) {
            case Cutflow::Stop2l_ME     : idx = 0      ; break;
            case Cutflow::Serhan        : idx = 1      ; break;
            case Cutflow::EWK2L3L       : idx = 2      ; break;
            case Cutflow::kUnknown      : idx = 3      ; break;
        }
        return idx;
    }

    string LeptonChan2str(const LeptonChan &c)
    {
        string s = "Unknown";
        switch(c){
            case LeptonChan::E          : s = "e"       ; break;
            case LeptonChan::M          : s = "m"       ; break;
            case LeptonChan::EE         : s = "ee"      ; break;
            case LeptonChan::MM         : s = "mm"      ; break;
            case LeptonChan::EM         : s = "em"      ; break;
            case LeptonChan::ME         : s = "me"      ; break;
            case LeptonChan::SF         : s = "sf"      ; break;
            case LeptonChan::DF         : s = "df"      ; break;
            case LeptonChan::EVT        : s = "Event"   ; break;
            case LeptonChan::kUnknown : {
                cout << "Cutflow::LeptonChan2str error. Unknown channel id." << endl;
                break;
            }
        } // switch
        size_t isUnknown = s.find("Unknown");
        if(isUnknown != string::npos) { cout << "   --> Exitting." << endl; exit(1); }
        else { return s; }
    }
    size_t LeptonChan2idx(const LeptonChan &c)
    {
        size_t idx = 100;
        switch(c){
            case LeptonChan::E          : idx = 0       ; break;
            case LeptonChan::M          : idx = 1       ; break;
            case LeptonChan::EE         : idx = 2       ; break;
            case LeptonChan::MM         : idx = 3       ; break;
            case LeptonChan::EM         : idx = 4       ; break;
            case LeptonChan::ME         : idx = 5       ; break;
            case LeptonChan::SF         : idx = 6       ; break;
            case LeptonChan::DF         : idx = 7       ; break;
            case LeptonChan::EVT        : idx = 8       ; break;
            case LeptonChan::kUnknown : {
                cout << "Cutflow::LeptonChan2idx error. Index for lepton channel not chosen correctly." << endl;
                break;
            }
        }
        if(idx>99) { cout << "  --> Exitting." << endl; exit(1); }
        else {  return idx; }
    }

} // namespace Susy
