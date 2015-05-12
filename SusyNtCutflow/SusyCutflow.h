#ifndef SusyNtCutflow_SusyCutflow_h
#define SusyNtCutflow_SusyCutflow_h

// Standard
#include <iostream>
#include <string>

// SusyNtuple
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/AnalysisType.h"

// SusyNtCutflow
#include "SusyNtCutflow/Cutflow.h"
#include "SusyNtCutflow/Link.h"
#include "SusyNtCutflow/Counter.h"


namespace Susy {
    
    class SusyCutflow : public SusyNtAna
    {
        public :
        SusyCutflow();
        virtual ~SusyCutflow(){};

        /**
        Set which cutflow you would like to run
        */
        SusyCutflow& setCutflow(const Cutflow &c);

        /** 
        TSelector methods
        */
        void Begin(TTree*);
        Bool_t Process(Long64_t entry);
        void Terminate();
        
        /**
        Method to attach SusyNtuple objects in current
        entry of the TChain
        */
        void linkNtObjects(Link* link);

        /** 
        Set the analysis type
        */
        SusyCutflow& setAnalysis(const AnalysisType &a);



        protected :
        Counter         m_counter;
        std::string     APP_NAME;
        AnalysisType    m_anatype;
        Cutflow         m_cutflow;
        bool            m_verbose;

    };


} // namepsace Susy

#endif
