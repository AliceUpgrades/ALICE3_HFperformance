# ALICE3 HF performance studies

## Table of contents

* [Introduction](#introduction)
* [Overview](#overview)
  * [Execution](#execution)

## Introduction

The purpose of this repository is to collect the macros and utilities to study the performances of the ALICE3 detector with heavy-flavoured hadrons, including charmed and beauty hadrons, HF jets and quarkonia. For these studies, the ALICE3 detector setup is simulated using the [DelphesO2](https://github.com/AliceO2Group/DelphesO2) package, while the heavy-flavour reconstruction is performed using the [O<sup>2</sup>](https://github.com/AliceO2Group/AliceO2) analysis framework.

## Overview

The validation framework is a general configurable platform that gives user the full control over what is done.
Its flexibility is enabled by strict separation of its specialised components into a system of bash scripts.
Configuration is separate from execution code, input configuration is separate from task configuration, execution steps are separate from the main steering code.

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
