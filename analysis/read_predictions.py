from array import array
import pandas as pd
import numpy as np
from ROOT import TH1F, TCanvas, TGraph, TLatex, gPad, TFile, TF1

def read_predictions(hadron = "Omega_ccc", collision = "PbPb"):
    nbins = 10
    minb = 0.
    maxb = 10.
    width = (maxb-minb)/float(nbins)

    if hadron is "Omega_ccc":
        br = 1.
        enhanc = 1.
        eff = 1.
        stringname = "Omega_ccc"
        latexname = "\Omega_{ccc}"
        filename = "Omega_ccc_LHC_PbPb_276TeV"
    if hadron is "Xsi_cc":
        br = 1.
        enhanc = 1.
        eff = 1.
        stringname = "Xi_cc"
        latexname = "\Xi_{cc}"
        filename = "Xsi_cc_LHC_PbPb_276TeV"
    if hadron is "Lc":
        br = 1.
        enhanc = 1.
        eff = 1.
        stringname = "Lc"
        latexname = "\Lambda_{c}"
        filename = "Lc_LHC_PbPb_276TeV"

    if collision is "PbPb":
        nevt = 1.
        energy = 2.76

    legendtext = '%s %.2f TeV, N^{MB}_{ev} = %.1f B, BR=%.3f percent \
       efficiency=%.3f, %d x w.r.t. coalescence' % (collision, energy, nevt/1e9, br*100, eff, enhanc)
    scale_factor =br * enhanc * eff * nevt;

    df = pd.read_csv("../Inputs/" + filename+".csv")
    pt = df["pt"]
    cross = df["cross"]
    hist = TH1F('hist', 'hist', 100, 0, 1)

    pt_val = array('f', pt)
    cross_val = array('f', cross)
    cross_val_ptscaled = array('f',[2*3.14*scale_factor*a*b for a,b in zip(pt_val, cross_val)])
    n = 25
    gr = TGraph( n, pt_val, cross_val_ptscaled)
    gr.SetLineColor(2)
    gr.SetLineWidth(4)
    gr.SetMarkerColor(4)
    gr.SetMarkerStyle(21)
    gr.SetTitle('')
    gr.GetXaxis().SetTitle('p_{T} (GeV)')
    gr.GetYaxis().SetTitle('dN/dp_{T} (%s) (GeV^{-1})' % latexname)

    c1 = TCanvas( 'c1', 'A Simple Graph Example')
    c1.SetCanvasSize(1500, 1000)
    c1.cd()
    gPad.SetLogy()
    hempty = TH1F("hempty", "hempty", 100, 0., 1033.)
    hempty.Draw()
    gr.Draw('ACPsame')
    tex = TLatex()
    tex.SetNDC()
    tex.SetTextFont(62)
    tex.SetTextColor(1)
    tex.SetTextSize( 0.03)
    tex.SetTextAlign(12)
    tex.DrawLatex( 0.12, 0.85, legendtext)
    tex.Draw()
    c1.SaveAs("../Inputs/" + filename+".pdf")

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
    print(norm, latexname)
read_predictions("Omega_ccc", "PbPb")
read_predictions("Xsi_cc", "PbPb")
read_predictions("Lc", "PbPb")
