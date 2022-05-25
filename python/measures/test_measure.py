import openstudio

class PythonTestMeasure(openstudio.Measure):
    def __init__(self):
        openstudio.Measure.__init__(self)

    def run(self, r: openstudio.Runner):
        m = r.get_current_model()
        obj = openstudio.ModelObject(m)
        obj.setName("ModelObject created in Python")

        return True

    def name(self):
        return "Python Test Measure"
