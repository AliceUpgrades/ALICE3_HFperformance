#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TCanvas.h"
#include "TDatabasePDG.h"
#include "TEfficiency.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLatex.h"
#include "TMath.h"
#include "TStyle.h"
#endif

using namespace std;

Int_t nPtBins = 0;
const Int_t nPtBinsToBeAnalyzed = 9;
Double_t ptBinLimits[nPtBinsToBeAnalyzed] = {0};

void mystyle();

void plots_2D(const char *signalfilename = "final/nopid_sig.root") {

  mystyle();

  TFile *input_signal = new TFile(signalfilename, "read");
  TCanvas *cnvSig = new TCanvas("cnvSig", "Signal fit", 1500, 1000);
  cnvSig->Divide(3, 2);

  auto dir_sig = (TDirectory *)input_signal->GetDirectory("hf-task-jpsi-mc");

  auto hPtvsEtapr0 = (TH2D *)dir_sig->Get("hPtvsEtaGenProng0");
  auto hPtvsEtapr1 = (TH2D *)dir_sig->Get("hPtvsEtaGenProng1");
  auto hPtvsEtaGen = (TH2D *)dir_sig->Get("hPtvsEtaGen");
  auto hPtvsYRec = (TH2D *)dir_sig->Get("hPtvsYRecSig");
  auto hPtvsYGen = (TH2D *)dir_sig->Get("hYGen");

  nPtBins = TMath::Min(hPtvsEtapr0->GetNbinsY(), nPtBinsToBeAnalyzed);

  for (int i = 0; i < nPtBins; i++) {
    ptBinLimits[i] = hPtvsEtapr0->GetYaxis()->GetBinLowEdge(i + 1);
    ptBinLimits[i + 1] = hPtvsEtapr0->GetYaxis()->GetBinLowEdge(i + 1) +
                         hPtvsEtapr0->GetYaxis()->GetBinWidth(i + 1);
  }

  hPtvsEtapr0->SetTitle(
      "#it{p}_{T} (J/#psi)  vs #it{p}_{T} "
      "daughter;#it{p}_{T}(prong0)(Gev/c);p_{T}(J/#psi)(Gev/#it{c}");
  hPtvsEtapr1->SetTitle(
      "#it{p}_{T} (J/#psi) vs #it{p}_{T} "
      "daughter;#it{p}_{T}(prong1)(Gev/c);p_{T}(J/#psi)(Gev/#it{c}c");
  hPtvsEtaGen->SetTitle("#it{p}_{T}  vs #eta (Gen. "
                        "Level);#eta(Gen);#it{p}_{T}(J/#psi)(Gev/#it{c}");
  hPtvsYRec->SetTitle(
      "#it{p}_{T}  vs y (rec. Level);rapidity;#it{p}_{T}(J/#psi)(Gev/#it{c}");

  cnvSig->cd(1);
  gPad->SetLogz();
  hPtvsEtapr0->Draw("colz");
  cnvSig->cd(2);
  gPad->SetLogz();
  hPtvsEtapr1->Draw("colz");
  cnvSig->cd(3);
  gPad->SetLogz();
  hPtvsEtaGen->Draw("colz");
  cnvSig->cd(4);
  gPad->SetLogz();
  // hPtvsYRec->Rebin2D(10,10,"hPtvsYRec");
  hPtvsYRec->Draw("colz");

  auto g5 = (TH2D *)hPtvsYRec->Clone();
  auto g6 = (TH2D *)hPtvsYGen->Clone();
  g5->SetTitle("distribution of efficiency;rapidity;p_{T}(J/#psi)(Gev/c");

  g5->Divide(g6);

  cnvSig->cd(5);
  // gPad->SetLogz();
  g5->Rebin2D(4, 4, "g5");
  g5->Draw("colz  ");

  TFile *fileOutEff = new TFile("eff.root", "recreate");
  hPtvsEtapr0->Write();
  hPtvsEtapr1->Write();
  hPtvsEtaGen->Write();
  hPtvsYRec->Write();

  cnvSig->SaveAs("2D_plots.pdf");
  fileOutEff->Close();
}

//====================================================================================================================================================

void mystyle() {

  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetTitleSize(0.045, "x");
  gStyle->SetTitleSize(0.045, "y");
  gStyle->SetMarkerSize(1);
  gStyle->SetLineWidth(2);
  gStyle->SetLabelOffset(0.015, "x");
  gStyle->SetLabelOffset(0.01, "y");
  gStyle->SetTitleOffset(1, "x");
  gStyle->SetTitleOffset(0.8, "y");
  gStyle->SetTextSize(0.03);
  gStyle->SetTextAlign(5);
  gStyle->SetTextColor(1);
}

//====================================================================================================================================================
