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

command = ('/usr/bin/time', '-f', '%e', 'bin/pciam',)

ImgPair = collections.namedtuple('ImgPair', ('name', 'i1', 'i2'))

result_parse = re.compile(r'^\(x,y,ncc\) = \((.*), (.*), (.*)\)$')

image_pairs = [
    ImgPair('small', 'img1_sm.tif', 'img2_sm.tif'),
    ImgPair('medium', 'img1_med.tif', 'img2_med.tif'),
    ImgPair('large', 'img1.tif', 'img2.tif')]

def main():
    DEVNULL = open(os.devnull, 'w')
    proc = None

    results = {}

    if os.path.exists('results_pciam.json'):
        with open('results_pciam.json', 'r') as file:
            results = json.load(file)
    try:
        for image_pair in image_pairs:
            for repeat in range(len(results.setdefault(image_pair.name, {'results':[], 'runtimes':[]})['runtimes']), 20):
                cmd = command + image_pair[-2:]
                print(cmd)
                proc = subprocess.Popen(cmd, stderr=subprocess.PIPE)
                err = proc.communicate()[1].decode('ascii')
                out, time = err.splitlines()
                out = result_parse.match(out)
                time = float(time)
                print(out.group(1), out.group(2), out.group(3))
                out = (int(out.group(1)), int(out.group(2)), float(out.group(3)))
                results[image_pair.name]['results'].append(out)
                results[image_pair.name]['runtimes'].append(time)
                with open('results_pciam.json', 'w') as file:
                    json.dump(results, file)
    finally:
        DEVNULL.close()
        if proc is not None and proc.poll is None:
            proc.terminate()

if __name__ == '__main__':
    main()
