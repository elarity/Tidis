#ifndef __CONFIG_H
#define __CONFIG_H

// linux
#if __linux__
  #define IS_EPOLL 1
#endif
// unix:openbsd freebsd
#if __unix__
#endif

#endif
