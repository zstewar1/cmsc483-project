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

    sizes = ['small', 'medium', 'large']

    sa_ic = ([{sz: [res[-1] for res in saserial[sz]['results']] for sz in sizes}] +
        [{sz: [res[-1] for res in saparallel[str(nc) + ' nodes'][sz]['results']] for sz in sizes} for nc in range(2, 21)])

    xticks = ['Serial'] + [str(n) + ' Nodes' for n in range(2, 21)]
    for sz in sizes:
        plt.boxplot([i[sz] for i in sa_ic])

        xtn = plt.setp(plt.axes(), xticklabels=xticks)
        plt.setp(xtn, rotation=90)

        plt.subplots_adjust(bottom=0.2)

        plt.title('Number of Iterations for ' + sz.capitalize() + ' Images')
        plt.ylabel('Iterations')
        plt.xlabel('Number of Nodes')
        #plt.show()
        plt.savefig('res_iter_box_{}.png'.format(sz), bbox_inches='tight')
        plt.clf()

    runtime = {sz: numpy.mean([i[sz] for i in sa_ic], 1) for sz in sizes}
    std = {sz: numpy.std([i[sz] for i in sa_ic], 1) for sz in sizes}

    x = numpy.arange(len(sa_ic)) + 1

    for sz in sizes:
        # plt.plot(x, runtime[sz])
        plt.errorbar(x, runtime[sz], std[sz])

    plt.xticks(numpy.arange(0, 22, 1))
    xtn = plt.setp(plt.axes(), xticklabels=[''] + xticks)
    plt.setp(xtn, rotation=90)

    plt.xlabel('Number of Nodes')
    plt.ylabel('Runtime (Number of Iterations)')

    plt.subplots_adjust(bottom=0.2)

    plt.legend([sz.capitalize() for sz in sizes])
    plt.title('Runtime by Number of Iterations')

    plt.grid()

    #plt.show()
    plt.savefig('runtime_iterations.png', bbox_inches='tight')
    plt.clf()

    speedup = {sz: numpy.repeat(runtime[sz][0], len(runtime[sz])) / runtime[sz] for sz in sizes}

    for sz in sizes:
        plt.plot(x, speedup[sz])

    plt.xticks(numpy.arange(0, 22, 1))
    xtn = plt.setp(plt.axes(), xticklabels=[''] + xticks)
    plt.setp(xtn, rotation=90)

    plt.yticks(numpy.arange(0, 24, 1))
    plt.ylim(0, 23)

    plt.xlabel('Number of Nodes')
    plt.ylabel('Speedup')

    plt.subplots_adjust(bottom=0.2)

    plt.legend([sz.capitalize() for sz in sizes], loc='upper left')
    plt.title('Speedup by Number of Iterations')

    plt.grid()

    #plt.show()
    plt.savefig('speedup_iterations.png', bbox_inches='tight')
    plt.clf()

    efficiency = {sz: speedup[sz] / (numpy.arange(len(speedup[sz])) + 1) for sz in sizes}

    for sz in sizes:
        plt.plot(x, efficiency[sz])

    plt.xticks(numpy.arange(0, 22, 1))
    xtn = plt.setp(plt.axes(), xticklabels=[''] + xticks)
    plt.setp(xtn, rotation=90)

    plt.xlabel('Number of Nodes')
    plt.ylabel('Efficiency')

    plt.subplots_adjust(bottom=0.2)

    plt.legend([sz.capitalize() for sz in sizes], loc='upper left')
    plt.title('Efficiency by Number of Iterations')

    plt.grid()

    #plt.show()
    plt.savefig('efficiency_iterations.png', bbox_inches='tight')
    plt.clf()

if __name__ == '__main__':
    main()
