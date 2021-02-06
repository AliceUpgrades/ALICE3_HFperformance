from array import array
import pandas as pd
import numpy as np
from ROOT import TH2F, TCanvas, TGraph, TLatex, gPad, TFile, TF1, TLegend
import yaml
from ROOT import gStyle
from ROOT import gROOT
from ROOT import TStyle

def scale(hadron = "Omega_ccc", model = "SHMC_2021", collision = "PbPb", \
        brmode="central", energy = 2.76):
    with open(r'prediction.yaml') as fileparam:
        param = yaml.load(fileparam, Loader=yaml.FullLoader)
    yieldmid = param["models"][model][collision][hadron]
    sigmaAA_b = param["statistics"][collision]["sigmaAA_b"]
    lumiAA_monthi_invnb = param["statistics"][collision]["lumiAA_monthi_invnb"]
    nevt = sigmaAA_b * lumiAA_monthi_invnb * 1e9
    br = param["branchingratio"][hadron][brmode]
    enhanc = 1
    eff = 1
    if hadron is "Omega_ccc":
        stringname = "Omega_ccc"
        latexname = "\Omega_{ccc}"
    if hadron is "Xsi_cc":
        stringname = "Xi_cc"
        latexname = "\Xi_{cc}"
    if hadron is "Lc":
        stringname = "Lc"
        latexname = "\Lambda_{c}"

    filename = "%s_%s_%s_%s" % (hadron, model, collision, brmode)
    legendtext = '%s N_{ev}(%s) = %.1f B, BR=%.5f pc, eff=%.2f' \
            % (model, collision, nevt/1e9, br*100, eff)
    scale_factor =br * enhanc * eff * nevt * yieldmid;
    return scale_factor, legendtext


def analysis(hadron="Omega_ccc"):
    gStyle.SetOptStat(0)
    models = None
    collisions = None
    brmode = None
    colors = None

    if hadron=="Omega_ccc":
        models = ["SHMC_2021", "SHMC_2021", "SHMC_2021", "Stat_ChoLee_1",
                  "Stat_ChoLee_2", "Catania", "SHMC_2021", "SHMC_2021"]
        collisions = ["PbPb", "KrKr", "ArAr", "PbPb", \
                      "PbPb", "PbPb", "KrKr", "KrKr"]
        brmode = ["central", "central", "central", "central", \
                  "central", "central", "max", "min"]
        colors = [1,2,4,5,6,7,9,11]

    if hadron=="Xi_cc":
        models = ["SHMC_2021", "Stat_ChoLee_1", "Stat_ChoLee_2", "Catania"]
        collisions = ["PbPb", "PbPb", "PbPb", "PbPb"]
        brmode = ["central", "central", "central", "central"]
        colors = [1,2,4,5]

    f = TFile("../Inputs/" + hadron +".root")
    histo_norm = f.Get("hpred_norm")

    c1 = TCanvas( 'c1', 'A Simple Graph Example')
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

    hempty = TH2F("hempty", ";p_{T};#Delta N/#Delta p_{T}", 100, 0., 10., 100, 0.001, 1e10)
    hempty.GetXaxis().SetTitle("p_{T}");
    hempty.GetXaxis().SetLabelFont(42);
    hempty.GetXaxis().SetTitleOffset(1);
    hempty.GetXaxis().SetTitleFont(42);
    hempty.GetYaxis().SetTitle("#Delta N/#Delta p_{T}");
    hempty.GetYaxis().SetLabelFont(42);
    hempty.GetYaxis().SetTitleOffset(1.3);
    hempty.GetYaxis().SetTitleFont(42);
    hempty.GetZaxis().SetLabelFont(42);
    hempty.GetZaxis().SetTitleOffset(1);
    hempty.GetZaxis().SetTitleFont(42);
    hempty.Draw()
    histolist = [None]*len(models)
    scalelist = [None]*len(models)
    textlist = [None]*len(models)

    leg = TLegend(0.1293588,0.6101036,0.7457818,0.8743523,"","brNDC");
    leg.SetBorderSize(1);
    leg.SetLineColor(0);
    leg.SetLineStyle(1);
    leg.SetLineWidth(1);
    leg.SetFillColor(0);
    leg.SetFillStyle(1001);

    for icase in range(len(models)):
        histolist[icase] = histo_norm.Clone("histo_pred%d" % icase)
        scalef, text = scale(hadron, models[icase], collisions[icase], brmode[icase])
        for ibin in range(histolist[icase].GetNbinsX()-1):
            histolist[icase].SetBinContent(ibin+1, scalef*histolist[icase].GetBinContent(ibin+1))
        histolist[icase].SetLineColor(colors[icase])
        histolist[icase].SetMarkerColor(colors[icase])
        histolist[icase].Draw("same")
        leg.AddEntry(histolist[icase], text, "lep")
    leg.Draw()
    c1.SaveAs(hadron+"_results.pdf")
    c1.SaveAs(hadron+"_results.C")
analysis("Omega_ccc")
analysis("Xi_cc")
