#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2016, Swanson Project.
# Please see the COPYING file in this distribution for license details.

import ast
import re
from setuptools import setup, find_packages


_version_re = re.compile(r'__version__\s+=\s+(.*)')


with open('swanson/__init__.py', 'rb') as f:
    version = str(ast.literal_eval(_version_re.search(
        f.read().decode('utf-8')).group(1)))


setup(
    name='swanson',
    version=version,
    description='Python host for Swanson programming language',
    author='Swanson Project',
    url='http://www.swanson-lang.org/',
    packages=find_packages(exclude=['tests']),
    install_requires=[],
    license='Apache',
)
