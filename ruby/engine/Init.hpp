#ifndef openstudio_ruby_init_included
#define openstudio_ruby_init_included

#include "ruby.h"

extern "C"
{
  // OpenStudio specific bindings
  void Init_mylib(void);
}

namespace openstudio {

namespace ruby {

void init() {
  // OpenStudio specific bindings
  ruby_setup();
  //Init_encodings();
  Init_mylib();
}

} // ruby
} // openstudio

#endif // openstudio_ruby_init_included
