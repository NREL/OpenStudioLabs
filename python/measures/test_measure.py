# import sys
# sys.path.append("/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/Products/python/")

from time import time, ctime

import mylib

class PythonTestMeasure(mylib.PythonMeasure):
    def __init__(self):
        mylib.PythonMeasure.__init__(self)

    def run_impl(self, r: mylib.Runner):
        m = r.get_current_model()
        m.pushOp("Op from Python")
        m.setName("Python Model")
        m.pushObject("Python Space")
        m.getObject(1).setName("MODIFIED FROM PYTHON")

        return True

    def name(self):
        return "Python Test Measure"


def make_measure():
    return PythonTestMeasure()

