// Copyright (c) 2014, StrongLoop Inc.
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef COMPAT_H_  // NOLINT(build/header_guard)
#define COMPAT_H_  // NOLINT(build/header_guard)

#include "node_version.h"
#include "v8.h"
#include "v8-profiler.h"

#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION >= 11
#define COMPAT_NODE_VERSION 12  // v0.12
#elif NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION == 10
#define COMPAT_NODE_VERSION 10  // v0.10
#else
#error "Unsupported node.js version."
#endif

namespace compat {

#if COMPAT_NODE_VERSION == 10
typedef v8::Arguments ArgumentType;
typedef v8::Handle<v8::Value> ReturnType;
typedef v8::InvocationCallback FunctionCallback;
#elif COMPAT_NODE_VERSION == 12
typedef v8::FunctionCallbackInfo<v8::Value> ArgumentType;
typedef void ReturnType;
typedef v8::FunctionCallback FunctionCallback;
#endif

inline v8::Local<v8::Boolean> True(v8::Isolate* isolate);
inline v8::Local<v8::Boolean> False(v8::Isolate* isolate);
inline v8::Local<v8::Primitive> Null(v8::Isolate* isolate);
inline v8::Local<v8::Primitive> Undefined(v8::Isolate* isolate);

class AllStatic {
 private:
  AllStatic();
};

struct Array : public AllStatic {
  inline static v8::Local<v8::Array> New(v8::Isolate* isolate, int length = 0);
};

struct Boolean : public AllStatic {
  inline static v8::Local<v8::Boolean> New(v8::Isolate* isolate, bool value);
};

struct FunctionTemplate : public AllStatic {
  inline static v8::Local<v8::FunctionTemplate> New(v8::Isolate* isolate,
                                                    FunctionCallback callback =
                                                        0);
};

struct HeapProfiler : public AllStatic {
  inline static const v8::HeapSnapshot* TakeHeapSnapshot(
      v8::Isolate* isolate,
      v8::Local<v8::String> title = v8::Local<v8::String>());
  inline static void DeleteAllHeapSnapshots(v8::Isolate* isolate);
};

struct Integer : public AllStatic {
  inline static v8::Local<v8::Integer> New(v8::Isolate* isolate, int32_t value);
  inline static v8::Local<v8::Integer> NewFromUnsigned(v8::Isolate* isolate,
                                                       uint32_t value);
};

struct Number : public AllStatic {
  inline static v8::Local<v8::Number> New(v8::Isolate* isolate, double value);
};

struct Object : public AllStatic {
  inline static v8::Local<v8::Object> New(v8::Isolate* isolate);
};

struct String : public AllStatic {
  enum NewStringType {
    kNormalString,
    kInternalizedString,
    kUndetectableString
  };
  inline static v8::Local<v8::String> NewFromUtf8(v8::Isolate* isolate,
                                                  const char* data,
                                                  NewStringType type =
                                                      kNormalString,
                                                  int length = -1);
};

class HandleScope {
 public:
  inline explicit HandleScope(v8::Isolate* isolate);

 private:
  v8::HandleScope handle_scope_;
};

template <typename T>
class Persistent {
 public:
  inline ~Persistent();
  inline v8::Local<T> ToLocal(v8::Isolate* isolate) const;
  inline void Reset();
  inline void Reset(v8::Isolate* isolate, v8::Local<T> value);
  inline bool IsEmpty() const;

 private:
  v8::Persistent<T> handle_;
};

class ReturnableHandleScope {
 public:
  inline explicit ReturnableHandleScope(const ArgumentType& args);
  inline ReturnType Return();
  inline ReturnType Return(bool value);
  inline ReturnType Return(intptr_t value);
  inline ReturnType Return(int value);
  inline ReturnType Return(double value);
  inline ReturnType Return(const char* value);
  inline ReturnType Return(v8::Local<v8::Value> value);

 private:
  inline v8::Isolate* isolate() const;
  const ArgumentType& args_;
  v8::HandleScope handle_scope_;
};

}  // namespace compat

#endif  // COMPAT_H_  // NOLINT(build/header_guard)
