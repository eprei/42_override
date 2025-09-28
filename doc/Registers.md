| 64-bit Register | 32-bit Register | 16-bit Register | 8-bit Register | Volatile | Argument |
|-----------------|-----------------|-----------------|----------------|----------|----------|
| RAX             | EAX             | AX              | AL/AH          | Yes      | Return   |
| RBX             | EBX             | BX              | BL/BH          | No       | -        |
| RCX             | ECX             | CX              | CL/CH          | Yes      | 4th      |
| RDX             | EDX             | DX              | DL/DH          | Yes      | 3rd      |
| RSI             | ESI             | SI              | SIL            | Yes      | 2nd      |
| RDI             | EDI             | DI              | DIL            | Yes      | 1st      |
| RBP             | EBP             | BP              | BPL            | No       | -        |
| RSP             | ESP             | SP              | SPL            | No       | -        |
| R8              | R8D             | R8W             | R8B            | Yes      | 5th      |
| R9              | R9D             | R9W             | R9B            | Yes      | 6th      |
| R10             | R10D            | R10W            | R10B           | Yes      | -        |
| R11             | R11D            | R11W            | R11B           | Yes      | -        |
| R12             | R12D            | R12W            | R12B           | No       | -        |
| R13             | R13D            | R13W            | R13B           | No       | -        |
| R14             | R14D            | R14W            | R14B           | No       | -        |
| R15             | R15D            | R15W            | R15B           | No       | -        |

**Calling Conventions:**
- **32-bit (i386):** All arguments pushed onto stack in reverse order
- **64-bit (AMD64):** First 6 arguments in registers (RDI, RSI, RDX, RCX, R8, R9), rest on stack. RAX for return value.