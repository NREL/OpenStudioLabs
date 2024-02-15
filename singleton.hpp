#ifndef DLLDEMO_SINGLETON_HPP
#define DLLDEMO_SINGLETON_HPP

template <typename T> class Singleton {
public:
  Singleton() = delete;
  using object_type = T;

  // If, at any point (in user code), Singleton<T>::instance()
  //  is called, then the following function is instantiated.
  static object_type &instance() {
    static object_type obj;
    return obj;
  }
};

#endif // DLLDEMO_SINGLETON_HPP
