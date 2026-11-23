# How this fork was born

I bought a devkit because I wanted to play my own games on my own terms.
Minecraft Xbox 360 Edition with my kids. Skate 3 the way I remembered it.
No streaming, no storefront telling me what's still allowed to run — just
the console I own, running the games I own, through an emulator I can read.

The base for this is kuaan08's Xenia UWP fork, which did the unglamorous
work nobody else bothered with: getting Xenia Canary to actually install
and boot as a UWP package on a real Xbox. Without that, none of this
exists. Credit where it's due — I stood on that work.

## The wall

Minecraft loaded a world and died. Every time. The log said:

```
BaseHeap::AllocFixed failed to alloc range from host
Access Violation: write at 0x000000010000000000
```

It turned out the Xbox OS silently refuses guest memory commits unless the
commit protection exactly matches the section protection — something you
will never find in any documentation, because nobody documents what an
Xbox devkit refuses to do. I only found it by running tiny native probes
directly on the Series S: hundreds of small programs that asked the OS one
question at a time until the pattern showed itself.

That investigation is the soul of this fork. Everything else — the game
list cleanup, the headless boot for testing, the art fixes — came from
actually using it on the couch and fixing whatever annoyed me next.

## What it is today

- Boots and runs on a Series S devkit as a UWP package.
- Gets Minecraft past the crash that killed every world load.
- A library view that shows your games instead of every system file.
- NFS Most Wanted and Skate 3 alongside it, running from real discs dumps.

It is experimental. Some of this may never upstream. I broke things more
than once getting here, and the commit history shows it.

## Where it could go

Honestly? Wherever my shelf points next. Fallout: New Vegas is already
waiting on disk. The dream is simple: every 360 game I own, booting from
the box under my TV, with the kinks fixed one title at a time — and every
fix written up so the next person with a devkit doesn't start from zero.

If upstream wants any of this, it's theirs. That's the deal with building
on open work.
