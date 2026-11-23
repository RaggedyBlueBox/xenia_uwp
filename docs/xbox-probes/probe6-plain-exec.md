# Xbox UWP probe 6: plain private executable memory

Date: 2026-11-23. Device: Xbox Series S devkit. Method: exe-swap
(`xeniacanary.exe` replaced by probe, app launched, `result.txt` read
back, original restored hash-verified).

## Question

The JIT code cache maps a file section at fixed `0xA0000000` and commits
executable pages inside the view. Those commits fail on Xbox, killing any
game once translated-code demand grows. Does plain private executable
memory (no section) work instead?

## Result

```
reserve_exec_rw: 0000020A80000000 err=0
commit16_exec_rw: 0000020A80000000 err=0
writeback_ok=1
protect_exec_to_rw: 1 err=0
fromapp_missing
reserve_rw: 0000020A80000000 err=0
commit16_rw: 0000020A80000000 err=0
```

- `VirtualAlloc` RESERVE + 16 MiB COMMIT with `PAGE_EXECUTE_READWRITE`:
  succeeds, writable, and `VirtualProtect` back to RW succeeds.
- Control case (plain RW) also succeeds.
- `VirtualAllocFromApp` could not be located by name in this probe
  (`fromapp_missing`); the build uses desktop `VirtualAlloc` on this
  partition anyway (see `XE_BASE_MEMORY_WIN_USE_DESKTOP_FUNCTIONS`,
  which includes `WINAPI_PARTITION_GAMES`).

## Consequence

The code cache on Xbox UWP now reserves plain private memory at the
fixed base instead of mapping a section view there
(`XE_PLATFORM_WINRT` path in `code_cache_base.h`). All base-relative
bookkeeping is unchanged.

Probe source: `probe6.c` (kept with the session artifacts, not committed).
