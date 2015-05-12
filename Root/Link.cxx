#include "SusyNtCutflow/Link.h"

using namespace Susy;

/* ======================================== */
//  Constructor
/* ======================================== */
Link::Link() :
    isMC(false),
    anatype(AnalysisType::kUnknown),
    nt(NULL),
    preElectrons(NULL),
    baseElectrons(NULL),
    electrons(NULL),
    preMuons(NULL),
    baseMuons(NULL),
    muons(NULL),
    preLeptons(NULL),
    baseLeptons(NULL),
    leptons(NULL),
    preJets(NULL),
    baseJets(NULL),
    jets(NULL),
    baseTaus(NULL),
    taus(NULL),
    met(NULL)
{
}
/* ======================================== */
//  Destructor
/* ======================================== */
Link::~Link()
{
    isMC=false;
    anatype = AnalysisType::kUnknown;
    nt=NULL;
    preElectrons=NULL;
    baseElectrons=NULL;
    electrons=NULL;
    preMuons=NULL;
    baseMuons=NULL;
    muons=NULL;
    preLeptons=NULL;
    baseLeptons=NULL;
    leptons=NULL;
    preJets=NULL;
    baseJets=NULL;
    jets=NULL;
    baseTaus=NULL;
    taus=NULL;
    met=NULL;
}
    
    
