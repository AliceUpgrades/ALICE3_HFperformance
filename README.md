# ALICE3 HF performance studies

## Table of contents

* [Introduction](#introduction)
* [Overview](#overview)
  * [Execution](#execution)

## Introduction

The purpose of this repository is to collect the macros and utilities to study the performances of the ALICE3 detector with heavy-flavoured hadrons, including charmed and beauty hadrons, HF jets and quarkonia. For these studies, the ALICE3 detector setup is simulated using the [DelphesO2](https://github.com/AliceO2Group/DelphesO2) package, while the heavy-flavour reconstruction is performed using the [O<sup>2</sup>](https://github.com/AliceO2Group/AliceO2) analysis framework.

## Overview

The repository now contains the following subfolders:

### InputTheory
This folder contains p<sub>T</sub>-dependent predictions for charmed and multi-charm baryons. In particular:
  * predictions in csv from the coalescence calculations based on arXiv.1907.12786 in PbPb collisions at 2.76 TeV. These inputs can be converted in ROOT histograms by using the following [script](https://github.com/AliceUpgrades/ALICE3_HFperformance/blob/main/analysis/read_predictions_ptdep_stat_cholee_2_pbpb2p76_absy0p5.py), as discussed later.
  * predictions for the Λ<sub>c</sub> baryons in pp collisions from Pythia

### InputExp
This folder contains the experimental inputs for performing significance estimations for the different hadrons in both pp and AA collisions. 
The name of the folder indicates whether these studies were performed with aliphysics or with the fast-simulation method available within the O<sup>2</sup> analysis framework. Each folder typically contains:
  * a histogram with the background content within 3 sigma from the peak mean normalized by the number of events extracted in MB Monte Carlo simulations
  * a histogram with the efficiencies for each channel. 

### Execution

Execution code can be found in the `exec` directory.

```bash
bash [<path>/]runtest.sh [-h] [-i <input config>] [-t <task config>] [-d]
```
