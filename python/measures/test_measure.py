import openstudio

class PythonTestMeasure(openstudio.Measure):

    def name(self):
        return "Python Test Measure"

    def arguments(self, model: openstudio.Model):
        args = openstudio.OSArgumentVector()
        r_value = openstudio.measure.OSArgument.makeDoubleArgument("r_value", True)
        r_value.setDisplayName("Percentage Increase of R-value for Roof Insulation.")
        args.append(r_value)
        return args

    def run(self,
            model: openstudio.Model,
            runner: openstudio.Runner,
            user_arguments: openstudio.OSArgumentMap):
        """Run Method"""
        super().run(model, runner, user_arguments)  # Do **NOT** remove this line

        r_value = runner.getDoubleArgumentValue("r_value", user_arguments)
        print(f"r_value={r_value}")

        obj = openstudio.ModelObject(model)
        obj.setName(f"ModelObject created in Python, r_value={r_value}")

        return True


