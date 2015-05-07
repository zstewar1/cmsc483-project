#!/usr/bin/env python
from __future__ import print_function, division
import itertools
import json
import numpy
from matplotlib import pyplot as plt

def main():
    with open('results_saserial.json') as file:
        saserial = json.load(file)
    with open('results_saparallel.json') as file:
        saparallel = json.load(file)

    sas_ic = {sz: [res[-2] for res in saserial[sz]['results']] for sz in saserial}
    sap2_ic = {sz: [res[-2] for res in saparallel['2 nodes'][sz]['results']] for sz in saparallel['2 nodes']}

    sizes = ['small', 'medium', 'large']

    plt.hist(list(itertools.chain.from_iterable(zip((sas_ic[sz] for sz in sizes), (sap2_ic[sz] for sz in sizes)))), 10, stacked=True)

    xticks = [s[1] + ' ' + s[0] for s in itertools.product(sizes, ['Simulated Annealing (serial)', 'Simulated Annealing (2 nodes)'])]

    plt.legend(xticks, loc='upper left')

    plt.title('Histogram of final NCC')
    plt.xlabel('Final NCC')

    plt.show()

if __name__ == '__main__':
    main()
