#!/bin/bash
# TRACK environment variables and build script (System NetCDF)
export CC=gcc
export FC=gfortran
export ARFLAGS=
export NETCDF=/usr
export PATH=${PATH}:.:~/TRACK/bin:~/TRACK/utils/bin

./master -build -i=linux -f=linux
make utils
