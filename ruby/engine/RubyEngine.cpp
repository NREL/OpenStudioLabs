#include "RubyEngine.hpp"
#include "Init.hpp"
#include "ruby.h"
#include <signal.h>
#include <time.h>
#include <string>
#include <stdexcept>
#include <iostream>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wregister"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include "SWIGRubyRuntime.hxx"

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

// TODO: We need to revisit this static initialization stuff
static int argc = 0;
static char** argv = nullptr;

extern "C"
{
  // static void Init_builtin_prelude(void);
  // void Init_builtin_gem_prelude(void);
  struct ruby_cmdline_options;
  typedef struct ruby_cmdline_options ruby_cmdline_options_t;
  void Init_ruby_description(ruby_cmdline_options_t*);

  void ruby_gc_set_params(void);
  // cf https://bugs.ruby-lang.org/issues/19758
  // I've included the ext/extinit.o and enc/encinit.o in the object files to that should work
  void Init_enc(void);
  void Init_ext(void);
  void rb_call_builtin_inits(void);
}

unsigned init() {
  ruby_sysinit(&argc, &argv);
  ruby_setup();
  return 0u;
}

static unsigned i = init();

namespace openstudio {

RubyEngine::RubyEngine(int argc, char* argv[]) : ScriptEngine(argc, argv) {
  ruby_set_argv(argc, argv);

  // Init_ruby_description(nullptr);

  // ruby_gc_set_params();
  // TODO! build with enable-relative!
  ruby_init_loadpath();
  //rb_eval_string(R"ruby(
  //$LOAD_PATH=[
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/site_ruby/3.2.0",
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/site_ruby/3.2.0/x86_64-linux",
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/site_ruby",
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/vendor_ruby/3.2.0",
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/vendor_ruby/3.2.0/x86_64-linux",
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/vendor_ruby",
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/3.2.0",
  //"/home/julien/Software/Others/OpenStudioLabs/build-debug/Products/ruby_lib/ruby/3.2.0/x86_64-linux"
  //]

  //)ruby");

  // Init_enc();
  // rb_enc_set_default_internal(rb_enc_from_encoding(rb_utf8_encoding()));
  // rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));

  // Init_ext(); /* load statically linked extensions before rubygems */
  // Init_extra_exts();
  // rb_call_builtin_inits();

  openstudio::ruby::init();
}

RubyEngine::~RubyEngine() {
  ruby_finalize();
}

static VALUE evaluateSimpleImpl(VALUE arg) {
  return rb_eval_string(StringValuePtr(arg));
}

VALUE evalString(const std::string& t_str) {
  VALUE val = rb_str_new2(t_str.c_str());
  int error;
  // save and restore the current working directory in case the call to ruby upsets it
  VALUE result = rb_protect(evaluateSimpleImpl, val, &error);
  if (error != 0) {
    VALUE errval = rb_eval_string("$!.to_s");
    char* str = StringValuePtr(errval);
    std::string err(str);
    VALUE locval = rb_eval_string("$@.to_s");
    str = StringValuePtr(locval);
    std::string loc(str);
    throw std::runtime_error("Error '" + err + "' at " + loc);
  }

  return result;
}

ScriptObject RubyEngine::eval(std::string_view sv) {
  std::string str{sv};
  return ScriptObject{evalString(str)};
}

void RubyEngine::exec(std::string_view sv) {
  std::string str{sv};
  [[maybe_unused]] const auto result = evalString(str);
}

// convert the underlying object to the correct type, then return it as a void *
// so the above template function can provide it back to the caller.
void* RubyEngine::getAs_impl(ScriptObject& obj, const std::type_info& ti) {
  auto val = std::any_cast<VALUE>(obj.object);

  const auto& type_name = getRegisteredTypeName(ti);

  void* return_value = nullptr;

  auto* type = SWIG_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_ConvertPtr(val, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error("Error getting object from SWIG/Ruby");
  }

  return return_value;
}

}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::RubyEngine(argc, argv);
  }
}
