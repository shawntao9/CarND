#! /bin/bash

# update
sudo apt-get update

# gfortran dependency
sudo apt-get install gfortran

# get unzip
sudo apt-get install unzip

# Ipopt: get, install, unzip
wget https://www.coin-or.org/download/source/Ipopt/Ipopt-3.13.2.zip && unzip Ipopt-3.13.2.zip && rm Ipopt-3.13.21.zip
./install_ipopt.sh ./Ipopt-3.13.2

# CppAD
sudo apt-get install cppad

# Gnuplot
sudo apt-get install gnuplot

