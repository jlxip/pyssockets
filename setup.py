#!/usr/bin/env python
from distutils.core import setup, Extension
import os

# Add (even though they don't compile) all files in src/
src = 'src'
src = [os.path.join(src, i) for i in os.listdir(src)]

pyssockets = Extension(
	'pyssockets',
	libraries = ['ssockets'],
	sources = ['pyssockets.c'] + src)

setup(
	name = 'pyssockets',
	version = '0.1.2',
	description = 'SSockets wrapper for python',
	author = 'jlxip',
	url = 'https://github.com/jlxip/pyssockets',
	ext_modules = [pyssockets])
