# Bufferbloat Problem in 5G NR

## Project Overview

This project focuses on addressing the Bufferbloat problem in 5G networks by modifying the RLC Layer in 5G NR using the NS-3 simulation tool. The goal is to efficiently support TCP and low latency traffic by introducing and evaluating innovative traffic control mechanisms.

## Team Details

- **Neel Yogendra Kansagra** (CS22MTECH11002)
- **Nikhil Kori** (CS22MTECH11014)
- **K Saravanan** (CS22MTECH12007)
- **TA**: Harinder Kaur (CS21RESCH11008)

## Problem Statement

The project aims to simulate the Bufferbloat issue in 5G and test proposed solutions to mitigate this problem. Bufferbloat refers to excessive latency caused by over-sized network buffers, which can degrade performance, especially in real-time applications.

## Project Description

Bufferbloat occurs when small and large packets are intermixed in a single queue, causing latency issues. The RLC Layer in the 3GPP model is responsible for this buffering in 5G networks. Our project modifies this layer to introduce a traffic control mechanism to manage bufferbloat effectively.

### Key Papers Referenced

1. ["Bufferbloat: Dark Buffers on the Internet"](https://dl.acm.org/doi/10.1145/2063166.2071893)
2. ["Smart Backlog Management to Fight Bufferbloat in 3GPP Protocol Stacks"](https://ieeexplore.ieee.org/document/8651727)
3. ["Dynamic Buffer Sizing and Pacing as Enablers of 5G Low-Latency Services"](https://ieeexplore.ieee.org/document/9169837)

## Implementations

### Traffic Control Layer

In NS-3, we introduced a Traffic Control Layer to emulate Linux Traffic Control Infrastructure. This layer manages packet processing actions like scheduling, dropping, and marking.

### Challenges & Solutions

- **Challenge #1**: Unable to install the root queue on gNB NetDevice due to its wireless nature.
- **Challenge #2**: Difficulty implementing the BQL Algorithm from scratch due to lack of a fixed interface.

**Preliminary Solution**: Introduced a router with BQL Algorithm and FQCodel to manage the buffer.

**Final Implementation**: Developed the Dynamic RLC Queue Limit (DRQL) Algorithm to dynamically adjust RLC buffer sizes based on traffic patterns.

### DRQL Algorithm

The DRQL Algorithm dynamically adjusts buffer limits to minimize latency and avoid buffer starvation. Key features include:
- **T**: Transmission Time Interval
- **Dequeued_bytes**: Bytes forwarded
- **Limit**: Maximum queue limit
- **Min_val**: Minimum sojourn time

### Simulation

**Test Setup**:
- One gNB with multiple UEs.
- Simulated scenarios with mice flow and elephant flow.

**Simulation Parameters**:
- **Number of UEs**: 1
- **gNB Tx Power**: 14 dBm
- **Channel Bandwidth**: 100 MHz
- **Central Frequency**: 7 GHz

**Results**:
- DRQL Algorithm improved packet loss ratio and throughput for elephant flows, while slightly increasing the delay for mice flows.

## Running the Simulation

To run the simulations, use the provided NS-3 scripts and configurations as described in the Running_readme

## Future Work

Future improvements could involve pairing the DRQL Algorithm with additional AQM algorithms to further enhance bufferbloat mitigation. Further research is needed to align performance levels with those of wired network implementations.

## Anti-Plagiarism Statement

We certify that this project is our own work, based on personal study and research. All sources and materials used have been properly acknowledged. This project has not been previously submitted for assessment.


