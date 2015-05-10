#!/usr/bin/env python
from __future__ import print_function, division
import json
import os
import re

result_parse = re.compile(r'^\(x,y,ncc\) = \((.*), (.*), (.*)\)$')

def main():
    sizes = {'small': '_sm', 'medium': '_med', 'large': ''}

    results = {}

    for size, suffix in sizes.items():
        for repeat in range(1, 21):
            with open(os.path.join('exhaustive_results', 'r{0}{1}'.format(repeat, suffix))) as file:
                res = results.setdefault(size, {'results': [], 'runtimes': []})
                out, time = file.read().splitlines()
                time = float(time)
                out = result_parse.match(out)
                out = (int(out.group(1)), int(out.group(2)), float(out.group(3)))
                res['results'].append(out)
                res['runtimes'].append(time)
    with open('results_exhaustive.json', 'w') as file:
        json.dump(results, file)

if __name__ == '__main__':
    main()
