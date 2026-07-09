# KekeOS_Native

Welcome to **KekeOS_Native**, a custom 32-bit x86 bare-metal operating system written from scratch in C and Assembly. 

This repository contains the original, pure bare-metal version of the Keke-OS project. It includes a custom bootloader setup, a basic kernel, framebuffer graphics with custom bitmap fonts, and an IDE/ATA storage driver.

> **Project Status:** This native version is currently archived/on pause. The project has moved to a Linux-kernel-based architecture over at [Keke OS](https://github.com/kominkrystof-cpu/Keke-OS) to leverage robust hardware drivers for the Purr++ gaming engine.

---

## The Infamous `disktest` Issue
The system functions beautifully in QEMU, but testing on real hardware (like a ThinkPad X240) revealed the harsh reality of modern x86 architecture. 

The current **ATA/IDE PIO driver (`ata.c`)** fails on bare-metal because modern machines utilize **SATA/AHCI controllers** instead of legacy IDE ports. A preliminary attempt to write a native AHCI driver was made but remains non-functional, causing the `disktest` command to return floating bus garbage values (`0xf0000000...`).

### Community Challenge: Fix the Disk Driver!
If you are a low-level OS development enthusiast and want to tackle a real challenge, the AHCI/ATA storage stack is waiting for a fix. Pull Requests are highly welcome!

---

## Repository Structure

* `kernel.c` / `kernel.bin` — The core kernel logic and entry point.
* `boot.asm` — Assembly initialization and GDT/stack setup.
* `ata.c` / `ata.h` — The legacy IDE/ATA storage driver (The broken piece).
* `fb_text.c` / `font.c` — Custom framebuffer rendering and bitmap font drawing.
* `grub.cfg` / `limine.cfg` — Bootloader configurations supporting both GRUB and Limine.
* `run.sh` / `Makefile` — Build automation and QEMU emulation scripts.

---

## How to Run (Emulation)

To compile the kernel and boot it instantly inside the QEMU emulator, make sure you have `gcc-multilib`, `xorriso`, and `qemu-system-i386` installed, then run:

```bash
chmod +x run.sh
./run.sh
```

---

## License & Contribution Terms

This project is licensed under the **MIT License** with an important community request:

1. **Attribution:** You must keep the original author's name (**Kryštof Komín**) in all copies or substantial portions of the software.
2. **The Feedback Loop:** If you successfully fix bugs or improve the low-level code (especially the ATA/AHCI disk drivers to make `disktest` work), the author kindly requests that you **open a Pull Request** or notify them via GitHub issues. 

Let's keep the knowledge open, but let the author know you solved the puzzle!
