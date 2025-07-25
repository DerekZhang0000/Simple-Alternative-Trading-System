# Simple-Alternative-Trading-System

## Overview
The Simple Alternative Trading System (SATS) is a real-time equity market simulator, which uses price-time priority.
The purpose of this project is to familiarize myself with exchange microstructure.

## Progress Log
### 5/12
- Began project with building out PITCH Message class
### 5/13
- Started building out Matching Engine, Order Queue, and Order class architectures
### 5/15
- Continued building out Matching Engine and other architectures
### 5/22
- Replaced price priority queue with Boost flat map
- Replaced time priority queue with deque
### 5/26
- Created architecture diagrams and documentation
- Removed the Order Queue class
- Completed Add Order ingestion functionality
### 5/27
- Reorganized project file structure
- Replaced Makefile with CMake
- Created PITCH Add Order unit test with Boost Test
### 5/29
- Completed Cancel Order ingestion functionality
### 6/2
- Refactored unit testing compilation
- Added unit tests for Matching Engine
### 6/4
- Minor bug fixes
### 6/5
- Started Order Executed functionality
### 6/6
- Completed Order Executed functionality
- Created Data Service and Engine Router classes
### 6/11
- Completed Engine Router functionality
### 6/12
- Minor bug fixes
- Expanded Matching Engine test coverage
### 6/14
- Completed core Matching Engine functionality
- Minor bug fixes
- Resolved all memory leaks
- Expanded Matching Engine test coverage
### 6/16
- Started building out Trader Pool class
- Added last price functionality to Matching Engine
### 6/20
- Continued building out Trader Pool class
### 6/25
- Completed core Gaussian Trader Bot functionality
### 7/6
- Completed core Market Maker and Trade Messenger Bot functionality
