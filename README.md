# Override

A collection of binary exploitation challenges from the 42 school's Override project. Each level presents a unique
vulnerability that must be exploited to progress to the next level.

## Project Overview

Override is a security-focused project at 42 that teaches binary exploitation techniques through progressively
challenging levels. Each level contains a vulnerable binary that must be exploited to retrieve a password for the next
level.

## Structure

```
override/
├── level00/      
├── level01/         
├── level02/        
...
├── level09/         
└── README.md
```

Each level directory contains:
- `source.c` - Decompiled/reconstructed source code
- `Ressources/walkthrough.md` - Detailed exploitation guide
- `flag` - Password for the next level

## Technologies & Techniques

- **Languages**: C, x86-64 Assembly
- **Tools**: GDB, pwndbg, objdump, readelf
- **Concepts**: Buffer overflows, format string attacks, stack manipulation, ROP chains

## Learning Objectives

- Understanding memory layout and stack frames
- Binary exploitation techniques
- Reverse engineering compiled binaries
- Security vulnerability analysis
