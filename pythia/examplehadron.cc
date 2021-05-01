// This is a simple macro to generate pythia predictions 

#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "THnSparse.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TList.h"
#include "TVector3.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TString.h"
#include "TRandom3.h"
#include "TH1D.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"  
#include <ctime>
#include <iostream> // needed for io
#include <cstdio>   // needed for io
#include <valarray>
#include <time.h>       /* time */

using namespace Pythia8;

int main(int argc, char* argv[]) {

  // WARNING: BEGINNING OF CONFIGURATION

  // Below parameters to change according to the analysis
  // parameters which will be soon read from a configuration file
  TString myhadronname = "Jpsi"; //generating events with this specific tune 
  TString myhadronlatex = "J/#psi"; //generating events with this specific tune 
  int pdgparticle = 443;  // PDG of the particle for which we want the simulation
  int maxnevents = 20000;  // max events per file/process
  int tune = 14; //Monash 2013
  int beamidA = 2212; //beam 1 proton
  int beamidB = 2212; //beam 2 proton
  double eCM = 14000; //centre-of-mass energy
  TString pythiamode = "Charmonium:all = on"; //generating events with this specific tune 
  TString outputfile = "fout_pythia8.root";

  //pt binning of charged particle and myhadron
  double nptbins = 50; 
  double ptmin = 0.;
  double ptmax = 50.;
  //ymin/max of charged particle and myhadron
  double ymin = 2.5;
  double ymax = 4.0;

  //END OF CONFIGURATION
  
  int cislo = -1;                 //unique number for each file
  cislo = atoi(argv[1]);
  int n_jobs = -1; // number of parallel jobs to be run. Be aware that each single file 
  		   // will be normalized by this number to make sure that the merged output
		   // file has the proper normalization!
  n_jobs = atoi(argv[2]);

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  pythia.readString(Form("%s", pythiamode.Data())); 
  pythia.readString(Form("Main:numberOfEvents = %d", maxnevents));
  pythia.readString("Next:numberShowEvent = 0");
  pythia.readString(Form("Tune:pp = %d", tune));
  pythia.readString(Form("Beams:idA = %d", beamidA));
  pythia.readString(Form("Beams:idB = %d", beamidB));
  pythia.readString(Form("Beams:eCM = %f", eCM));

  pythia.readString("Random:setSeed = on");
  pythia.readString(Form("Random:seed = %d",cislo));
  
  pythia.init();

  TFile *fout = new TFile(outputfile.Data(), "recreate");
 
  fout->cd();
  TH1F*hparticlept = new TH1F("hchargedparticles_pt", ";p_{T};charged particle dN/dp_{T}", nptbins, ptmin, ptmax);
  TH1F*hptyields_unnorm = new TH1F(Form("h%syieldsvspt_unnorm", myhadronname.Data()), ";p_{T} (GeV);unnormalized yield (particle+anti)", nptbins, ptmin, ptmax);
  TH1F*hptcross = new TH1F(Form("h%scrossvspt", myhadronname.Data()), Form(";p_{T} (GeV);%s d#sigma^{PYTHIA}/dp_{T} (#mu b/GeV)", myhadronlatex.Data()), nptbins, ptmin, ptmax);

  // Begin event loop. Generate event. Skip if error. List first one.
  int nmyhadron = 0;
  for (int iEvent = 0; iEvent < maxnevents; ++iEvent) {

    pythia.next();
    int nCharged = 0;
   
    for (int i = 0; i < pythia.event.size(); ++i){
      if(pythia.event[i].pT()<0 || pythia.event[i].pT()>1.e+5) continue;
      if(pythia.event[i].y()<ymin || pythia.event[i].y()>ymax) continue;

	hparticlept->Fill(pythia.event[i].pT());
	if(pythia.event[i].idAbs()==pdgparticle) {++nmyhadron;
		hptyields_unnorm->Fill(pythia.event[i].pT());
	}
       // End of event loop. Statistics. Histogram. Done.
    }
    }
     pythia.stat();
     int nbinspT=hptcross->GetNbinsX();
     double norm_fact = pythia.info.sigmaGen()*1000/(2*n_jobs*pythia.info.nAccepted());
     printf("norm fact %f\n", norm_fact);
     double contentmyhadron[nbinspT], binwidthmyhadron[nbinspT];
   
     for(int ibin=0; ibin < nbinspT; ibin++){
            contentmyhadron[ibin] = hptyields_unnorm->GetBinContent(ibin);
	    binwidthmyhadron[ibin]= hptyields_unnorm->GetBinWidth(ibin);
	    printf("bin %d, Content %f, binwidth %f\n",ibin, contentmyhadron[ibin], binwidthmyhadron[ibin]);
	    hptcross ->SetBinContent(ibin, contentmyhadron[ibin]*norm_fact/binwidthmyhadron[ibin]);
     }
  printf("nAccepted %d, nTried %d\n", pythia.info.nAccepted(), pythia.info.nTried());
  printf("pythia.info.sigmaGen() %f\n", pythia.info.sigmaGen()); 
  printf("N myhadron %d", nmyhadron);
  hptcross -> Write();
  hptyields_unnorm->Write();
  fout->Write();
  return 0;
}
