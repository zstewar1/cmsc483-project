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

    sas_ic = {sz: [res[-1] for res in saserial[sz]['results']] for sz in saserial}
    sap2_ic = {sz: [res[-1] for res in saparallel['2 nodes'][sz]['results']] for sz in saparallel['2 nodes']}

    sizes = ['small', 'medium', 'large']

    plt.boxplot(list(itertools.chain.from_iterable(zip((sas_ic[sz] for sz in sizes), (sap2_ic[sz] for sz in sizes)))))

    xticks = [s[1] + ' ' + s[0] for s in itertools.product(sizes, ['Simulated Annealing (serial)', 'Simulated Annealing (2 nodes)'])]

    xtn = plt.setp(plt.axes(), xticklabels=xticks)
    plt.setp(xtn, rotation=45)

    plt.subplots_adjust(bottom=0.25)

    plt.title('Number of Iterations to find Solution')
    plt.ylabel('Iterations')

    plt.show()

if __name__ == '__main__':
    main()
