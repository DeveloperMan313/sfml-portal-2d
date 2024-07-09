#pragma once

#include <stdexcept>

namespace game {

template <class Ins, typename... T> class Singleton {
public:
  Singleton(const Singleton &) = delete;

  Singleton &operator=(const Singleton &) = delete;

  static Ins &get() {
    if (Singleton::ins == nullptr) {
      throw std::runtime_error("Singleton's instance does not exist");
    }
    return *Singleton::ins;
  }

  static void createInstance(T... args) {
    if (Singleton::ins != nullptr) {
      throw std::runtime_error("Singleton's instance already exists");
    }
    Singleton::ins = new Ins(args...);
  };

  static void deleteInstance() {
    delete Singleton::ins;
    Singleton::ins = nullptr;
  }

private:
  static inline Ins *ins = nullptr;

  Singleton() = default;

  ~Singleton() = default;
};

} // namespace game
