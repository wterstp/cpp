// header_template.h
#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class NonCopyable {
public:
  NonCopyable();
  ~NonCopyable();
  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;
};

#endif /* NONCOPYABLE_H */
