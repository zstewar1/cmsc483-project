#!/usr/bin/env python
from __future__ import print_function, division
import collections
import itertools
import os
import random
import subprocess
import sys

#          0                1     2     3            4         5
command = ['/usr/bin/time', '-f', '%e', 'bin/pciam', '<IMG1>', '<IMG2>']

ImgPair = collections.namedtuple('ImgPair', ('name', 'i1', 'i2'))

image_pairs = [
    ImgPair('small', 'img1_sm.tif', 'img2_sm.tif'),
    ImgPair('medium', 'img1_med.tif', 'img2_med.tif'),
    ImgPair('large', 'img1.tif', 'img2.tif')]

def main():
    DEVNULL = open(os.devnull, 'w')
    proc = None
    try:

    except KeyboardInterrupt:
        if proc is not None and proc.poll is None:
            proc.terminate()
    finally:
        DEVNULL.close()

if __name__ == '__main__':
    main()
