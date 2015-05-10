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
    with open('results_exhaustive.json') as file:
        results['exhaustive'] = json.load(file)
    with open('results_saserial.json') as file:
        results['serial'] = json.load(file)
    with open('results_saparallel.json') as file:
        res = json.load(file)
        for key in sorted(res, key=lambda x: int(x.split(maxsplit=1)[0])):
            results[key] = res[key]
    with open('results_pciam.json') as file:
        results['pciam'] = json.load(file)

    for sizes in results.values():
        for res in sizes.values():
            res['mean'] = numpy.mean(res['runtimes'])
            res['std'] = numpy.std(res['runtimes'])

    sizes = ['small', 'medium', 'large']

    width = 0.3

    col = ['r', 'y', 'g']

    means = []

    fig, ax = plt.subplots()

    for i, size in enumerate(sizes):
        mean = []
        std = []
        for nodes in results:
            mean.append(results[nodes][size]['mean'])
            std.append(results[nodes][size]['std'])

        means.append(mean)

        mean, std = numpy.array(mean), numpy.array(std)

        stdlow = numpy.array(std)
        stdlow[stdlow >= mean] = mean[stdlow >= mean] * .999999

        x = numpy.arange(mean.shape[0]) + 0.05
        ax.bar(x + i * width, mean, width, color=col[i], yerr=(stdlow,std), log=True)

    means = numpy.array(means)

    ax.set_xlabel('Run')
    ax.set_ylabel('Runtime (s)')
    ax.set_title('Comparison of Runtimes for Different Approaches')
    ax.set_xticks(x + (width / 2))
    ax.set_xticklabels([nodes.capitalize() for nodes in results], rotation='vertical')
    ax.set_xlim(0, mean.shape[0])

    ax.legend([size.capitalize() for size in sizes])

    low = 10**math.floor(math.log10(numpy.amin(means)))
    high = 10**math.ceil(math.log10(numpy.amax(means)))

    ax.set_ylim(low, high)

    plt.subplots_adjust(bottom=0.25)

    # plt.show()
    plt.savefig('full_time_compare.png', bbox_inches='tight')
    plt.clf()

if __name__ == '__main__':
    main()
