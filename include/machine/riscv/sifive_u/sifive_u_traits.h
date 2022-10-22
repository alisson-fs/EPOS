// EPOS SiFive-U (RISC-V) Metainfo and Configuration

#ifndef __riscv_sifive_u_traits_h
#define __riscv_sifive_u_traits_h

#include <system/config.h>

__BEGIN_SYS

class Machine_Common;
template<> struct Traits<Machine_Common>: public Traits<Build>
{
protected:
    static const bool library = (Traits<Build>::MODE == Traits<Build>::LIBRARY);
};

template<> struct Traits<Machine>: public Traits<Machine_Common>
{
public:
    static const unsigned int NOT_USED          = 0xffffffff;

    // Physical Memory
    static const unsigned int RAM_BASE          = 0x0000000080000000;                           // 2 GB
    static const unsigned int RAM_TOP           = 0x0000000087ffffff;                           // 2 GB + 128 MB (max 1536 MB of RAM => RAM + MIO < 2 G)
    static const unsigned int MIO_BASE          = 0x0000000000000000;
    static const unsigned int MIO_TOP           = 0x000000001fffffff;
    //static const unsigned int PAGE_TABLES       = 0x87f00000; // MEM_TOP - 16 MB                       // 512 MB (max 512 MB of MIO => RAM + MIO < 2 G)

    // Physical Memory at Boot
    static const unsigned long BOOT              = NOT_USED;
    static const unsigned long SETUP             = library ? NOT_USED : RAM_BASE;        // RAM_BASE (will be part of the free memory at INIT, using a logical address identical to physical eliminate SETUP relocation)
    static const unsigned long IMAGE             = 0x0000000080100000;                           // RAM_BASE + 1 MB (will be part of the free memory at INIT, defines the maximum image size; if larger than 3 MB then adjust at SETUP)

    // Logical Memory
    static const unsigned long APP_LOW           = library ? RAM_BASE : 0x0000000080400000;      // 2 GB + 4 MB
    static const unsigned long APP_HIGH          = 0x00000000ff7ffff;                           // SYS - 1

    static const unsigned long APP_CODE          = APP_LOW;
    static const unsigned long APP_DATA          = APP_CODE + 4 * 1024 * 1024;

    static const unsigned long INIT              = library ? NOT_USED :0x0000000080080000;       // RAM_BASE + 512 KB (will be part of the free memory at INIT)
    static const unsigned long PHY_MEM           = 0x00000000ff800000;                           // 512 MB (max 1536 MB of RAM)
    static const unsigned long IO                = 0x0000000000000000;                           // 0 (max 512 MB of IO = MIO_TOP - MIO_BASE)
    static const unsigned long SYS               = 0xffffff8000000000;                           // 4 GB - 8 MB


    // Default Sizes and Quantities
    static const unsigned int MAX_THREADS       = 16;
    static const unsigned int STACK_SIZE        = 64 * 1024;
    static const unsigned int HEAP_SIZE         = 1 * 1024 * 1024;
};

template <> struct Traits<IC>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;
};

template <> struct Traits<Timer>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;

    static const unsigned int UNITS = 1;
    static const unsigned int CLOCK = 10000000;

    // Meaningful values for the timer frequency range from 100 to 10000 Hz. The
    // choice must respect the scheduler time-slice, i. e., it must be higher
    // than the scheduler invocation frequency.
    static const int FREQUENCY = 1000; // Hz
};

template <> struct Traits<OTP>: public Traits<Machine_Common>
{
    static const unsigned int BYTES_PER_FUSE = 4;
    static const unsigned int TOTAL_FUSES    = 4096;  // 0x1000, as described by U-Boot.

    static const unsigned int TPW_DELAY      = 20;    // Program pulse width delay
    static const unsigned int TPWI_DELAY     = 5;     // Program pulse interval delay
    static const unsigned int TASP_DELAY     = 1;     // Program address setup delay
    static const unsigned int TCD_DELAY      = 40;    // Read data access delay
    static const unsigned int TKL_DELAY      = 10;    // Clock pulse low delay
    static const unsigned int TMS_DELAY      = 1;     // PTM mode setup delay
};

template <> struct Traits<IO>: public Traits<Machine_Common> {};

template <> struct Traits<UART>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = 2;

    static const unsigned int CLOCK = 22729000;

    static const unsigned int DEF_UNIT = 1;
    static const unsigned int DEF_BAUD_RATE = 115200;
    static const unsigned int DEF_DATA_BITS = 8;
    static const unsigned int DEF_PARITY = 0; // none
    static const unsigned int DEF_STOP_BITS = 1;
};

template<> struct Traits<Serial_Display>: public Traits<Machine_Common>
{
    static const bool enabled = (Traits<Build>::EXPECTED_SIMULATION_TIME != 0);
    static const int ENGINE = UART;
    static const int UNIT = 1;
    static const int COLUMNS = 80;
    static const int LINES = 24;
    static const int TAB_SIZE = 8;
};

template<> struct Traits<Scratchpad>: public Traits<Machine_Common>
{
    static const bool enabled = false;
};

__END_SYS

#endif
