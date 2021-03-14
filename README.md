# ALICE3 HF performance studies

## Table of contents

* [Introduction](#introduction)
* [Overview](#overview)
  * [Execution](#execution)

## Introduction

The purpose of this repository is to collect the macros and utilities to study the performances of the ALICE3 detector with heavy-flavoured hadrons, including charmed and beauty hadrons, HF jets and quarkonia. For these studies, the ALICE3 detector setup is simulated using the [DelphesO2](https://github.com/AliceO2Group/DelphesO2) package, while the heavy-flavour reconstruction is performed using the [O<sup>2</sup>](https://github.com/AliceO2Group/AliceO2) analysis framework.

## Overview

The repository now contains the following subfolders
### InputTheory

* The steering script `runtest.sh` provides control parameters and interface to the machinery for task execution.
* User provides configuration bash scripts which:
  * modify control parameters,
  * produce modified configuration files,
  * generate step scripts executed by the framework in the validation steps.

### Execution

Execution code can be found in the `exec` directory.

```bash
bash [<path>/]runtest.sh [-h] [-i <input config>] [-t <task config>] [-d]
```
