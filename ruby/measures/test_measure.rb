class RubyTestMeasure < Openstudio::Measure

  def name
    return "Ruby Test Measure"
  end

  def arguments(model)
    args = OpenStudio::OSArgumentVector.new

    #make an argument insulation R-value
    r_value = OpenStudio::OSArgument::makeDoubleArgument("r_value",true)
    r_value.setDisplayName("Percentage Increase of R-value for Exterior Wall Insulation.")
    args << r_value

    return args
  end #end the arguments method

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    r_value = runner.getDoubleArgumentValue("r_value",user_arguments)

    obj = Openstudio::ModelObject.new(m)

    obj.setName("ModelObject created in Ruby with r_value=#{r_value}");
    return true;
  end
end
