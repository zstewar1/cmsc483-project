#!/usr/bin/env python
from __future__ import print_function, division
import collections
import itertools
import json
import math
import numpy
from matplotlib import pyplot as plt

def main():
    results = collections.OrderedDict()
    with open('results_exhaustive_memory.json') as file:
        results['exhaustive'] = json.load(file)
    with open('results_saserial_memory.json') as file:
        results['serial'] = json.load(file)
    with open('results_saparallel_memory.json') as file:
        res = json.load(file)
        for key in sorted(res, key=lambda x: int(x.split(maxsplit=1)[0])):
            results[key] = res[key]
    with open('results_pciam_memory.json') as file:
        results['pciam'] = json.load(file)

    sizes = ['small', 'medium', 'large']

    means = {sz: numpy.array([numpy.mean(mem_uses[sz]) for mem_uses in results.values()]) for sz in sizes}
    stds = {sz: numpy.array([numpy.std(mem_uses[sz]) for mem_uses in results.values()]) for sz in sizes}

    width = 0.3

    col = ['r', 'y', 'g']

    fig, ax = plt.subplots()

    for i, size in enumerate(sizes):
        mean, std = means[size], stds[size]

        stdlow = numpy.array(std)
        stdlow[stdlow >= mean] = mean[stdlow >= mean] * .999999

        x = numpy.arange(mean.shape[0]) + 0.05
        ax.bar(x + i * width, mean, width, color=col[i], yerr=(stdlow,std), log=True)

    means = numpy.array([mean for mean in means.values()])

    ax.set_xlabel('Run')
    ax.set_ylabel('Memory Usage (kB)')
    ax.set_title('Comparison of Memory Usage for Different Approaches')
    ax.set_xticks(x + (width / 2))
    ax.set_xticklabels([nodes.capitalize() for nodes in results], rotation='vertical')
    ax.set_xlim(0, mean.shape[0])

    ax.legend([size.capitalize() for size in sizes], loc='upper left')

    low = 10**math.floor(math.log10(numpy.amin(means)))
    high = 10**math.ceil(math.log10(numpy.amax(means)))

    ax.set_ylim(low, high)

    plt.subplots_adjust(bottom=0.25)

    # plt.show()
    plt.savefig('full_memory_compare.png', bbox_inches='tight')
    plt.clf()

if __name__ == '__main__':
    main()
