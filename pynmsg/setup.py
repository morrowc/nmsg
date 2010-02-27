from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(
    name = '_nmsg',
    ext_modules = [
        Extension('_nmsg', ['nmsg.pyx'],
            libraries = ['nmsg'],
            library_dirs = ['../nmsg/.libs'],
            include_dirs = ['../nmsg', '../protobuf-c', '..']
        )
    ],
    cmdclass = {'build_ext': build_ext},
)

setup(
    name = 'nmsg',
    py_modules = ['nmsg'],
)
