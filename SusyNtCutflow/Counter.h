#ifndef SusyNtCutflow_Counter_h
#define SusyNtCutflow_Counter_h

// Standard
#include <iostream>
#include <string>
#include <vector>
#include <fstream>  // for debug

// SusyNtuple
#include "SusyNtuple/AnalysisType.h"

// SusyNtCutflow
#include "SusyNtCutflow/Cutflow.h"
#include "SusyNtCutflow/Selector.h"
#include "SusyNtCutflow/Link.h"

namespace Susy {

    class Counter
    {
        public :
        Counter();
        Counter& setAnalysis(const AnalysisType &a);
        Counter& setCutflow(const Cutflow &c);

        
        const std::vector<std::string> eventCutflowLabels();
        const std::vector<std::vector<std::string>> cutflowLabels(const Cutflow &c);

        /**
            Initialize the counters for the event cleaning
            cuts. Sets initial values to 0.
        */
        void constructEventCleaningCounters();
        /**
            Initialize the counters for "analysis-dependent"
            cuts. Sets initialze values to 0.
        */
        void constructCounters(const Cutflow &c);

        /**
            Returns the column of counters for the event
            flags
        */ 
        std::string retrieveEventCounters();
        
        std::string retrieveAnaCounters();

        /**
            As the event cleaning cuts are ~independent of
            the analysis and depend on bit flags, it is simple enough
            to just put them here rather than within the Selector
            class
        */
        bool pass_eventCleaning(Link* link);

        bool pass_cutflow(Link* link);
        //
        // Regions
        //
        bool pass_Stop2l_ME(Link* link);


        /**
            "Dilepton" flavor for >= 1 baselepton
        */
        void dumpThisCut(Link* link);
        void dumpThisInfo(Link* link);
        void  getLeptonFlavor(Link* link); 


        /**
            debug output file
        */
        std::fstream outfile;
        std::string debug_name = "dantrim_sigLep_May12.txt";
        // dummy
        int m_dummy;

        protected :
            AnalysisType m_anatype;
            Cutflow m_cutflow;

            /**
            Conainter to hold the labels for the various regions
            */
            std::vector<std::string> m_regions;

            /**
            Counter to hold event-cleaning counts
            */
            float m_cleaningCounters [100];

            /** 
            Basic final state topologies
            */
            bool m_dilepton;
            float m_dileptonCounters [100][100][100];
            bool m_singleLep;
            float m_singleLepCounters [100][100];
            bool m_triLep;
        
            LeptonChan m_lepchan;


            /**
            Object to implement the selections
            */
            Selector    m_selector;
            
            

    }; // class Counter

        
} // namespace Susy

#endif
