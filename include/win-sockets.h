
#ifndef _WINSOCKETS_H_
#define _WINSOCKETS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <basetsd.h>

#include <libsmb2.h>

#ifndef O_SYNC
#ifndef O_DSYNC
#define O_DSYNC		040000
#endif // !O_DSYNC
#define __O_SYNC	020000000
#define O_SYNC		(__O_SYNC|O_DSYNC)
#endif // !O_SYNC

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define getpid GetCurrentProcessId
#endif

#define EBADFD WSAENOTSOCK

typedef SSIZE_T ssize_t;

struct iovec {
  size_t iov_len;
  void * iov_base;
};

#ifndef _vscprintf
/* For some reason, MSVC fails to honour this #ifndef. */
/* Hence function renamed to _vscprintf_so(). */
static inline int _vscprintf_so(const char * format, va_list pargs) {
  int retval;
  va_list argcopy;
  va_copy(argcopy, pargs);
  retval = vsnprintf(NULL, 0, format, argcopy);
  va_end(argcopy);
  return retval;
}
#endif // _vscprintf

#ifndef vasprintf
static inline int vasprintf(char **strp, const char *fmt, va_list ap) {
  int len = _vscprintf_so(fmt, ap);
  if (len == -1) return -1;
  char *str = malloc((size_t)len + 1);
  if (!str) return -1;
  int r = vsnprintf(str, len + 1, fmt, ap); /* "secure" version of vsprintf */
  if (r == -1) return free(str), -1;
  *strp = str;
  return r;
}
#endif // vasprintf

#ifndef asprintf
static inline int asprintf(char *strp[], const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int r = vasprintf(strp, fmt, ap);
  va_end(ap);
  return r;
}
#endif // asprintf

static inline int writev(socket_type sock, struct iovec *iov, int nvecs)
{
  DWORD ret;
  LPWSABUF wsa = (LPWSABUF)malloc(sizeof(WSABUF) * nvecs);
  struct iovec *ptr = iov;
  for (size_t i = 0; i < nvecs; i++, ptr++) {
    wsa[i].len = (unsigned long)ptr->iov_len;
    wsa[i].buf = (char *)ptr->iov_base;
  }

  int res = WSASend(sock, wsa, nvecs, &ret, 0, NULL, NULL);
  free(wsa);

  if (res == 0) {
    return ret;
  }
  return -1;
}

static inline char* getlogin() {
  return "Guest";
}

static inline int readv(socket_type sock, struct iovec *iov, int nvecs)
{
  DWORD ret;
  DWORD flags = 0;

  LPWSABUF wsa = (LPWSABUF)malloc(sizeof(WSABUF) * nvecs);
  struct iovec *ptr = iov;
  for (size_t i = 0; i < nvecs; i++, ptr++) {
    wsa[i].len = (unsigned long)ptr->iov_len;
    wsa[i].buf = (char *)ptr->iov_base;
  }

  int res = WSARecv(sock, wsa, nvecs, &ret, &flags, NULL, NULL);
  free(wsa);

  if (res == 0) {
    return ret;
  }
  return -1;
}

static inline int close(socket_type sock)
{
  return closesocket(sock);
}

static inline int poll(struct pollfd pfd[], uint32_t size, int nvecs)
{
  return WSAPoll(pfd, size, nvecs);
}

#ifdef __cplusplus
}
#endif
#endif /* !_WINSOCKETS_H_ */