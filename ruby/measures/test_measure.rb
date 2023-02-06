class RubyTestMeasure < Openstudio::Measure

  def name
    return "Ruby Test Measure"
  end

  def run(runner)
    m = runner.get_current_model()
    obj = Openstudio::ModelObject.new(m)
    obj.setName("ModelObject created in Ruby");

    f = Openstudio::Foo.new()
    bars = f.bars()
    bars.each do |b|
      puts b.name()
    end

    return true;
  end
end
