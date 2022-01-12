class RubyTestMeasure < Openstudio::Measure

  def name
    return "RubyTestMeasure"
  end

  def run_impl(runner)
    m = runner.get_current_model()
    obj = Openstudio::ModelObject.new(m)
    obj.setName("ModelObject created in Ruby");

    return true;
  end
end
