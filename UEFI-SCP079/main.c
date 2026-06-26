#include "efi.h"
#include "common.h"
EFI_SYSTEM_TABLE *ST;
EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

//struct EFI_SYSTEM_TABLE *ST;
//struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
EFI_SIMPLE_POINTER_PROTOCOL *SPP;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *STIEP;
EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *DPFTP;
EFI_DEVICE_PATH_UTILITIES_PROTOCOL *DPUP;
//struct EFI_GUID lip_guid = {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
EFI_GUID lip_guid = {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
//struct EFI_GUID dpp_guid = {0x09576e91, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
EFI_GUID dpp_guid = {0x09576e91, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

void efi_init(EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
    //struct EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
    EFI_GUID spp_guid = {0x31878c87, 0xb75, 0x11d5, {0x9a, 0x4f, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};
    EFI_GUID sfsp_guid = {0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
    EFI_GUID stiep_guid = {0xdd9e7534, 0x7762, 0x4698, {0x8c, 0x14, 0xf5, 0x85, 0x17, 0xa6, 0x25, 0xaa}};
    EFI_GUID dpttp_guid = {0x8b843e20, 0x8132, 0x4852, {0x90, 0xcc, 0x55, 0x1a, 0x4e, 0x4a, 0x7f, 0x1c}};
    EFI_GUID dpftp_guid = {0x5c99a21, 0xc70f, 0x4ad2, {0x8a, 0x5f, 0x35, 0xdf, 0x33, 0x43, 0xf5, 0x1e}};
    EFI_GUID dpup_guid = {0x379be4e, 0xd706, 0x437d, {0xb0, 0x37, 0xed, 0xb8, 0x2f, 0xb7, 0x72, 0xa4}};

    ST = SystemTable;
    ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
    ST->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&GOP);
    ST->BootServices->LocateProtocol(&spp_guid, NULL, (void **)&SPP);
    ST->BootServices->LocateProtocol(&sfsp_guid, NULL, (void **)&SFSP);
    ST->BootServices->LocateProtocol(&stiep_guid, NULL, (void **)&STIEP);
    ST->BootServices->LocateProtocol(&dpttp_guid, NULL, (void **)&DPTTP);
    ST->BootServices->LocateProtocol(&dpftp_guid, NULL, (void **)&DPFTP);
    ST->BootServices->LocateProtocol(&dpup_guid, NULL, (void **)&DPUP);
}

#include "efi.h"
#include "common.h"

#define MAX_STR_BUF 100

void putc(unsigned short c)
{
    unsigned short str[2] = L" ";
    str[0] = c;
    ST->ConOut->OutputString(ST->ConOut, str);
}

void puts(unsigned short *s)
{
    ST->ConOut->OutputString(ST->ConOut, s);
}

void puth(unsigned long long val, unsigned char num_digits)
{
    int i;
    unsigned short unicode_val;
    unsigned short str[MAX_STR_BUF];

    for (i = num_digits - 1; i >= 0; i--) {
        unicode_val = (unsigned short)(val & 0x0f);
        if (unicode_val < 0xa)
            str[i] = L'0' + unicode_val;
        else
            str[i] = L'A' + (unicode_val - 0xa);
        val >>= 4;
    }
    str[num_digits] = L'\0';

    puts(str);
}

unsigned short getc(void)
{
    EFI_INPUT_KEY key;
    unsigned long long waitidx;

    ST->BootServices->WaitForEvent(1, &(ST->ConIn->WaitForKey), &waitidx);
    while (ST->ConIn->ReadKeyStroke(ST->ConIn, &key))
        ;

    return (key.UnicodeChar) ? key.UnicodeChar : (key.ScanCode + SC_OFS);
}

unsigned int gets(unsigned short *buf, unsigned int buf_size)
{
    unsigned int i;

    for (i = 0; i < buf_size - 1;) {
        buf[i] = getc();
        putc(buf[i]);
        if (buf[i] == L'\b') {
            i--;
            continue;
        } else if (buf[i] == L'\r') {
            putc(L'\n');
            break;
        }
        i++;
    }
    buf[i] = L'\0';

    return i;
}

int strcmp(const unsigned short *s1, const unsigned short *s2)
{
    char is_equal = 1;

    for (; (*s1 != L'\0') && (*s2 != L'\0'); s1++, s2++) {
        if (*s1 != *s2) {
            is_equal = 0;
            break;
        }
    }

    if (is_equal) {
        if (*s1 != L'\0') {
            return 1;
        } else if (*s2 != L'\0') {
            return -1;
        } else {
            return 0;
        }
    } else {
        return (int)(*s1 - *s2);
    }
}

void strncpy(unsigned short *dst, unsigned short *src, unsigned long long n)
{
    while (n--)
        *dst++ = *src++;
}

unsigned long long strlen(unsigned short *str)
{
    unsigned long long len = 0;

    while (*str++ != L'\0')
        len++;

    return len;
}

unsigned char check_warn_error(unsigned long long status, unsigned short *message)
{
    if (status) {
        puts(message);
        puts(L":");
        puth(status, 16);
        puts(L"\r\n");
    }

    return !status;
}

void assert(unsigned long long status, unsigned short *message)
{
    if (!check_warn_error(status, message))
        while (1)
            ;
}
#define typejiange 500000
//void efi_main(void *ImageHandle __attribute__((unused)), struct EFI_SYSTEM_TABLE *SystemTable)
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    // 先设置 ST 以便使用输出
    //ST = SystemTable;
    
    // 尝试输出一条消息（使用 ConOut）
    //ST->ConOut->OutputString(ST->ConOut, L"Hello from efi_main!\r\n");
    //__asm__ volatile ("b .");
    
    
    unsigned long long status;
    unsigned long long status2;
    void *tevent,*tevent2;
    void *wait_list[1];
    void *wait_list2[1];
    unsigned long long idx;
    unsigned long long timewait=5000000;
    int i = 0;

    efi_init(SystemTable);
    //ST->ConOut->ClearScreen(ST->ConOut);

    status = ST->BootServices->CreateEvent(EVT_TIMER, 0, NULL, NULL, &tevent);
    assert(status, L"CreateEvent");
    status2 = ST->BootServices->CreateEvent(EVT_TIMER, 0, NULL, NULL, &tevent2);
    assert(status2, L"CreateEvent");

    wait_list[0] = tevent;
    wait_list2[0] = tevent2;

    while (TRUE) {
        status = ST->BootServices->SetTimer(tevent, TimerRelative, timewait);
        assert(status, L"SetTimer");

        status = ST->BootServices->WaitForEvent(1, wait_list, &idx);
        assert(status, L"WaitForEvent");

        switch (i) {
        case 0:
            puts(L"System startup\n\n\r");
            break;
        case 1:
            puts(L"EXITY BIOS\n\rVERSION 1.0\r\nCOPYRIGHT (C) 1978 BY EXITY TECHNOLOGY.\n\n\n\r");
            break;
        case 2:
            puts(L"System self-check\n\n\rBEGIN MEMORY BOARD, Memory Address    ");
            break;
        case 3:
            puts(L"[ ");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN|EFI_BACKGROUND_BLACK);
            puts(L"OK");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            puts(L" ]\n\n\r    ");
            break;
        case 4:
            puts(L"THE TOP OF RAM IS 7FFF HEX.\n\r    STACK BEGINS FROM 7F90 HEX.\n\n\n\r");
            break;
        case 5:
            puts(L"BEGIN CPU/SYSTEM BOARDS, Line Exchange    ");
            break;
        case 6:
            puts(L"[ ");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN|EFI_BACKGROUND_BLACK);
            puts(L"OK");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            puts(L" ]\n\n\r    ");
            break;
        case 7:
            puts(L"Started Initialize ExtIOStream\n\r");
            break;
        case 8:
            timewait=2500000;
            puts(L"    External Storage Device Detected @ 9F HEX.\n\r");
            break;
        case 9:
            puts(L"    Mounting /boot..\n\r");
            break;
        case 10:
            puts(L"    Mounted /boot    [ ");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN|EFI_BACKGROUND_BLACK);
            puts(L"OK");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            puts(L" ]\n\r");
            break;
        case 11:
            puts(L"    Stared Apply Kernel Variables\n\r");
            break;
        case 12:
            puts(L"    Running 'init.s'....\n\r");
            break;
        case 13:
        {
            //ST->ConOut->ClearScreen(ST->ConOut);
            puts(L"> ");
            int j = 0;
            unsigned short *ss = L"start psu_pms_res.s";
            while (TRUE) {
                status = ST->BootServices->SetTimer(tevent2, TimerRelative, typejiange);
                assert(status, L"SetTimer");
                status = ST->BootServices->WaitForEvent(1, wait_list2, &idx);
                assert(status, L"WaitForEvent");
                putc(ss[j]);
                j++;
                if (j == 20)
                {
                    puts(L"\n\r");
                    break;
                }
            }
            break;
        }
        case 14:
            puts(L"No var detected. Supply=DC Freq=n/a Stable=yes.\n\r");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN|EFI_BACKGROUND_BLACK);
            puts(L"Ready to receive.\n\r");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            break;
        case 15:
        {
            puts(L">");
            int j = 0;
            unsigned short *ss = L"port 21";
            while (TRUE) {
                status = ST->BootServices->SetTimer(tevent2, TimerRelative, typejiange);
                assert(status, L"SetTimer");
                status = ST->BootServices->WaitForEvent(1, wait_list2, &idx);
                assert(status, L"WaitForEvent");
                putc(ss[j]);
                j++;
                if (j == 8)
                {
                    puts(L"\n\r");
                    break;
                }
            }
            puts(L"Port 21 is currently closed.\n\r");
            break;
        }
        case 16:
        {
            puts(L">");
            int j = 0;
            unsigned short *ss = L"port open 21";
            while (TRUE) {
                status = ST->BootServices->SetTimer(tevent2, TimerRelative, typejiange);
                assert(status, L"SetTimer");
                status = ST->BootServices->WaitForEvent(1, wait_list2, &idx);
                assert(status, L"WaitForEvent");
                putc(ss[j]);
                j++;
                if (j == 13)
                {
                    puts(L"\n\r");
                    break;
                }
            }
            puts(L"Port 21 has been opened.\n\r");
            break;
        }
        case 17:
        {
            puts(L">");
            int j = 0;
            unsigned short *ss = L"Ping 275.6.92.157";
            while (TRUE) {
                status = ST->BootServices->SetTimer(tevent2, TimerRelative, typejiange);
                assert(status, L"SetTimer");
                status = ST->BootServices->WaitForEvent(1, wait_list2, &idx);
                assert(status, L"WaitForEvent");
                putc(ss[j]);
                j++;
                if (j == 18)
                {
                    puts(L"\n\r");
                    break;
                }
            }
            puts(L"Pinging 275.6.92.157 with 64 bits of data.\n\rReply from 275.6.92.157: Bits=64 Time=12ms\n\rSent=1 Received=1 Lost=0 AvgTime=12ms\n\r");
            puts(L"FTP connection request at PORT 21 from 275.6.92.157. Accept y/n ?\n\r>");
            status = ST->BootServices->SetTimer(tevent2, TimerRelative, typejiange);
            assert(status, L"SetTimer");
            status = ST->BootServices->WaitForEvent(1, wait_list2, &idx);
            assert(status, L"WaitForEvent");
            puts(L"y\n\r");
            break;
        }
        case 18:
        {
            ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN|EFI_BACKGROUND_BLACK);
            puts(L"Connection established.\n\r");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            break;
        }
        case 19:
        {
            puts(L"Downloading data...\n\r");
            break;
        }
        case 20:
        {
            ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN|EFI_BACKGROUND_BLACK);
            puts(L"Download complete.\n\r");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            break;
        }
        case 21:
        {
            puts(L"Upload ready.\n\r");
            break;
        }
        case 22:
        {
            puts(L"Transfer in progress: Len=5512 Header=INT32 ");
            break;
        }
        case 23:
        {
            puts(L"[ ");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_RED|EFI_BACKGROUND_BLACK);
            puts(L"FAIL");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            puts(L" ]\n\r    ");
            break;
        }
        case 24:
        {
            puts(L"FAILURE IN: HCZ_079_PMS, error 02A91 HEX.\n\r    ");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_RED|EFI_BACKGROUND_BLACK);
            puts(L"EndOfStreamException\n\r    Read index out of range (expected 4. received 689)\n\r");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            break;
        }
        case 25:
        {
            puts(L"    Automated exception handling active. Buffer arry resized.\n\r");
            break;
        }
        case 26:
        {
            puts(L"    New firmware version available. Updating...\n\r");
            break;
        }
        default:
            ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN|EFI_BACKGROUND_BLACK);
            puts(L"    Complete.");
            ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE|EFI_BACKGROUND_BLACK);
            getc();
            ST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
            break;
        }
        i++;
    }
    return EFI_SUCCESS;
}
