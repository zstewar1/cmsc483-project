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

command = ('/usr/bin/time', '-f', '%e', 'mpirun', '-npernode', '6', 'bin/saparallel_vary_iter',)

ImgPair = collections.namedtuple('ImgPair', ('name', 'i1', 'i2'))

image_pairs = [
#    ImgPair('small', 'img1_sm.tif', 'img2_sm.tif'),
#    ImgPair('medium', 'img1_med.tif', 'img2_med.tif'),
    ImgPair('large', 'img1.tif', 'img2.tif')]

def main():
    results = {}

    of = 'results_saparallel_vary_iter.json'

    if os.path.exists(of):
        with open(of, 'r') as file:
            results = json.load(file)
    try:
        for iter_reduce in range(0, 101, 5):
            if not iter_reduce:
                iter_reduce = 1
            reduce_str = str(iter_reduce)
            results.setdefault(reduce_str, {})
            for image_pair in image_pairs:
                results[reduce_str].setdefault(image_pair.name, [])
                for repeat in range(len(results[reduce_str][image_pair.name]), 20):
                    print('Repeat', repeat)
                    cmd = command + image_pair[-2:] + ('0.99', reduce_str)
                    print(cmd)
                    proc = subprocess.Popen(cmd, stderr=subprocess.PIPE)
                    err = proc.communicate()[1].decode('ascii')
                    time = err.splitlines()[1]
                    time = float(time)
                    results[reduce_str][image_pair.name].append(time)
                    with open(of, 'w') as file:
                        json.dump(results, file)
    finally:
        if proc is not None and proc.poll is None:
            proc.terminate()

if __name__ == '__main__':
    main()
