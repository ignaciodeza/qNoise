from setuptools import setup, Extension
import pybind11
import sys

ext_modules = [
    Extension(
        "qnoise._qnoise",
        sources=["qnoise/qNoisePy.cpp", "../cpp/qNoise.cpp"],
        include_dirs=[pybind11.get_include(), "../cpp"],
        language="c++",
        extra_compile_args=["-std=c++11", "-O3", "-Wall", "-fPIC"] if sys.platform != "win32" else ["/std:c++14", "/O2"],
    ),
]

setup(ext_modules=ext_modules, zip_safe=False)