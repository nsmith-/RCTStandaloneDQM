#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)
import sys

run = int(sys.argv[1])

dqmFile = ROOT.TFile.Open('run%d.root' % run)

l1tPUMfolder = dqmFile.Get('DQMData/Run %d/L1T/Run summary/L1TPUM' % run)


for ieta in range(22) :
    multi = ROOT.THStack('multiplot', ';PUM Bin;Average Region Rank')

    bx0plot = l1tPUMfolder.Get('BX0/regionsPUMEta%d' % ieta)
    bx0prof = bx0plot.ProfileX('pumEta%dBX0' % ieta)
    bx0prof.SetTitle('Central BX')
    bx0prof.SetLineStyle(ROOT.kDashed)
    multi.Add(bx0prof, 'ep')

    bxp2plot = l1tPUMfolder.Get('BXP2/regionsPUMEta%d' % ieta)
    bxp2prof = bxp2plot.ProfileX('pumEta%dBXP2' % ieta)
    bxp2prof.SetTitle('Central BX + 2')
    bxp2prof.SetLineColor(ROOT.kRed)
    multi.Add(bxp2prof, 'ep')

    bxm2plot = l1tPUMfolder.Get('BXM2/regionsPUMEta%d' % ieta)
    bxm2prof = bxm2plot.ProfileX('pumEta%dBXM2' % ieta)
    bxm2prof.SetTitle('Central BX - 2')
    bxm2prof.SetLineColor(ROOT.kBlue)
    multi.Add(bxm2prof, 'ep')

    multi.Draw('nostack')
    legend = ROOT.gPad.BuildLegend(0.2, 0.7, 0.6, 0.9)
    legend.SetHeader('RCT eta = %d' % ieta)
    ROOT.gPad.Print('run%d/PUMavgRankEta%02d_PUM.png' % (run, ieta))

otherPlots = ['regionsTotalEt', 'regionsAvgEt', 'regionsAvgNonZeroEt', 'nonZeroRegions']
for plot in otherPlots :
    bxp2plot = l1tPUMfolder.Get('BXP2/%s' % plot).Clone(plot+'_BXP2')
    bx0plot = l1tPUMfolder.Get('BX0/%s' % plot).Clone(plot+'_BX0')
    bxm2plot = l1tPUMfolder.Get('BXM2/%s' % plot).Clone(plot+'_BXM2')

    multi = ROOT.THStack('multi', ';%s;%s' % (bx0plot.GetXaxis().GetTitle(), bx0plot.GetYaxis().GetTitle()))

    bx0plot.SetTitle('Central BX')
    bx0plot.SetLineStyle(ROOT.kDashed)
    multi.Add(bx0plot, 'histex0')
    bxp2plot.SetTitle('Central BX + 2')
    bxp2plot.SetLineColor(ROOT.kRed)
    multi.Add(bxp2plot, 'histex0')
    bxm2plot.SetTitle('Central BX - 2')
    bxm2plot.SetLineColor(ROOT.kBlue)
    multi.Add(bxm2plot, 'histex0')

    multi.Draw('nostack')
    if 'Avg' in plot :
        multi.GetXaxis().SetRangeUser(0,15)

    legend = ROOT.gPad.BuildLegend(0.6, 0.7, 1., 0.9)
    legend.SetHeader(plot)
    legend.SetFillColorAlpha(ROOT.kWhite, 0.)
    ROOT.gPad.Print('run%d/%s_PUM.png' % (run, plot))
