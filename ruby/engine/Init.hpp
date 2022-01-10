#ifndef openstudio_ruby_init_included
#define openstudio_ruby_init_included

#include "ruby.h"

extern "C"
{
  void Init_encdb();
  void Init_encoding();
  void Init_encodings();

  //void Init_ascii(); // this is not included in libenc
  void Init_big5();
  void Init_cesu_8();
  void Init_cp949();
  void Init_emacs_mule();
  void Init_euc_jp();
  void Init_euc_kr();
  void Init_euc_tw();
  void Init_gb18030();
  void Init_gb2312();
  void Init_gbk();
  void Init_iso_8859_1();
  void Init_iso_8859_10();
  void Init_iso_8859_11();
  void Init_iso_8859_13();
  void Init_iso_8859_14();
  void Init_iso_8859_15();
  void Init_iso_8859_16();
  void Init_iso_8859_2();
  void Init_iso_8859_3();
  void Init_iso_8859_4();
  void Init_iso_8859_5();
  void Init_iso_8859_6();
  void Init_iso_8859_7();
  void Init_iso_8859_8();
  void Init_iso_8859_9();
  void Init_koi8_r();
  void Init_koi8_u();
  void Init_shift_jis();
  //void Init_unicode(); // this is not included in libenc
  //void Init_us_ascii(); // this is not included in libenc
  void Init_utf_16be();
  void Init_utf_16le();
  void Init_utf_32be();
  void Init_utf_32le();
  //void Init_utf_8(); // this is not included in libenc
  void Init_windows_1250();
  void Init_windows_1251();
  void Init_windows_1252();
  void Init_windows_1253();
  void Init_windows_1254();
  void Init_windows_1257();
  void Init_windows_31j();

  void Init_transdb();

  void Init_trans_big5();
  void Init_trans_cesu_8();
  void Init_trans_chinese();
  void Init_trans_ebcdic();
  void Init_trans_emoji();
  void Init_trans_emoji_iso2022_kddi();
  void Init_trans_emoji_sjis_docomo();
  void Init_trans_emoji_sjis_kddi();
  void Init_trans_emoji_sjis_softbank();
  void Init_trans_escape();
  void Init_trans_gb18030();
  void Init_trans_gbk();
  void Init_trans_iso2022();
  void Init_trans_japanese();
  void Init_trans_japanese_euc();
  void Init_trans_japanese_sjis();
  void Init_trans_korean();
  void Init_trans_single_byte();
  void Init_trans_utf8_mac();
  void Init_trans_utf_16_32();

  void Init_bigdecimal();
  void Init_bigdecimal(void);
  void Init_continuation(void);
  void Init_coverage(void);
  void Init_cparse(void);
  void Init_date_core(void);
  void Init_digest(void);
  void Init_escape(void);
  void Init_etc(void);
  void Init_fcntl(void);
  void Init_fiber(void);
  void Init_fiddle(void);
  void Init_generator(void);
  void Init_md5(void);
  void Init_monitor(void);
  void Init_nkf(void);
  void Init_nonblock(void);
  void Init_objspace(void);
  void Init_parser(void);
  void Init_pathname(void);
  void Init_psych(void);
  void Init_ripper(void);
  void Init_rmd160(void);
  void Init_sdbm(void);
  void Init_sha1(void);
  void Init_sha2(void);
  void Init_sizeof(void);
  void Init_socket(void);
  void Init_stringio(void);
  void Init_strscan(void);
  void Init_wait(void);
  void Init_zlib(void);

  void Init_openssl(void);
  void Init_ruby_description(void);

#ifndef _WIN32
  void Init_console(void);
  void Init_dbm(void);
  void Init_gdbm(void);
  void Init_pty(void);
  void Init_readline(void);
  void Init_syslog(void);
#endif

  // OpenStudio specific bindings
  void Init_mylib(void);
}

namespace openstudio {

namespace ruby {

void init() {
  RUBY_INIT_STACK;
  ruby_init();
  
  //swig::GC_VALUE::hash_id = rb_intern("hash");
  //swig::GC_VALUE::lt_id = rb_intern("<");
  //swig::GC_VALUE::gt_id = rb_intern(">");
  //swig::GC_VALUE::eq_id = rb_intern("==");
  //swig::GC_VALUE::le_id = rb_intern("<=");
  //swig::GC_VALUE::ge_id = rb_intern(">=");
  //
  //swig::GC_VALUE::pos_id = rb_intern("+@");
  //swig::GC_VALUE::neg_id = rb_intern("-@");
  //swig::GC_VALUE::inv_id = rb_intern("~");
  //
  //swig::GC_VALUE::add_id = rb_intern("+");
  //swig::GC_VALUE::sub_id = rb_intern("-");
  //swig::GC_VALUE::mul_id = rb_intern("*");
  //swig::GC_VALUE::div_id = rb_intern("/");
  //swig::GC_VALUE::mod_id = rb_intern("%");
  //
  //swig::GC_VALUE::and_id = rb_intern("&");
  //swig::GC_VALUE::or_id = rb_intern("|");
  //swig::GC_VALUE::xor_id = rb_intern("^");
  //
  //swig::GC_VALUE::lshift_id = rb_intern("<<");
  //swig::GC_VALUE::rshift_id = rb_intern(">>");
  
  //// in case any further init methods try to require files, init this first
  //Init_EmbeddedScripting();
  //
  //// Need embedded_help for requiring files out of the embedded system
  //auto embedded_extensions_string = embedded_files::getFileAsString(":/embedded_help.rb");
  //
  //try {
  //  rubyInterpreter.evalString(embedded_extensions_string);
  //} catch (const std::exception& e) {
  //  rubyInterpreter.evalString(R"(STDOUT.flush)");
  //  std::cout << "Exception in embedded_help: " << e.what() << std::endl;  // endl will flush
  //  return ruby_cleanup(1);
  //} catch (...) {
  //  rubyInterpreter.evalString(R"(STDOUT.flush)");
  //  std::cout << "Unknown Exception in embedded_help" << std::endl;  // endl will flush
  //  return ruby_cleanup(1);
  //}
  
  //// encodings
  /// Get the symbols from: `DUMPBIN /ARCHIVEMEMBERS  C:\Users\julien\.conan\data\openstudio_ruby\2.7.2\nrel\testing\package\2fa339f0e9f8e459bd56b19a37be69734f2745f4\lib\enc\libenc.lib`
  Init_encdb();
  rb_provide("enc/encdb.so");
  Init_encoding();
  //Init_ascii();
  //rb_provide("enc/ascii.so");
  Init_big5();
  rb_provide("enc/big5.so");
  Init_cesu_8();
  rb_provide("enc/cesu_8.so");
  Init_cp949();
  rb_provide("enc/cp949.so");
  Init_emacs_mule();
  rb_provide("enc/emacs_mule.so");
  Init_euc_jp();
  rb_provide("enc/euc_jp.so");
  Init_euc_kr();
  rb_provide("enc/euc_kr.so");
  Init_euc_tw();
  rb_provide("enc/euc_tw.so");
  Init_gb18030();
  rb_provide("enc/gb18030.so");
  Init_gb2312();
  rb_provide("enc/gb2312.so");
  Init_gbk();
  rb_provide("enc/gbk.so");
  Init_iso_8859_1();
  rb_provide("enc/iso_8859_1.so");
  Init_iso_8859_10();
  rb_provide("enc/iso_8859_10.so");
  Init_iso_8859_11();
  rb_provide("enc/iso_8859_11.so");
  Init_iso_8859_13();
  rb_provide("enc/iso_8859_13.so");
  Init_iso_8859_14();
  rb_provide("enc/iso_8859_14.so");
  Init_iso_8859_15();
  rb_provide("enc/iso_8859_15.so");
  Init_iso_8859_16();
  rb_provide("enc/iso_8859_16.so");
  Init_iso_8859_2();
  rb_provide("enc/iso_8859_2.so");
  Init_iso_8859_3();
  rb_provide("enc/iso_8859_3.so");
  Init_iso_8859_4();
  rb_provide("enc/iso_8859_4.so");
  Init_iso_8859_5();
  rb_provide("enc/iso_8859_5.so");
  Init_iso_8859_6();
  rb_provide("enc/iso_8859_6.so");
  Init_iso_8859_7();
  rb_provide("enc/iso_8859_7.so");
  Init_iso_8859_8();
  rb_provide("enc/iso_8859_8.so");
  Init_iso_8859_9();
  rb_provide("enc/iso_8859_9.so");
  Init_koi8_r();
  rb_provide("enc/koi8_r.so");
  Init_koi8_u();
  rb_provide("enc/koi8_u.so");
  Init_shift_jis();
  rb_provide("enc/shift_jis.so");
  //Init_unicode();
  //rb_provide("enc/unicode.so");
  //Init_us_ascii();
  //rb_provide("enc/us_ascii.so");
  Init_utf_16be();
  rb_provide("enc/utf_16be.so");
  Init_utf_16le();
  rb_provide("enc/utf_16le.so");
  Init_utf_32be();
  rb_provide("enc/utf_32be.so");
  Init_utf_32le();
  rb_provide("enc/utf_32le.so");
  //Init_utf_8();
  //rb_provide("enc/utf_8.so");
  Init_windows_1250();
  rb_provide("enc/windows_1250.so");
  Init_windows_1251();
  rb_provide("enc/windows_1251.so");
  Init_windows_1252();
  rb_provide("enc/windows_1252.so");
  Init_windows_1253();
  rb_provide("enc/windows_1253.so");
  Init_windows_1254();
  rb_provide("enc/windows_1254.so");
  Init_windows_1257();
  rb_provide("enc/windows_1257.so");
  Init_windows_31j();
  rb_provide("enc/windows_31j.so");
  
  /// Get the symbols from: `DUMPBIN /ARCHIVEMEMBERS  C:\Users\julien\.conan\data\openstudio_ruby\2.7.2\nrel\testing\package\2fa339f0e9f8e459bd56b19a37be69734f2745f4\lib\enc\libtrans.lib`
  Init_transdb();
  rb_provide("enc/trans/transdb.so");
  
  //Init_trans_big5();
  //rb_provide("enc/trans/big5.so");
  Init_trans_big5();
  rb_provide("enc/trans/big5.so");
  
  Init_trans_cesu_8();
  rb_provide("enc/trans/cesu_8.so");
  
  Init_trans_chinese();
  rb_provide("enc/trans/chinese.so");
  
  Init_trans_ebcdic();
  rb_provide("enc/trans/ebcdic.so");
  
  Init_trans_emoji();
  rb_provide("enc/trans/emoji.so");
  
  Init_trans_emoji_iso2022_kddi();
  rb_provide("enc/trans/emoji_iso2022_kddi.so");
  
  Init_trans_emoji_sjis_docomo();
  rb_provide("enc/trans/emoji_sjis_docomo.so");
  
  Init_trans_emoji_sjis_kddi();
  rb_provide("enc/trans/emoji_sjis_kddi.so");
  
  Init_trans_emoji_sjis_softbank();
  rb_provide("enc/trans/emoji_sjis_softbank.so");
  
  Init_trans_escape();
  rb_provide("enc/trans/escape.so");
  
  Init_trans_gb18030();
  rb_provide("enc/trans/gb18030.so");
  
  Init_trans_gbk();
  rb_provide("enc/trans/gbk.so");
  
  Init_trans_iso2022();
  rb_provide("enc/trans/iso2022.so");
  
  Init_trans_japanese();
  rb_provide("enc/trans/japanese.so");
  
  Init_trans_japanese_euc();
  rb_provide("enc/trans/japanese_euc.so");
  
  Init_trans_japanese_sjis();
  rb_provide("enc/trans/japanese_sjis.so");
  
  Init_trans_korean();
  rb_provide("enc/trans/korean.so");
  
  Init_trans_single_byte();
  rb_provide("enc/trans/single_byte.so");
  
  Init_trans_utf8_mac();
  rb_provide("enc/trans/utf8_mac.so");
  
  Init_trans_utf_16_32();
  rb_provide("enc/trans/utf_16_32.so");
  
  Init_bigdecimal();
  rb_provide("bigdecimal");
  rb_provide("bigdecimal.so");
  
  Init_continuation();
  rb_provide("continuation");
  rb_provide("continuation.so");
  
  Init_coverage();
  rb_provide("coverage");
  rb_provide("coverage.so");
  
  Init_cparse();
  rb_provide("cparse");
  rb_provide("racc/cparse");
  rb_provide("cparse.so");
  rb_provide("racc/cparse.so");
  
  Init_date_core();
  rb_provide("date_core");
  rb_provide("date_core.so");
  
  Init_digest();
  rb_provide("digest");
  rb_provide("digest.so");
  
  Init_escape();
  rb_provide("escape");
  rb_provide("escape.so");
  
  Init_etc();
  rb_provide("etc");
  rb_provide("etc.so");
  
  Init_fcntl();
  rb_provide("fcntl");
  rb_provide("fcntl.so");
  
  Init_fiber();
  rb_provide("fiber");
  rb_provide("fiber.so");
  
  Init_fiddle();
  rb_provide("fiddle");
  rb_provide("fiddle.so");
  
  Init_generator();
  rb_provide("json/ext/generator");
  rb_provide("json/ext/generator.so");
  
  Init_md5();
  rb_provide("md5");
  rb_provide("digest/md5");
  rb_provide("md5.so");
  rb_provide("digest/md5.so");
  
  Init_monitor();
  rb_provide("monitor");
  rb_provide("monitor.so");
  
  Init_nkf();
  rb_provide("nkf");
  rb_provide("nkf.so");
  
  Init_nonblock();
  rb_provide("nonblock");
  rb_provide("nonblock.so");
  rb_provide("io/nonblock");
  rb_provide("io/nonblock.so");
  
  Init_ruby_description();
  
  Init_objspace();
  rb_provide("objspace");
  rb_provide("objspace.so");
  
  Init_parser();
  rb_provide("json/ext/parser");
  rb_provide("json/ext/parser.so");
  
  Init_pathname();
  rb_provide("pathname");
  rb_provide("pathname.so");
  
  Init_psych();
  rb_provide("psych");
  rb_provide("psych.so");
  
  Init_ripper();
  rb_provide("ripper");
  rb_provide("ripper.so");
  
  Init_rmd160();
  rb_provide("rmd160");
  rb_provide("digest/rmd160");
  rb_provide("rmd160.so");
  rb_provide("digest/rmd160.so");
  
  Init_sdbm();
  rb_provide("sdbm");
  rb_provide("sdbm.so");
  
  Init_sha1();
  rb_provide("sha1");
  rb_provide("digest/sha1");
  rb_provide("sha1.so");
  rb_provide("digest/sha1.so");
  
  Init_sha2();
  rb_provide("sha2");
  rb_provide("digest/sha2");
  rb_provide("sha2.so");
  rb_provide("digest/sha2.so");
  
  Init_sizeof();
  rb_provide("sizeof");
  rb_provide("sizeof.so");
  
  Init_socket();
  rb_provide("socket");
  rb_provide("socket.so");
  
  Init_stringio();
  rb_provide("stringio");
  rb_provide("stringio.so");
  
  Init_strscan();
  rb_provide("strscan");
  rb_provide("strscan.so");
  
  Init_wait();
  rb_provide("wait");
  rb_provide("io/wait");
  rb_provide("wait.so");
  rb_provide("io/wait.so");
  
  Init_zlib();
  rb_provide("zlib");
  rb_provide("zlib.so");
  
  Init_openssl();
  rb_provide("openssl");
  rb_provide("openssl.so");
  
  Init_nonblock();
  rb_provide("io/nonblock");
  rb_provide("io/nonblock.so");

#ifndef _WIN32

  // DLM: we have Init_console on Windows but crashes when try to init it, fails to load openssl
  Init_console();
  rb_provide("console");
  rb_provide("console.so");
  
  Init_dbm();
  rb_provide("dbm");
  rb_provide("dbm.so");
  
  Init_gdbm();
  rb_provide("gdbm");
  rb_provide("gdbm.so");
  
  Init_pty();
  rb_provide("pty");
  rb_provide("pty.so");
  
  Init_readline();
  rb_provide("readline");
  rb_provide("readline.so");
  
  Init_syslog();
  rb_provide("syslog");
  rb_provide("syslog.so");
#endif

  // OpenStudio specific bindings
  Init_encodings();
  Init_mylib();
}

} // ruby
} // openstudio

#endif // openstudio_ruby_init_included
