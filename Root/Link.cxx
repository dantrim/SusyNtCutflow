#include "SusyNtCutflow/Link.h"

using namespace Susy;

/* ======================================== */
//  Constructor
/* ======================================== */
Link::Link() :
    isMC(false),
    anatype(AnalysisType::kUnknown),
    nt(NULL),
    tools(NULL),
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
    preTaus(NULL),
    baseTaus(NULL),
    taus(NULL),
    prePhotons(NULL),
    basePhotons(NULL),
    photons(NULL),
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
    tools=NULL;
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
    preTaus=NULL;
    baseTaus=NULL;
    taus=NULL;
    prePhotons=NULL;
    basePhotons=NULL;
    photons=NULL;
    met=NULL;
}
    
    
