from array import array
import pandas as pd
import numpy as np
from ROOT import TH1F, TH2F, TCanvas, TGraph, TLatex, gPad, TFile, TF1
from ROOT import gStyle
from ROOT import gROOT
from ROOT import TStyle, TLegendEntry

def read_predictions(hadron = "Omega_ccc", collision = "PbPb"):
    nbins = 10
    minb = 0.
    maxb = 10.
    width = (maxb-minb)/float(nbins)
    miny = 0
    maxy = 0
    if hadron is "Omega_ccc":
        stringname = "Omega_ccc"
        latexname = "#Omega_{ccc}"
        filename = "Omega_ccc_LHC_PbPb_276TeV"
        maxy = 1e-5
        miny = 1e-9
    if hadron is "Xsi_cc":
        stringname = "Xi_cc"
        latexname = "#Xi_{cc}"
        filename = "Xsi_cc_LHC_PbPb_276TeV"
        maxy = 1e-2
        miny = 1e-6
    if hadron is "Lc":
        stringname = "Lc"
        latexname = "#Lambda_{c}"
        filename = "Lc_LHC_PbPb_276TeV"
        maxy = 1e2
        miny = 1e-4

    if collision is "PbPb":
        nevt = 1.
        energy = 2.76

    legendtext = '%s %.2f TeV, arXiv.1907.12786 (Coal.2)' % (collision, energy)
    df = pd.read_csv("../Inputs/" + filename+".csv")
    pt = df["pt"]
    cross = df["cross"]
    hist = TH1F('hist', 'hist', 100, 0, 1)

    pt_val = array('f', pt)
    cross_val = array('f', cross)
    cross_val_ptscaled = array('f',[2*3.14*a*b for a,b in zip(pt_val, cross_val)])
    n = 25
    gr = TGraph( n, pt_val, cross_val_ptscaled)
    gr.SetLineColor(1)
    gr.SetLineWidth(4)
    gr.SetMarkerColor(1)
    gr.SetMarkerStyle(21)
    gr.SetTitle('')
    gr.GetXaxis().SetTitle('p_{T} (GeV)')
    gr.GetYaxis().SetTitle('dN/dp_{T} (%s) (GeV^{-1})' % latexname)

    histo = TH1F("hpred", ";p_{T}; #Delta N/#Delta p_{T}", nbins, minb, maxb)
    norm = 0.
    for i in range(nbins-1):
      histo.SetBinContent(i+1,gr.Eval(i*width+width/2.))
      print(i+1, i*width+width/2., gr.Eval(i*width+width/2.))
      norm = norm + width*gr.Eval(i*width+width/2.)
    f = TFile("../Inputs/" + stringname+".root", "recreate")
    gr.Write()
    histo.Write()
    histo_norm = TH1F("hpred_norm", ";p_{T}; #Delta N/#Delta p_{T}", nbins, minb, maxb)
    for i in range(nbins-1):
      histo_norm.SetBinContent(i+1,histo.GetBinContent(i+1)/norm)
    histo_norm.Write()

    c1 = TCanvas("c1", "A Simple Graph Example",881,176,668,616);
    gStyle.SetOptStat(0);
    c1.SetHighLightColor(2);
    c1.Range(-1.25,-4.625,11.25,11.625);
    c1.SetFillColor(0);
    c1.SetBorderMode(0);
    c1.SetBorderSize(2);
    c1.SetLogy();
    c1.SetFrameBorderMode(0);
    c1.SetFrameBorderMode(0);
    c1.cd()
    gPad.SetLogy()
    hempty = TH2F("hempty", ";p_{T};dN/d p_{T}", 100, 0., 8., 100, miny, maxy)
    hempty.GetYaxis().SetTitle('dN/dp_{T} (%s) (GeV^{-1})' % latexname)
    hempty.GetXaxis().SetTitle("p_{T}");
    hempty.GetXaxis().SetLabelFont(42);
    hempty.GetXaxis().SetTitleOffset(1);
    hempty.GetXaxis().SetTitleFont(42);
    hempty.GetYaxis().SetLabelFont(42);
    hempty.GetYaxis().SetTitleOffset(1.35);
    hempty.GetYaxis().SetTitleFont(42);
    hempty.GetZaxis().SetLabelFont(42);
    hempty.GetZaxis().SetTitleOffset(1);
    hempty.GetZaxis().SetTitleFont(42);
    hempty.Draw()
    gr.Draw('Psame')
    histo.SetLineColor(1)
    histo.SetLineWidth(2)
    histo.Draw("same")
    tex = TLatex()
    tex.SetNDC()
    tex.SetTextFont(40)
    tex.SetTextColor(1)
    tex.SetTextSize( 0.03)
    tex.SetTextAlign(12)
    tex.DrawLatex( 0.12, 0.85, legendtext + ", dNdy=%.8f" % norm)
    #tex.Draw()
    c1.SaveAs("../Inputs/" + filename+".pdf")
    c1.SaveAs("../Inputs/" + filename+".C")

read_predictions("Omega_ccc", "PbPb")
read_predictions("Xsi_cc", "PbPb")
read_predictions("Lc", "PbPb")
