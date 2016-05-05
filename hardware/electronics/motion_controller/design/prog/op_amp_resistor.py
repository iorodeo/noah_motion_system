# op_amp_resistor.py
#
# Simple program to help select resistors for differential amplifier bias
# network.  Resistor relationship is as follows:
#
# 1/Rin + 1/Rg = 1/Rf
#
# where
#
# Rin = resistor(s) on inputs (+ and -)
# Rg  = resistor to ground on + leg
# Rf  = feedback resistor
#
# Notes: 
# * assumes the three input resistors are equal.
# * selecting Rin and Rf allows us to calculate Rg. 
#
# -----------------------------------------------------------------------------
from __future__ import print_function

def calcRg(Rin, Rf):
    Rg = 1.0/( 1.0/Rf - 1.0/Rin)
    return Rg


if __name__ == '__main__':

    Rin = 49900.0
    #Rin = 50000.0
    Rf  = 3000.0
    Rg = calcRg(Rin,Rf)

    print('Rin: {0}'.format(Rin))
    print('Rf:  {0}'.format(Rf))
    print('Rg:  {0}'.format(Rg))

