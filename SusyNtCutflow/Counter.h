#ifndef SusyNtCutflow_Counter_h
#define SusyNtCutflow_Counter_h

// Standard
#include <iostream>
#include <string>
#include <vector>

// SusyNtCutflow
#include "SusyNtCutflow/Cutflow.h"
#include "SusyNtCutflow/Selector.h"
#include "SusyNtCutflow/Link.h"

namespace Susy {

    class Counter
    {
        public :
        Counter();
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

        /**
            As the event cleaning cuts are ~independent of
            the analysis and depend on bit flags, it is simple enough
            to just put them here rather than within the Selector
            class
        */
        bool pass_eventCleaning(Link* link);
        

        protected :
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
            //std::vector<std::vector<std::vector<float>>> m_dileptonCounters;
            bool m_singleLep;
            float m_singleLepCounters [100][100];
            //std::vector<std::vector<std::vector<float>>> m_singleLepCounters;

            /**
            Cut requirements
            */
            int         m_minBaseLepIncl;     ///< minimum number of baseline leptons (greather than or equal to)
            int         m_minBaseLepExcl;     ///< minimum number of baseline leptons (greater than)
            int         m_nBaseLep;           ///< number of baseline leptons (==)
            int         m_nSigLep;            ///< number of signal leptons (==)

            /**
            Object to implement the selections
            */
            Selector    m_selector;
            
            

    }; // class Counter

        
} // namespace Susy

#endif
