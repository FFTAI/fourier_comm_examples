#include <array>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <new>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace rust {
inline namespace cxxbridge1 {
// #include "rust/cxx.h"

struct unsafe_bitcopy_t;

namespace {
template <typename T>
class impl;
} // namespace

#ifndef CXXBRIDGE1_RUST_STRING
#define CXXBRIDGE1_RUST_STRING
class String final {
public:
  String() noexcept;
  String(const String &) noexcept;
  String(String &&) noexcept;
  ~String() noexcept;

  String(const std::string &);
  String(const char *);
  String(const char *, std::size_t);
  String(const char16_t *);
  String(const char16_t *, std::size_t);

  static String lossy(const std::string &) noexcept;
  static String lossy(const char *) noexcept;
  static String lossy(const char *, std::size_t) noexcept;
  static String lossy(const char16_t *) noexcept;
  static String lossy(const char16_t *, std::size_t) noexcept;

  String &operator=(const String &) &noexcept;
  String &operator=(String &&) &noexcept;

  explicit operator std::string() const;

  const char *data() const noexcept;
  std::size_t size() const noexcept;
  std::size_t length() const noexcept;
  bool empty() const noexcept;

  const char *c_str() noexcept;

  std::size_t capacity() const noexcept;
  void reserve(size_t new_cap) noexcept;

  using iterator = char *;
  iterator begin() noexcept;
  iterator end() noexcept;

  using const_iterator = const char *;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  bool operator==(const String &) const noexcept;
  bool operator!=(const String &) const noexcept;
  bool operator<(const String &) const noexcept;
  bool operator<=(const String &) const noexcept;
  bool operator>(const String &) const noexcept;
  bool operator>=(const String &) const noexcept;

  void swap(String &) noexcept;

  String(unsafe_bitcopy_t, const String &) noexcept;

private:
  struct lossy_t;
  String(lossy_t, const char *, std::size_t) noexcept;
  String(lossy_t, const char16_t *, std::size_t) noexcept;
  friend void swap(String &lhs, String &rhs) noexcept { lhs.swap(rhs); }

  std::array<std::uintptr_t, 3> repr;
};
#endif // CXXBRIDGE1_RUST_STRING

#ifndef CXXBRIDGE1_RUST_BOX
#define CXXBRIDGE1_RUST_BOX
template <typename T>
class Box final {
public:
  using element_type = T;
  using const_pointer =
      typename std::add_pointer<typename std::add_const<T>::type>::type;
  using pointer = typename std::add_pointer<T>::type;

  Box() = delete;
  Box(Box &&) noexcept;
  ~Box() noexcept;

  explicit Box(const T &);
  explicit Box(T &&);

  Box &operator=(Box &&) &noexcept;

  const T *operator->() const noexcept;
  const T &operator*() const noexcept;
  T *operator->() noexcept;
  T &operator*() noexcept;

  template <typename... Fields>
  static Box in_place(Fields &&...);

  void swap(Box &) noexcept;

  static Box from_raw(T *) noexcept;

  T *into_raw() noexcept;

  /* Deprecated */ using value_type = element_type;

private:
  class uninit;
  class allocation;
  Box(uninit) noexcept;
  void drop() noexcept;

  friend void swap(Box &lhs, Box &rhs) noexcept { lhs.swap(rhs); }

  T *ptr;
};

template <typename T>
class Box<T>::uninit {};

template <typename T>
class Box<T>::allocation {
  static T *alloc() noexcept;
  static void dealloc(T *) noexcept;

public:
  allocation() noexcept : ptr(alloc()) {}
  ~allocation() noexcept {
    if (this->ptr) {
      dealloc(this->ptr);
    }
  }
  T *ptr;
};

template <typename T>
Box<T>::Box(Box &&other) noexcept : ptr(other.ptr) {
  other.ptr = nullptr;
}

template <typename T>
Box<T>::Box(const T &val) {
  allocation alloc;
  ::new (alloc.ptr) T(val);
  this->ptr = alloc.ptr;
  alloc.ptr = nullptr;
}

template <typename T>
Box<T>::Box(T &&val) {
  allocation alloc;
  ::new (alloc.ptr) T(std::move(val));
  this->ptr = alloc.ptr;
  alloc.ptr = nullptr;
}

template <typename T>
Box<T>::~Box() noexcept {
  if (this->ptr) {
    this->drop();
  }
}

template <typename T>
Box<T> &Box<T>::operator=(Box &&other) &noexcept {
  if (this->ptr) {
    this->drop();
  }
  this->ptr = other.ptr;
  other.ptr = nullptr;
  return *this;
}

template <typename T>
const T *Box<T>::operator->() const noexcept {
  return this->ptr;
}

template <typename T>
const T &Box<T>::operator*() const noexcept {
  return *this->ptr;
}

template <typename T>
T *Box<T>::operator->() noexcept {
  return this->ptr;
}

template <typename T>
T &Box<T>::operator*() noexcept {
  return *this->ptr;
}

template <typename T>
template <typename... Fields>
Box<T> Box<T>::in_place(Fields &&...fields) {
  allocation alloc;
  auto ptr = alloc.ptr;
  ::new (ptr) T{std::forward<Fields>(fields)...};
  alloc.ptr = nullptr;
  return from_raw(ptr);
}

template <typename T>
void Box<T>::swap(Box &rhs) noexcept {
  using std::swap;
  swap(this->ptr, rhs.ptr);
}

template <typename T>
Box<T> Box<T>::from_raw(T *raw) noexcept {
  Box box = uninit{};
  box.ptr = raw;
  return box;
}

template <typename T>
T *Box<T>::into_raw() noexcept {
  T *raw = this->ptr;
  this->ptr = nullptr;
  return raw;
}

template <typename T>
Box<T>::Box(uninit) noexcept {}
#endif // CXXBRIDGE1_RUST_BOX

#ifndef CXXBRIDGE1_RUST_ERROR
#define CXXBRIDGE1_RUST_ERROR
class Error final : public std::exception {
public:
  Error(const Error &);
  Error(Error &&) noexcept;
  ~Error() noexcept override;

  Error &operator=(const Error &) &;
  Error &operator=(Error &&) &noexcept;

  const char *what() const noexcept override;

private:
  Error() noexcept = default;
  friend impl<Error>;
  const char *msg;
  std::size_t len;
};
#endif // CXXBRIDGE1_RUST_ERROR

#ifndef CXXBRIDGE1_RUST_OPAQUE
#define CXXBRIDGE1_RUST_OPAQUE
class Opaque {
public:
  Opaque() = delete;
  Opaque(const Opaque &) = delete;
  ~Opaque() = delete;
};
#endif // CXXBRIDGE1_RUST_OPAQUE

#ifndef CXXBRIDGE1_IS_COMPLETE
#define CXXBRIDGE1_IS_COMPLETE
namespace detail {
namespace {
template <typename T, typename = std::size_t>
struct is_complete : std::false_type {};
template <typename T>
struct is_complete<T, decltype(sizeof(T))> : std::true_type {};
} // namespace
} // namespace detail
#endif // CXXBRIDGE1_IS_COMPLETE

#ifndef CXXBRIDGE1_LAYOUT
#define CXXBRIDGE1_LAYOUT
class layout {
  template <typename T>
  friend std::size_t size_of();
  template <typename T>
  friend std::size_t align_of();
  template <typename T>
  static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_size_of() {
    return T::layout::size();
  }
  template <typename T>
  static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_size_of() {
    return sizeof(T);
  }
  template <typename T>
  static
      typename std::enable_if<detail::is_complete<T>::value, std::size_t>::type
      size_of() {
    return do_size_of<T>();
  }
  template <typename T>
  static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_align_of() {
    return T::layout::align();
  }
  template <typename T>
  static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_align_of() {
    return alignof(T);
  }
  template <typename T>
  static
      typename std::enable_if<detail::is_complete<T>::value, std::size_t>::type
      align_of() {
    return do_align_of<T>();
  }
};

template <typename T>
std::size_t size_of() {
  return layout::size_of<T>();
}

template <typename T>
std::size_t align_of() {
  return layout::align_of<T>();
}
#endif // CXXBRIDGE1_LAYOUT

namespace repr {
struct PtrLen final {
  void *ptr;
  ::std::size_t len;
};
} // namespace repr

namespace detail {
template <typename T, typename = void *>
struct operator_new {
  void *operator()(::std::size_t sz) { return ::operator new(sz); }
};

template <typename T>
struct operator_new<T, decltype(T::operator new(sizeof(T)))> {
  void *operator()(::std::size_t sz) { return T::operator new(sz); }
};
} // namespace detail

template <typename T>
union MaybeUninit {
  T value;
  void *operator new(::std::size_t sz) { return detail::operator_new<T>{}(sz); }
  MaybeUninit() {}
  ~MaybeUninit() {}
};

namespace {
template <>
class impl<Error> final {
public:
  static Error error(repr::PtrLen repr) noexcept {
    Error error;
    error.msg = static_cast<char const *>(repr.ptr);
    error.len = repr.len;
    return error;
  }
};
} // namespace
} // namespace cxxbridge1
} // namespace rust

struct MotorManagerSync;

#ifndef CXXBRIDGE1_STRUCT_MotorManagerSync
#define CXXBRIDGE1_STRUCT_MotorManagerSync
struct MotorManagerSync final : public ::rust::Opaque {
  bool cxx_wait_for_first_messages(float timeout_sec) const noexcept;
  bool cxx_enable(::std::int32_t id) const noexcept;
  bool cxx_disable(::std::int32_t id) const noexcept;
  float cxx_get_position(::std::int32_t id) const;
  bool cxx_set_position(::std::int32_t id, float value) const noexcept;
  float cxx_get_velocity(::std::int32_t id) const;
  bool cxx_set_velocity(::std::int32_t id, float value) const noexcept;
  float cxx_get_current(::std::int32_t id) const;
  bool cxx_set_current(::std::int32_t id, float value) const noexcept;
  float cxx_get_effort(::std::int32_t id) const;
  bool cxx_set_effort(::std::int32_t id, float value) const noexcept;
  bool cxx_set_control_mode(::std::int32_t id, ::std::string const &value) const noexcept;
  ::rust::String cxx_get_control_mode(::std::int32_t id) const noexcept;
  ::rust::String cxx_get_motor_state(::std::int32_t id) const noexcept;
  bool cxx_set_motor_pid_gain(::std::int32_t id, float position_kp, float velocity_kp, float velocity_ki) const noexcept;
  bool cxx_set_control_pd_gain(::std::int32_t id, float kp, float kd) const noexcept;
  bool cxx_stop() const noexcept;
  ~MotorManagerSync() = delete;

private:
  friend ::rust::layout;
  struct layout {
    static ::std::size_t size() noexcept;
    static ::std::size_t align() noexcept;
  };
};
#endif // CXXBRIDGE1_STRUCT_MotorManagerSync

extern "C" {
::std::size_t cxxbridge1$MotorManagerSync$operator$sizeof() noexcept;
::std::size_t cxxbridge1$MotorManagerSync$operator$alignof() noexcept;

::MotorManagerSync *cxxbridge1$make_motor_manager_v1(::std::vector<::std::int32_t> const &ids) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_wait_for_first_messages(::MotorManagerSync const &self, float timeout_sec) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_enable(::MotorManagerSync const &self, ::std::int32_t id) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_disable(::MotorManagerSync const &self, ::std::int32_t id) noexcept;

::rust::repr::PtrLen cxxbridge1$MotorManagerSync$cxx_get_position(::MotorManagerSync const &self, ::std::int32_t id, float *return$) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_set_position(::MotorManagerSync const &self, ::std::int32_t id, float value) noexcept;

::rust::repr::PtrLen cxxbridge1$MotorManagerSync$cxx_get_velocity(::MotorManagerSync const &self, ::std::int32_t id, float *return$) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_set_velocity(::MotorManagerSync const &self, ::std::int32_t id, float value) noexcept;

::rust::repr::PtrLen cxxbridge1$MotorManagerSync$cxx_get_current(::MotorManagerSync const &self, ::std::int32_t id, float *return$) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_set_current(::MotorManagerSync const &self, ::std::int32_t id, float value) noexcept;

::rust::repr::PtrLen cxxbridge1$MotorManagerSync$cxx_get_effort(::MotorManagerSync const &self, ::std::int32_t id, float *return$) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_set_effort(::MotorManagerSync const &self, ::std::int32_t id, float value) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_set_control_mode(::MotorManagerSync const &self, ::std::int32_t id, ::std::string const &value) noexcept;

void cxxbridge1$MotorManagerSync$cxx_get_control_mode(::MotorManagerSync const &self, ::std::int32_t id, ::rust::String *return$) noexcept;

void cxxbridge1$MotorManagerSync$cxx_get_motor_state(::MotorManagerSync const &self, ::std::int32_t id, ::rust::String *return$) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_set_motor_pid_gain(::MotorManagerSync const &self, ::std::int32_t id, float position_kp, float velocity_kp, float velocity_ki) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_set_control_pd_gain(::MotorManagerSync const &self, ::std::int32_t id, float kp, float kd) noexcept;

bool cxxbridge1$MotorManagerSync$cxx_stop(::MotorManagerSync const &self) noexcept;
} // extern "C"

::std::size_t MotorManagerSync::layout::size() noexcept {
  return cxxbridge1$MotorManagerSync$operator$sizeof();
}

::std::size_t MotorManagerSync::layout::align() noexcept {
  return cxxbridge1$MotorManagerSync$operator$alignof();
}

::rust::Box<::MotorManagerSync> make_motor_manager_v1(::std::vector<::std::int32_t> const &ids) noexcept {
  return ::rust::Box<::MotorManagerSync>::from_raw(cxxbridge1$make_motor_manager_v1(ids));
}

bool MotorManagerSync::cxx_wait_for_first_messages(float timeout_sec) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_wait_for_first_messages(*this, timeout_sec);
}

bool MotorManagerSync::cxx_enable(::std::int32_t id) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_enable(*this, id);
}

bool MotorManagerSync::cxx_disable(::std::int32_t id) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_disable(*this, id);
}

float MotorManagerSync::cxx_get_position(::std::int32_t id) const {
  ::rust::MaybeUninit<float> return$;
  ::rust::repr::PtrLen error$ = cxxbridge1$MotorManagerSync$cxx_get_position(*this, id, &return$.value);
  if (error$.ptr) {
    throw ::rust::impl<::rust::Error>::error(error$);
  }
  return ::std::move(return$.value);
}

bool MotorManagerSync::cxx_set_position(::std::int32_t id, float value) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_set_position(*this, id, value);
}

float MotorManagerSync::cxx_get_velocity(::std::int32_t id) const {
  ::rust::MaybeUninit<float> return$;
  ::rust::repr::PtrLen error$ = cxxbridge1$MotorManagerSync$cxx_get_velocity(*this, id, &return$.value);
  if (error$.ptr) {
    throw ::rust::impl<::rust::Error>::error(error$);
  }
  return ::std::move(return$.value);
}

bool MotorManagerSync::cxx_set_velocity(::std::int32_t id, float value) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_set_velocity(*this, id, value);
}

float MotorManagerSync::cxx_get_current(::std::int32_t id) const {
  ::rust::MaybeUninit<float> return$;
  ::rust::repr::PtrLen error$ = cxxbridge1$MotorManagerSync$cxx_get_current(*this, id, &return$.value);
  if (error$.ptr) {
    throw ::rust::impl<::rust::Error>::error(error$);
  }
  return ::std::move(return$.value);
}

bool MotorManagerSync::cxx_set_current(::std::int32_t id, float value) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_set_current(*this, id, value);
}

float MotorManagerSync::cxx_get_effort(::std::int32_t id) const {
  ::rust::MaybeUninit<float> return$;
  ::rust::repr::PtrLen error$ = cxxbridge1$MotorManagerSync$cxx_get_effort(*this, id, &return$.value);
  if (error$.ptr) {
    throw ::rust::impl<::rust::Error>::error(error$);
  }
  return ::std::move(return$.value);
}

bool MotorManagerSync::cxx_set_effort(::std::int32_t id, float value) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_set_effort(*this, id, value);
}

bool MotorManagerSync::cxx_set_control_mode(::std::int32_t id, ::std::string const &value) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_set_control_mode(*this, id, value);
}

::rust::String MotorManagerSync::cxx_get_control_mode(::std::int32_t id) const noexcept {
  ::rust::MaybeUninit<::rust::String> return$;
  cxxbridge1$MotorManagerSync$cxx_get_control_mode(*this, id, &return$.value);
  return ::std::move(return$.value);
}

::rust::String MotorManagerSync::cxx_get_motor_state(::std::int32_t id) const noexcept {
  ::rust::MaybeUninit<::rust::String> return$;
  cxxbridge1$MotorManagerSync$cxx_get_motor_state(*this, id, &return$.value);
  return ::std::move(return$.value);
}

bool MotorManagerSync::cxx_set_motor_pid_gain(::std::int32_t id, float position_kp, float velocity_kp, float velocity_ki) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_set_motor_pid_gain(*this, id, position_kp, velocity_kp, velocity_ki);
}

bool MotorManagerSync::cxx_set_control_pd_gain(::std::int32_t id, float kp, float kd) const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_set_control_pd_gain(*this, id, kp, kd);
}

bool MotorManagerSync::cxx_stop() const noexcept {
  return cxxbridge1$MotorManagerSync$cxx_stop(*this);
}

extern "C" {
::MotorManagerSync *cxxbridge1$box$MotorManagerSync$alloc() noexcept;
void cxxbridge1$box$MotorManagerSync$dealloc(::MotorManagerSync *) noexcept;
void cxxbridge1$box$MotorManagerSync$drop(::rust::Box<::MotorManagerSync> *ptr) noexcept;
} // extern "C"

namespace rust {
inline namespace cxxbridge1 {
template <>
::MotorManagerSync *Box<::MotorManagerSync>::allocation::alloc() noexcept {
  return cxxbridge1$box$MotorManagerSync$alloc();
}
template <>
void Box<::MotorManagerSync>::allocation::dealloc(::MotorManagerSync *ptr) noexcept {
  cxxbridge1$box$MotorManagerSync$dealloc(ptr);
}
template <>
void Box<::MotorManagerSync>::drop() noexcept {
  cxxbridge1$box$MotorManagerSync$drop(this);
}
} // namespace cxxbridge1
} // namespace rust
