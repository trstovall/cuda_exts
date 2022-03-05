import os
from distutils.core import setup, Extension
import numpy as np

os.environ["CC"] = "g++"
os.environ["CXX"] = "g++"
os.environ["CUDA_PATH"] = "/usr/local/cuda"

CUDA_PATH = os.environ['CUDA_PATH']

if not os.path.isdir(CUDA_PATH):
   print("CUDA_PATH {} not found. Please update the CUDA_PATH variable and rerun".format(CUDA_PATH))
   exit(0)

if not os.path.isdir(os.path.join(CUDA_PATH, "include")):
    print("include directory not found in CUDA_PATH. Please update CUDA_PATH and try again")
    exit(0)

setup(name = 'vector_add', version = '1.0',  \
   ext_modules = [
      Extension('vector_add', ['vector_add.c'], 
      # include_dirs=[np.get_include(), os.path.join(CUDA_PATH, "include")],
      libraries=[
         "vectoradd",
         "cudart"
      ],
      library_dirs = ["./libs", os.path.join(CUDA_PATH, "lib64")]
)])
