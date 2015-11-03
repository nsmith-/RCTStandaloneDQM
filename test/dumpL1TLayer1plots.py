#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)
import sys

run = int(sys.argv[1])

dqmFile = ROOT.TFile.Open('outputs/run%d.root' % run)

l1tLayer1folder = dqmFile.Get('DQMData/Run %d/L1TEMU/Run summary/L1TLayer1/Occupancies' % run)
items = (k.ReadObj() for k in l1tLayer1folder.GetListOfKeys())

ROOT.gROOT.ProcessLine(".L newRct.C+")
ROOT.gROOT.ProcessLine("setTDRStyle()")

def plot2D(plot) :
    plot.Draw('box')
    ROOT.gPad.Print('run%d/%s_L1TLayer1.png' % (run, plot.GetName()))

def plot1D(plot) :
    plot.Draw()
    ROOT.gPad.SetLogx(True)
    ROOT.gPad.SetLogy(True)
    ROOT.gPad.Print('run%d/%s_L1TLayer1.png' % (run, plot.GetName()))
    ROOT.gPad.SetLogx(False)
    ROOT.gPad.SetLogy(False)

for item in items :
    if type(item) is ROOT.TH2F :
        plot2D(item)
    elif type(item) is ROOT.TH1F :
        plot1D(item)

