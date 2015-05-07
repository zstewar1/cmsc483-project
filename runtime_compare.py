#!/usr/bin/env python
from __future__ import print_function, division
import json
import numpy
from matplotlib import pyplot as plt

def main():
    with open('results_pciam.json') as file:
        pciam = json.load(file)
    with open('results_saserial.json') as file:
        saserial = json.load(file)
    with open('results_saparallel.json') as file:
        saparallel = json.load(file)

    x = [1,2,3]
    sizes = ['small', 'medium', 'large']

    ypciam = [numpy.average(pciam[sz]['runtimes']) for sz in sizes]
    ysas = [numpy.average(saserial[sz]['runtimes']) for sz in sizes]
    ysap2 = [numpy.average(saparallel['2 nodes'][sz]['runtimes']) for sz in sizes]

    plt.plot(x, ypciam)
    plt.plot(x, ysas)
    plt.plot(x, ysap2)
    plt.xticks(x, ['348x260', '696x520', '1392x1040'])

    plt.ylabel('Runtime (seconds)')
    plt.xlabel('Image size (Pixels)')
    plt.legend(['PCIAM', 'Simulated Annealing (serial)', 'Simulated Annealing (2 nodes)'], loc='upper left')

    plt.show()


if __name__ == '__main__':
    main()
