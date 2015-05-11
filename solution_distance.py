#!/usr/bin/env python
from __future__ import print_function, division
import collections
import itertools
import json
import math
import numpy
from matplotlib import pyplot as plt

def main():
    with open('results_exhaustive.json') as file:
        exhaustive = json.load(file)

    results = collections.OrderedDict()
    with open('results_saserial.json') as file:
        results['serial'] = json.load(file)
    with open('results_saparallel.json') as file:
        res = json.load(file)
        for key in sorted(res, key=lambda x: int(x.split(maxsplit=1)[0])):
            results[key] = res[key]

    sizes = ['small', 'medium', 'large']

    correct_positions = {sz: numpy.array(exhaustive[sz]['results'][0][:2]) for sz in sizes}

    sa_positions = collections.OrderedDict()
    for nodes in results:
        sa_positions[nodes] = {sz: [numpy.array(res[:2]) for res in results[nodes][sz]['results']] for sz in sizes}

    for sz in sizes:
        distances = [[numpy.linalg.norm(pos - correct_positions[sz]) for pos in sa_positions[nodes][sz]] for nodes in sa_positions]

        plt.boxplot(distances)

        xtn = plt.setp(plt.axes(), xticklabels=list(sa_positions))
        plt.setp(xtn, rotation=90)

        plt.subplots_adjust(bottom=0.2)

        plt.title('Difference Between SA Solution and Correct Solution for ' + sz.capitalize() + ' Images')
        plt.ylabel('Euclidian Distance to Correct Solution (px)')
        plt.xlabel('Number of Nodes')

        plt.ylim(-10)

        # plt.show()
        # plt.savefig('solution_distance_{}.png'.format(sz), bbox_inches='tight')
        plt.clf()

    for sz in sizes:
        for nodes in sa_positions:
            poses = sa_positions[nodes][sz]
            x = [pos[0] for pos in poses]
            y = [pos[1] for pos in poses]
            plt.plot(x, y, 'o')
        plt.errorbar(correct_positions[sz][0], correct_positions[sz][1], fmt='+')
        plt.show()

if __name__ == '__main__':
    main()
