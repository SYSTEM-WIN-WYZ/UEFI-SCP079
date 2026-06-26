/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efi.h

Abstract:

    Public EFI header files



Revision History

--*/


// Add a predefined macro to detect usage of the library
#ifndef _GNU_EFI
#define _GNU_EFI
#endif

//
// Build flags on input
//  EFI32
//  EFI_DEBUG               - Enable debugging code
//  EFI_NT_EMULATOR         - Building for running under NT
//


#ifndef _EFI_INCLUDE_
#define _EFI_INCLUDE_

#ifdef __cplusplus
extern "C" {
#endif

#define EFI_FIRMWARE_VENDOR         L"INTEL"
#define EFI_FIRMWARE_MAJOR_REVISION 12
#define EFI_FIRMWARE_MINOR_REVISION 33
#define EFI_FIRMWARE_REVISION ((EFI_FIRMWARE_MAJOR_REVISION <<16) | (EFI_FIRMWARE_MINOR_REVISION))

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#include "x86_64/efibind.h"
#elif defined(_M_IX86) || defined(__i386__)
#include "ia32/efibind.h"
#elif defined(_M_IA64) || defined(__ia64__)
#include "ia64/efibind.h"
#elif defined (_M_ARM64) || defined(__aarch64__)
#include "aarch64/efibind.h"
#elif defined (_M_ARM) || defined(__arm__)
#include "arm/efibind.h"
#elif defined (_M_MIPS64) || defined(__mips64__) || defined(__mips64)
#include "mips64el/efibind.h"
#elif defined (__riscv) && __riscv_xlen == 64
#include "riscv64/efibind.h"
#elif defined (__loongarch64)
#include "loongarch64/efibind.h"
#else
#error Usupported architecture
#endif

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    unsigned long long (*Reset)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned char ExtendedVerification);
    unsigned long long (*OutputString)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned short *String);
    unsigned long long (*TestString)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned short *String);
    unsigned long long (*QueryMode)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned long long ModeNumber,
        unsigned long long *Columns,
        unsigned long long *Rows);
    unsigned long long (*SetMode)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned long long ModeNumber);
    unsigned long long (*SetAttribute)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned long long Attribute);
    unsigned long long (*ClearScreen)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
    unsigned long long (*SetCursorPosition)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned long long Column,
        unsigned long long Row);
    unsigned long long (*EnableCursor)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned char Enable);
    struct SIMPLE_TEXT_OUTPUT_MODE *Mode;
};

#include "eficompiler.h"
#include "efidef.h"
#include "efidevp.h"
#include "efipciio.h"
#include "efiprot.h"
#include "eficon.h"
#include "eficonex.h"
#include "efiser.h"
#include "efi_nii.h"
#include "efipxebc.h"
#include "efinet.h"
#include "efiapi.h"
#include "efifs.h"
#include "efierr.h"
#include "efiui.h"
#include "efiip.h"
#include "efiudp.h"
#include "efitcp.h"
#include "efipoint.h"
#include "efishell.h"

#ifdef __cplusplus
}
#endif

#endif
