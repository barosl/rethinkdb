#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <string>

#include "containers/printf_buffer.hpp"
#include "time.hpp"

#ifndef NDEBUG
#define trace_call(fn, args...) do {                                          \
        debugf("%s:%u: %s: entered\n", __FILE__, __LINE__, stringify(fn));  \
        fn(args);                                                           \
        debugf("%s:%u: %s: returned\n", __FILE__, __LINE__, stringify(fn)); \
    } while (0)
#define TRACEPOINT debugf("%s:%u reached\n", __FILE__, __LINE__)
#else
#define trace_call(fn, args...) fn(args)
// TRACEPOINT is not defined in release, so that TRACEPOINTS do not linger in the code unnecessarily
#endif

// HEY: Maybe debugf and log_call and TRACEPOINT should be placed in
// debugf.hpp (and debugf.cc).
/* Debugging printing API (prints current thread in addition to message) */
void debug_print_quoted_string(printf_buffer_t *buf, const uint8_t *s, size_t n);
void debugf_prefix_buf(printf_buffer_t *buf);
void debugf_dump_buf(printf_buffer_t *buf);

// Primitive debug_print declarations.
void debug_print(printf_buffer_t *buf, uint64_t x);
void debug_print(printf_buffer_t *buf, const std::string& s);

template <class T>
void debug_print(printf_buffer_t *buf, T *ptr) {
    buf->appendf("%p", ptr);
}

#ifndef NDEBUG
void debugf(const char *msg, ...) __attribute__((format (printf, 1, 2)));
template <class T>
void debugf_print(const char *msg, const T &obj) {
    printf_buffer_t buf;
    debugf_prefix_buf(&buf);
    buf.appendf("%s: ", msg);
    debug_print(&buf, obj);
    buf.appendf("\n");
    debugf_dump_buf(&buf);
}
#else
#define debugf(...) ((void)0)
#define debugf_print(...) ((void)0)
#endif  // NDEBUG

template <class T>
std::string debug_strprint(const T &obj) {
    printf_buffer_t buf;
    debug_print(&buf, obj);
    return std::string(buf.data(), buf.size());
}

class debugf_in_dtor_t {
public:
    explicit debugf_in_dtor_t(const char *msg, ...) __attribute__((format (printf, 2, 3)));
    ~debugf_in_dtor_t();
private:
    std::string message;
};

class Term;
void pb_print(Term *t);

// TODO: make this more efficient (use `clock_monotonic` and use a vector of
// integers rather than accumulating a string).
class debug_timer_t {
public:
    explicit debug_timer_t(std::string _name = "");
    ~debug_timer_t();
    microtime_t tick(const std::string &tag);
private:
    microtime_t start, last;
    std::string name, out;
    DISABLE_COPYING(debug_timer_t);
};

#endif  // DEBUG_HPP_

