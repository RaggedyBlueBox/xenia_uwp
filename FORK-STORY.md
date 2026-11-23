# How this fork was born

Full disclosure first: I'm 19, I'm not much of a programmer, and this
whole thing is vibe-coded. I tell an AI what I want, it writes the code,
I test it on real hardware, and we argue until it works. Everything in
this repo was built that way — on a Series S devkit, tested on my TV,
one crash at a time.

I wanted my Xbox 360 games running on my own console, on my own terms.
Minecraft Xbox 360 Edition. No streaming, no storefront telling
me what's still allowed to run — just the box I own, playing the games
I own, through an emulator I can read.

## What this stands on

None of this is mine from scratch, and I won't pretend otherwise:

- **Xenia**, by Ben Vanik and hundreds of contributors — the actual 360
  emulator, years of absurdly hard work I could never do myself.
- **Xenia Canary**, the community fork where new fixes land first.
- **kuaan08's Xenia UWP fork**, which did the unglamorous work nobody
  else bothered with: getting Canary to install and boot as a UWP
  package on a real Xbox. Without that, nothing here exists.
- The community infrastructure around it: the game-patches and
  optimized-settings repos, and the x360db metadata the library view
  pulls art from.

My part is the last mile on real hardware: finding what breaks on a
devkit, proving it with on-console probes, and fixing it.

## The wall

Minecraft loaded a world and died. Every time. The log said:

```
BaseHeap::AllocFixed failed to alloc range from host
Access Violation: write at 0x000000010000000000
```

It turned out the Xbox OS silently refuses guest memory commits unless
the commit protection exactly matches the section protection — something
you will never find in any documentation, because nobody documents what
an Xbox devkit refuses to do. I only found it by running tiny native
probes directly on the Series S: hundreds of small programs that asked
the OS one question at a time until the pattern showed itself.

That investigation is the soul of this fork. Everything else — the
clean game list, the headless boot for testing, the art fixes — came
from actually using it on the couch and fixing whatever annoyed me next.

## What it is today

- Boots and runs on a Series S devkit as a UWP package.
- Gets Minecraft past the crash that killed every world load.
- A library view that shows your games instead of every system file.

It is experimental. I broke things more than once getting here, and the
commit history shows it.

## Where it could go

Honestly? Wherever my shelf points next. The dream is simple: every 360 game I own, booting from
the box under my TV, with the kinks fixed one title at a time — and
every fix written up so the next person with a devkit doesn't start
from zero.

If upstream wants any of this, it's theirs. That's the deal with
building on open work.
