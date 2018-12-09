#pragma once

// C compatible API
#ifdef __cplusplus
extern "C" {
#endif
void cu_start_tracing();
void cu_stop_tracing();
void cu_start_event_sym(void* sym_addr);
void cu_end_event_sym(void* sym_addr);
void cu_start_event();
void cu_end_event();
#ifdef __cplusplus
}
#endif

// C++ only API
#ifdef __cplusplus

#if defined(__linux__)
#define CTOR_RETURN_DEPTH 0
#elif defined(__APPLE__)
#define CTOR_RETURN_DEPTH 1
#else
#warning "Unsupported platform"
#endif

class cu_scoped_event
{
public:
  cu_scoped_event(void* sym_addr = NULL)
    : m_sym_addr((sym_addr != NULL) ? sym_addr : __builtin_return_address(CTOR_RETURN_DEPTH))
  {
    cu_start_event_sym(m_sym_addr);
  }

  ~cu_scoped_event()
  {
    cu_end_event_sym(m_sym_addr);
  }

private:
  void* m_sym_addr;
};
#endif
