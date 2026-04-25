#include <cstdio>
#include <array>
#include <climits>
#include <windows.h>
#include <winternl.h>
#include "exportLists.hpp"
#include <conio.h>

bool MulDivCheck() {
	return MulDiv(1, INT_MIN, INT_MIN) == -1;
}

template <typename T, size_t N>
bool DllExportsCheck(HMODULE hModule, const std::array<T, N> &exportList) {
    bool foundExport = false;
    
    for (const char *exportName : exportList) {
        if (GetProcAddress(hModule, exportName)) {
            foundExport = true;
            break;
        }
        else {
        }
    }

    return not foundExport;
}

using movaps_fn_t = void (*)(void *);

bool KernelMovapsRealignCheck() {
    SetErrorMode(SEM_NOALIGNMENTFAULTEXCEPT);

    BYTE movapsStub[] = {
        0x0F, 0x28, 0x01, // movaps xmm0, [rcx]
        0xC3              // ret
    };

    void *executableStub = VirtualAlloc(
        nullptr,
        sizeof(movapsStub),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    if (!executableStub) {
        return false;
    }

    memcpy(executableStub, movapsStub, sizeof(movapsStub));

    movaps_fn_t movaps_fn = (movaps_fn_t)executableStub;

    alignas(16) BYTE buffer[32]{};
    void *misaligned = buffer + 1;

    __try {
        movaps_fn(misaligned);
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
    
}

int main() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) {
        return 1;
    }

    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    if (!kernel32) {
        return 1;
    }

    printf("Muldiv Check: %d\n", MulDivCheck());
    printf("ntdll exports Check: %d\n", DllExportsCheck(ntdll, ntdllExports));
    printf("kernel32 exports check: %d\n", DllExportsCheck(kernel32, kernel32Exports));
    printf("movaps misalignment kernel patch check: %d\n", KernelMovapsRealignCheck());

	// values of 1 mean that the check passed, and 0 means it failed.

    (void)_getch();
}
