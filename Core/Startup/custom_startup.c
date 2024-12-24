#include <stdint.h>

/* External symbols from the linker script */
extern uint32_t _sidata;   /* Start address for the initialization values of the .data section (in Flash) */
extern uint32_t _sdata;    /* Start address for the .data section (in SRAM) */
extern uint32_t _edata;    /* End address for the .data section (in SRAM) */
extern uint32_t _sbss;     /* Start address for the .bss section (in SRAM) */
extern uint32_t _ebss;     /* End address for the .bss section (in SRAM) */
extern void __libc_init_array(void); /* Static constructors (part of the standard library) */
extern int main(void);     /* The application's entry point */
extern int primary_bootloader(void);
extern void UART_Transmit_Data(char *data);
extern uint32_t _estack;
/* Function prototypes */
void Reset_Handler(void);
void Default_Handler(void);

/* Exception and interrupt handlers weakly aliased to Default_Handler */
void NMI_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)           __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)          __attribute__((weak, alias("Default_Handler")));

/* Interrupt vector table */
__attribute__((section(".isr_vector")))
const uint32_t g_pfnVectors[] = {
    (uint32_t)&_estack,               /* Initial stack pointer */
    (uint32_t)Reset_Handler,        /* Reset handler */
    (uint32_t)NMI_Handler,          /* NMI handler */
    (uint32_t)HardFault_Handler,    /* Hard fault handler */
    (uint32_t)MemManage_Handler,    /* MPU fault handler */
    (uint32_t)BusFault_Handler,     /* Bus fault handler */
    (uint32_t)UsageFault_Handler,   /* Usage fault handler */
    0, 0, 0, 0,                     /* Reserved */
    (uint32_t)SVC_Handler,          /* SVCall handler */
    (uint32_t)DebugMon_Handler,     /* Debug monitor handler */
    0,                              /* Reserved */
    (uint32_t)PendSV_Handler,       /* PendSV handler */
    (uint32_t)SysTick_Handler,      /* SysTick handler */
    /* Add other IRQ handlers here as needed */
};

/* Reset handler */
void Reset_Handler(void) {
    uint32_t *src, *dst;

    /* Copy the .data section initializers from Flash to SRAM */
    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero fill the .bss section */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call the static constructors */
    __libc_init_array();

    /* Call the application's entry point */
    while(primary_bootloader()!=0);
    //primary_bootloader();
    /* Infinite loop in case main returns */
    UART_Transmit_Data("HARD RESET REQUIRED NOW");
}

/* Default interrupt handler */
__attribute__((weak)) void Default_Handler(void) {
    while (1); /* Hang indefinitely for debugging */
}
