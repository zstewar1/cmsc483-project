#!/usr/bin/env python
from __future__ import print_function, division
import collections
import itertools
import os
import json
import random
import re
import subprocess
import sys

command = ('/usr/bin/time', '-f', '%e', 'mpirun', '-npernode', '6', '-np', 'bin/mcparallel',)

ImgPair = collections.namedtuple('ImgPair', ('name', 'i1', 'i2'))

result_parse = re.compile(r'^\(x,y,ncc,iters\) = \((.*), (.*), (.*), (.*)\)$')

"""
    image_pairs = [
    ImgPair('small', 'img1_sm.tif', 'img2_sm.tif'),
    ImgPair('medium', 'img1_med.tif', 'img2_med.tif'),
    ImgPair('large', 'img1.tif', 'img2.tif')]
    """

image_pairs = [ImgPair('large', 'img1.tif', 'img2.tif')]
number_iterations = 10
iteration_count_vals = [50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000]


def main():
    DEVNULL = open(os.devnull, 'w')
    proc = None
    
    results = {}
    
    if os.path.exists('results_mcparallel.json'):
        os.remove('results_mcparallel.json');
    #with open('results_saparallel.json', 'r') as file:
    #results = json.load(file)
    try:
        for nodes in range(1,16,2):
            print("starting node count: " + str(nodes))
            for nItr in iteration_count_vals:
                nodestr = str(nodes) + ' nodes' + ' nItr ' + str(nItr)
                print(nodestr)
                results.setdefault(nodestr, {})
                for image_pair in image_pairs:
                    results[nodestr].setdefault(image_pair.name, {'results':[], 'runtimes':[]})
                    for repeat in range(len(results[nodestr][image_pair.name]['runtimes']), number_iterations):
                        print('Repeat', repeat)
                        cmd = command[:-1] + (str(nodes),) + command[-1:] + image_pair[-2:] + (str(nItr), '2.99')
                        print(cmd)
                        proc = subprocess.Popen(cmd, stderr=subprocess.PIPE)
                        err = proc.communicate()[1].decode('ascii')
                        out, time = err.splitlines()
                        out = result_parse.match(out)
                        time = float(time)
                        print(out.group(1), out.group(2), out.group(3), out.group(4))
                        out = (int(out.group(1)), int(out.group(2)), float(out.group(3)), int(out.group(4)))
                        results[nodestr][image_pair.name]['results'].append(out)
                        results[nodestr][image_pair.name]['runtimes'].append(time)
                        with open('results_mcparallel.json', 'w') as file:
                            json.dump(results, file)
    finally:
        DEVNULL.close()
        if proc is not None and proc.poll is None:
            proc.terminate()

if __name__ == '__main__':
    main()
