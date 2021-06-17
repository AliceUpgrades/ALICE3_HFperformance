void SOverBCompvsPID() {

  Int_t nBinsMerge = 9;
  Double_t xAxis1[10] = {0, 0.5, 1, 2, 3, 4, 5, 7, 10, 15};
  TString histoname, title;
  Double_t miny, maxy, ratiominx, ratiominy;
  char choice = 'A'; //A:efficiency B:bkg per events C:Significance D:signal/bkg
  
  switch (choice)
  {
      case 'A' :
          histoname = "eff";
          title = "Reconstruction Efficiency";
          miny = 0.11;
          maxy = 1.5;
          ratiominx = 0.985;
          ratiominy = 1.;
          break;
      case 'B' :
          histoname = "hBkgPerEvent";
          title = "Bkg/nEv (3#sigma)";
          miny = 2e-12;
	  maxy = 10000.;
          ratiominx = 0.;
          ratiominy = 1.;
          break;
      case 'C' :
          histoname = "histosignf";
          title = "Significance(3#sigma)";
          miny = 0.2;
          maxy = 10e5;
          ratiominx = 0.;
          ratiominy = 30;
          break;
      case 'D' :
          histoname = "histosigoverbkg";
          title = "Signal/Background (3#sigma)";
          miny = 2e-8;
          maxy = 10e3;
          ratiominx = 0.;
          ratiominy = 600;
          break;
  }

  // hBkgPerEvent eff histosignf histosigoverbkg
  TFile *filecombine = TFile::Open("combine/foutputJpsitoee.root", "READ");
  cout << "ok" << endl;
  TH1D *sig_combine = new TH1D(*((TH1D *)filecombine->Get(histoname)));

  TFile *filenopid = TFile::Open("nopid/foutputJpsitoee.root", "READ");
  cout << "ok" << endl;
  TH1D *sig_nopid = new TH1D(*((TH1D *)filenopid->Get(histoname)));

  TFile *filetof = TFile::Open("tof/foutputJpsitoee.root", "READ");
  TH1D *sig_tof = new TH1D(*((TH1D *)filetof->Get(histoname)));

  TFile *filerich = TFile::Open("rich/foutputJpsitoee.root", "READ");
  TH1D *sig_rich = new TH1D(*((TH1D *)filerich->Get(histoname)));

  int textfont = 42;
  float textsize = 0.047;
  float labelsize = 0.04;

  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(textfont);
  lat->SetTextSize(textsize);

  TLatex *lat2 = new TLatex();
  lat2->SetNDC();
  lat2->SetTextFont(textfont);
  lat2->SetTextSize(textsize * 1.0);

  TLatex *lat3 = new TLatex();
  lat3->SetNDC();
  lat3->SetTextFont(textfont);
  lat3->SetTextSize(textsize * 1.0);

  TCanvas *canvas3a = new TCanvas("canvas3a", "", 443, 100, 700, 700);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFont(textfont);
  gStyle->SetLegendTextSize(textsize);

  canvas3a->Range(0, 0, 1, 1);
  canvas3a->SetFillColor(0);
  canvas3a->SetBorderMode(0);
  canvas3a->SetBorderSize(2);
  canvas3a->SetFrameBorderMode(0);

  // ------------>Primitives in pad: canvas3a_1
  TPad *canvas3a_1 = new TPad("canvas3a_1", "canvas3a_1", 0.01, 0.3, 0.99, 0.99);
  canvas3a_1->Draw();
  canvas3a_1->cd();
  canvas3a_1->Range(-0.3750001, -2248.875, 13.375, 20249.88);
  canvas3a_1->SetFillColor(0);
  canvas3a_1->SetBorderMode(0);
  canvas3a_1->SetBorderSize(2);
  // canvas3a_1->SetGridx();
  // canvas3a_1->SetGridy();
  canvas3a_1->SetLeftMargin(0.15);
  canvas3a_1->SetFrameBorderMode(0);
  canvas3a_1->SetFrameBorderMode(0);

  cout << "ok" << endl;

  // sig_sc3->SetMarkerSize(1.25);
  // sig_sc3->SetMinimum(2.27);
  // sig_sc3->SetMaximum(2.30);
  sig_combine->SetEntries(7);
  sig_combine->SetStats(0);
  sig_combine->SetLineWidth(2);
  sig_combine->SetLineColor(1);
  // sig_sc3->SetMarkerColor(1);
  // sig_sc3->SetMarkerStyle(21);
  sig_combine->GetXaxis()->SetTitle("#it{p}_{T}(J/#psi) [GeV/#it{c}]");
  sig_combine->GetXaxis()->SetNdivisions(511);
  sig_combine->GetXaxis()->SetRangeUser(0, 15);
  sig_combine->GetXaxis()->SetLabelFont(42);
  sig_combine->GetXaxis()->SetLabelSize(0.04);
  sig_combine->GetXaxis()->SetTitleSize(0.04);
  sig_combine->GetXaxis()->SetTitleOffset(1.0);
  sig_combine->GetYaxis()->SetRangeUser(miny, maxy);
  sig_combine->GetXaxis()->SetTitleFont(42);

  // sig_sc3->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{dy dp_{T}} [#frac{#mu
  // b}{GeV/c}]");

  sig_combine->GetYaxis()->SetTitle(Form("%s",title.Data()));
  sig_combine->GetYaxis()->SetLabelFont(42);
  sig_combine->GetYaxis()->SetTitleFont(42);
  sig_combine->GetYaxis()->SetLabelSize(0.05);
  sig_combine->GetYaxis()->SetTitleSize(0.07);
  sig_combine->GetYaxis()->SetTitleOffset(0.9);
  sig_combine->GetYaxis()->SetDecimals(kTRUE);

  canvas3a_1->SetLogy();
  sig_combine->Draw("");

  sig_nopid->SetLineWidth(2);
  sig_nopid->SetLineColor(2);
  sig_tof->SetLineWidth(2);
  sig_tof->SetLineColor(3);
  sig_rich->SetLineWidth(2);
  sig_rich->SetLineColor(4);
  // sig_sc2->SetMarkerStyle(21);
  // sig_sc2->SetMarkerSize(1.25);
  // sig_sc2->SetMarkerColor(2);
  sig_nopid->Draw("same");
  sig_tof->Draw("same");
  sig_rich->Draw("same");
  lat->DrawLatex(0.2, 0.83, "ALICE3 projection, pp #sqrt{s}= 14 TeV");
  lat2->DrawLatex(0.2, 0.76, "Pythia 8, mode = 2, N_{ev}=210 10^{12}");
  lat3->DrawLatex(0.2, 0.69, "J/#psi #rightarrow e^{+}e^{-}, |y| < 1.44, BR=5.94%");
  lat3->SetTextSize(0.035);
  //  lat3->DrawLatex(0.2,0.15,"TOF with 3 #sigma_{e} for 0.15 < #it{p}_{T} < 1
  //  Gev/#it{c}"); lat3->DrawLatex(0.2,0.2,"RICH with 3 #sigma_{rad} for 0.5 <
  //  #it{p}_{T} < 15 Gev/#it{c}");

  lat3->DrawLatex(0.2, 0.2, "|n#sigma_{TOF}| < 3, 0.15 < #it{p}_{T} < 1 GeV/#it{c}");
  lat3->DrawLatex(0.2, 0.15, "|n#sigma_{RICH}| < 3, 0.15 < #it{p}_{T} < 15 GeV/#it{c}");

  /////////////////////////////////////////////////////////////////////// RATIO
  ////////////////////////////////////////////////////////////////////////

  TLegend *leg = new TLegend(0.6, 0.15, 0.9, 0.40, 0, "NDC");
  leg->AddEntry(sig_combine, "TOF + RICH", "lp");
  leg->AddEntry(sig_nopid, "w/o PID", "lp");
  leg->AddEntry(sig_tof, "TOF", "lp");
  leg->AddEntry(sig_rich, "RICH", "lp");
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  // leg->SetTextSize(22);

  leg->Draw();

  canvas3a_1->Modified();
  canvas3a->cd();

  // ------------>Primitives in pad: canvas3a_2
  TPad *canvas3a_2 = new TPad("canvas3a_2", "canvas3a_2", 0.01, 0.01, 0.99, 0.365);
  canvas3a_2->Draw();
  canvas3a_2->cd();
  canvas3a_2->Range(-0.3750001, -0.7452094, 13.375, 0.6383566);
  canvas3a_2->SetFillColor(0);
  canvas3a_2->SetBorderMode(0);
  canvas3a_2->SetBorderSize(2);
  // canvas3a_2->SetGridx();
  canvas3a_2->SetGridy();
  canvas3a_2->SetLeftMargin(0.15);
  canvas3a_2->SetBottomMargin(0.2);
  canvas3a_2->SetFrameBorderMode(0);
  canvas3a_2->SetFrameBorderMode(0);

  TH1F *SigRatio = new TH1F("SigRatio", "", 9, xAxis1);
  TH1F *SigRatio_tof = new TH1F("SigRatio_tof", "", 9, xAxis1);
  TH1F *SigRatio_rich = new TH1F("SigRatio_rich", "", 9, xAxis1);

  SigRatio->Divide(sig_combine, sig_nopid, 1., 1., "B");
  SigRatio_tof->Divide(sig_tof, sig_nopid, 1., 1., "B");
  SigRatio_rich->Divide(sig_rich, sig_nopid, 1., 1., "B");

  // SigRatio->SetMarkerStyle(1);
  // SigRatio->SetMarkerSize(1);
  SigRatio->SetMinimum(0.99);
  SigRatio->SetMaximum(1.01);
  SigRatio->SetEntries(7);
  SigRatio->SetStats(0);
  SigRatio->SetLineWidth(2);
  SigRatio_tof->SetLineWidth(2);
  SigRatio_rich->SetLineWidth(2);
  SigRatio->SetLineColor(kRed);
  SigRatio_tof->SetLineColor(kBlue);
  SigRatio_rich->SetLineColor(kBlack);
  SigRatio->GetXaxis()->SetTitle("p_{T}(J/#psi) [GeV/c]");
  // SigRatio->GetXaxis()->SetNdivisions(511);
  SigRatio->GetXaxis()->SetLabelFont(42);
  SigRatio->GetXaxis()->SetLabelSize(0.085);
  SigRatio->GetXaxis()->SetTitleSize(0.095);
  SigRatio->GetXaxis()->SetTitleOffset(1.0);
  // SigRatio->GetXaxis()->SetRangeUser(0,500);
  SigRatio->GetYaxis()->SetRangeUser(ratiominx, ratiominy);
  SigRatio->GetXaxis()->SetTitleFont(42);
  SigRatio->GetYaxis()->SetTitle("with PID / w/o PID");
  SigRatio->GetYaxis()->SetNdivisions(106);
  SigRatio->GetYaxis()->SetLabelFont(42);
  SigRatio->GetYaxis()->SetLabelSize(0.085);
  SigRatio->GetYaxis()->SetTitleSize(0.08);
  SigRatio->GetYaxis()->SetTitleOffset(0.7);
  SigRatio->GetYaxis()->SetTitleFont(42);
  SigRatio->GetYaxis()->SetDecimals(kTRUE);
  SigRatio->Draw("");
  SigRatio_tof->Draw("same");
  SigRatio_rich->Draw("same");

  TLine *line = new TLine(0, 1, 15., 1);
  line->SetLineColor(1);
  line->SetLineWidth(4);
  line->SetLineStyle(2);
  // line->Draw("same");

  TLegend *leg_ratio = new TLegend(0.65, 0.25, 0.85, 0.50, 0, "NDC");
  leg_ratio->AddEntry(SigRatio, "(TOF + RICH) / w/o PID", "lp");
  leg_ratio->AddEntry(SigRatio_tof, "TOF / w/o PID", "lp");
  leg_ratio->AddEntry(SigRatio_rich, "RICH / w/o PID", "lp");
  leg_ratio->SetFillStyle(0);
  leg_ratio->SetBorderSize(0);
  leg_ratio->SetFillColor(0);
  // leg->SetTextSize(22);

  leg_ratio->Draw();

  canvas3a_2->Modified();
  canvas3a->cd();
  canvas3a->Modified();
  canvas3a->cd();
  canvas3a->SetSelected(canvas3a);
  canvas3a->SaveAs(Form("%s.pdf",histoname.Data()));
}
