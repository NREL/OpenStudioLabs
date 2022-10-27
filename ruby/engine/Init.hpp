#ifndef openstudio_ruby_init_included
#define openstudio_ruby_init_included

#include "ruby.h"

extern "C"
{
  // OpenStudio specific bindings
  void Init_openstudio(void);
}

namespace openstudio {

namespace ruby {

void init() {
  ruby_setup();
  Init_openstudio();
}

}  // namespace ruby
}  // namespace openstudio

#endif  // openstudio_ruby_init_included
