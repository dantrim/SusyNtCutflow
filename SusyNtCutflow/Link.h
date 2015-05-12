#ifndef SusyNtCutflow_Link_h
#define SusyNtCutflow_Link_h

// SusyNtuple
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/SusyNtObject.h"


namespace Susy {

    class Link {
        public :
        Link();
        virtual ~Link();
        
        bool isMC;
   
        /**
        Link to the AnalysisType corresponding cutflow 
        */ 
        AnalysisType anatype;
        /**
        Link to the SusyNtObject that we are reading in
        */
        SusyNtObject* nt;
        
        /**
        Link to the SusyNtuple objects
        */
        ElectronVector*         preElectrons;       ///< selected electrons before OR
        ElectronVector*         baseElectrons;      ///< baseline electrons
        ElectronVector*         electrons;          ///< signal electrons
        
        MuonVector*             preMuons;           ///< selected muons before OR
        MuonVector*             baseMuons;          ///< baseline muons
        MuonVector*             muons;              ///< signal muons
        
        LeptonVector*           preLeptons;         ///< selected leptons before OR
        LeptonVector*           baseLeptons;        ///< baseline leptons
        LeptonVector*           leptons;            ///< signal leptons
        
        JetVector*              preJets;            ///< selected jets before OR
        JetVector*              baseJets;           ///< baseline jets
        JetVector*              jets;               ///< signal jets

        TauVector*              baseTaus;           ///< basline taus
        TauVector*              taus;               ///< signal taus

        const Susy::Met*        met;                ///< met

    }; // class Link

} // namespace Susy

#endif
