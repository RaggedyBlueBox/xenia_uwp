#include <windows.h>
#include <stdio.h>
#include <string.h>

// Probe plain (non-section) executable memory on Xbox UWP.
int main() {
  char out[2048];
  int n = 0;
  DWORD err;

  // 1. Plain reserve + commit EXECUTE_READWRITE via VirtualAlloc.
  void* p1 = VirtualAlloc(NULL, 256 * 1024 * 1024, MEM_RESERVE,
                          PAGE_EXECUTE_READWRITE);
  err = GetLastError();
  n += snprintf(out + n, sizeof(out) - n, "reserve_exec_rw: %p err=%lu\n",
                p1, p1 ? 0 : (unsigned long)err);
  if (p1) {
    void* c1 = VirtualAlloc(p1, 16 * 1024 * 1024, MEM_COMMIT,
                            PAGE_EXECUTE_READWRITE);
    err = GetLastError();
    n += snprintf(out + n, sizeof(out) - n, "commit16_exec_rw: %p err=%lu\n",
                  c1, c1 ? 0 : (unsigned long)err);
    if (c1) {
      memset(c1, 0xCC, 4096);
      n += snprintf(out + n, sizeof(out) - n, "writeback_ok=%d\n",
                    ((unsigned char*)c1)[0] == 0xCC);
    }
    DWORD old;
    BOOL prot = VirtualProtect(p1, 4096, PAGE_READWRITE, &old);
    err = GetLastError();
    n += snprintf(out + n, sizeof(out) - n,
                  "protect_exec_to_rw: %d err=%lu\n", (int)prot,
                  prot ? 0 : (unsigned long)err);
    VirtualFree(p1, 0, MEM_RELEASE);
  }

  // 2. Same via VirtualAllocFromApp (what the UWP build uses).
  HMODULE k32 = GetModuleHandleA("kernel32.dll");
  void* (WINAPI* FromApp)(void*, SIZE_T, ULONG, ULONG) =
      (void* (WINAPI*)(void*, SIZE_T, ULONG, ULONG))(
          void*)GetProcAddress(k32, "VirtualAllocFromApp");
  if (!FromApp) {
    n += snprintf(out + n, sizeof(out) - n, "fromapp_missing\n");
  } else {
    void* p2 = FromApp(NULL, 256 * 1024 * 1024, MEM_RESERVE,
                       PAGE_EXECUTE_READWRITE);
    err = GetLastError();
    n += snprintf(out + n, sizeof(out) - n, "fromapp_reserve_exec: %p err=%lu\n",
                  p2, p2 ? 0 : (unsigned long)err);
    if (p2) {
      void* c2 = FromApp(p2, 16 * 1024 * 1024, MEM_COMMIT,
                         PAGE_EXECUTE_READWRITE);
      err = GetLastError();
      n += snprintf(out + n, sizeof(out) - n, "fromapp_commit16_exec: %p err=%lu\n",
                    c2, c2 ? 0 : (unsigned long)err);
      // release via VirtualFree (documented pair)
      VirtualFree(p2, 0, MEM_RELEASE);
    }
  }

  // 3. Plain RW reserve+commit sanity (control case).
  void* p3 = VirtualAlloc(NULL, 64 * 1024 * 1024, MEM_RESERVE, PAGE_READWRITE);
  err = GetLastError();
  n += snprintf(out + n, sizeof(out) - n, "reserve_rw: %p err=%lu\n", p3,
                p3 ? 0 : (unsigned long)err);
  if (p3) {
    void* c3 = VirtualAlloc(p3, 16 * 1024 * 1024, MEM_COMMIT, PAGE_READWRITE);
    err = GetLastError();
    n += snprintf(out + n, sizeof(out) - n, "commit16_rw: %p err=%lu\n", c3,
                  c3 ? 0 : (unsigned long)err);
    VirtualFree(p3, 0, MEM_RELEASE);
  }

  FILE* f = fopen("result.txt", "w");
  if (f) {
    fwrite(out, 1, n, f);
    fclose(f);
  }
  return 0;
}
