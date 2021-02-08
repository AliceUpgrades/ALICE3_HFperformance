#pylint: disable=too-many-locals,too-many-statements, missing-docstring, pointless-string-statement
from math import sqrt
from array import array
import yaml
# pylint: disable=import-error, no-name-in-module, unused-import
from ROOT import TH1F, TH2F, TCanvas, TGraph, TLatex, gPad, TFile, TF1
from ROOT import gStyle, gROOT, TStyle, TLegendEntry, TLegend

"""
read predictions from arXiv.1907.12786
"""
def scale(hadron="Omega_ccc", model="SHMC_2021", collision="PbPb", \
        brmode="central"):
    with open(r'prediction.yaml') as fileparam:
        param = yaml.load(fileparam, Loader=yaml.FullLoader)
    yieldmid = param["models"][model][collision][hadron]
    sigma_aa_b = param["statistics"][collision]["sigmaAA_b"]
    lumiaa_monthi_invnb = param["statistics"][collision]["lumiAA_monthi_invnb"]
    nevt = sigma_aa_b * lumiaa_monthi_invnb * 1e9
    bratio = param["branchingratio"][hadron][brmode]
    legendtext = '%s N_{ev}(%s) = %.1f B, BR=%.5f%%' \
            % (model, collision, nevt/1e9, bratio*100)
    scale_factor = bratio * nevt * yieldmid
    return scale_factor, legendtext, nevt

def analysis(hadron="Omega_ccc"):
    gStyle.SetOptStat(0)
    with open(r'prediction.yaml') as fileparam:
        param = yaml.load(fileparam, Loader=yaml.FullLoader)
    models = param["comparison_models"][hadron]["models"]
    collisions = param["comparison_models"][hadron]["collisions"]
    brmode = param["comparison_models"][hadron]["brmode"]
    colors = param["comparison_models"][hadron]["colors"]
    useshape = param["comparison_models"][hadron]["useshape"]
    ymin = param["comparison_models"][hadron]["ymin"]
    ymax = param["comparison_models"][hadron]["ymax"]
    activatecorr = param["do_corr"]["activate"]
    binanal = array('d', param["pt_binning"]["hadron"][hadron])

    fin = TFile("../Inputs/" + useshape +".root")
    histo_norm = fin.Get("hpred_norm")


    histolist = [None]*len(models)
    histoefflist = [None]*len(models)
    histobkglist = [None]*len(models)
    histosiglist = [None]*len(models)

    effvalue = 1.
    effarray = [effvalue]*(len(binanal)-1)
    stringeff = "%d" % effvalue
    foutput = TFile("foutput" + hadron + ".root", "recreate")
    for icase, _ in enumerate(models):
        doeff = param["do_corr"][hadron][collisions[icase]]["doeff"]
        if activatecorr is True:
            if doeff is True:
                stringeff = "simu"
                efffile = param["do_corr"][hadron][collisions[icase]]["efffile"]
                namenumeff = param["do_corr"][hadron][collisions[icase]]["namenumhist"]
                namedeneff = param["do_corr"][hadron][collisions[icase]]["namedenhist"]
                fileff = TFile(efffile)
                hnum = fileff.Get(namenumeff)
                hden = fileff.Get(namedeneff)
                hnum = hnum.Rebin(len(binanal)-1, namenumeff, binanal)
                hden = hden.Rebin(len(binanal)-1, namedeneff, binanal)
                heff = hnum.Clone("heff")
                heff.Divide(heff, hden, 1., 1., "B")
                for ibin in range(heff.GetNbinsX()-1):
                    effarray[ibin] = heff.GetBinContent(ibin+1)
                histoefflist[icase] = heff.Clone("heff_%s%s%s" % \
                        (models[icase], collisions[icase], brmode[icase]))

        histolist[icase] = histo_norm.Clone("histo_pred%s%s%s" % \
                (models[icase], collisions[icase], brmode[icase]))
        scalef, text, nevt = scale(hadron, models[icase], collisions[icase], brmode[icase])
        for ibin in range(histolist[icase].GetNbinsX()-1):
            binwdith = histolist[icase].GetBinWidth(ibin+1)
            yvalue = histolist[icase].GetBinContent(ibin+1)
            histolist[icase].SetBinContent(ibin+1, binwdith*scalef*yvalue)
        histolist[icase] = histolist[icase].Rebin(len(binanal)-1, \
                "histo_yield%s%s%s" % (models[icase], collisions[icase], \
                    brmode[icase]), binanal)
        for ibin in range(histolist[icase].GetNbinsX()-1):
            histolist[icase].SetBinContent(ibin+1, \
                effarray[ibin]*histolist[icase].GetBinContent(ibin+1))
        histolist[icase].SetLineColor(colors[icase])
        histolist[icase].SetMarkerColor(colors[icase])
        histolist[icase].SetLineWidth(2)
        histolist[icase].Draw("same")
        text = text + " Yield(tot)=%.2f, #varepsilon=%s" % (histolist[icase].Integral(), \
                stringeff)
        if activatecorr is True:
            if doeff is True:
                bkgfile = param["do_corr"][hadron][collisions[icase]]["bkgfile"]
                namehistobkg = param["do_corr"][hadron][collisions[icase]]["namehistobkg"]
                filebkg = TFile(bkgfile)
                hbkg = filebkg.Get(namehistobkg)
                histobkglist[icase] = hbkg.Clone("hbkg_%s%s%s" % \
                        (models[icase], collisions[icase], brmode[icase]))
                histobkglist[icase].Scale(nevt)
                foutput.cd()
                histoefflist[icase].Write()
                histobkglist[icase].Write()
                histosiglist[icase] = histolist[icase].Clone("hsign_%s%s%s" % \
                        (models[icase], collisions[icase], brmode[icase]))
                for ibin in range(histosiglist[icase].GetNbinsX()-1):
                    signal = histolist[icase].GetBinContent(ibin+1)
                    bkg = histobkglist[icase].GetBinContent(ibin+1)
                    histosiglist[icase].SetBinContent(ibin+1, signal/sqrt(signal+bkg))
                histosiglist[icase].Write()
        foutput.cd()
        histolist[icase].Write()
    foutput.Close()

analysis("Omega_ccc")
analysis("Omega_cc")
analysis("Xi_cc")
analysis("X3872")
