#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/uart.h"

#include "buttonmatrix.pio.h"


int main()
{    
    stdio_usb_init();

    // Initialise UART 0
    //uart_init(uart0, 31250);
 
    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(16, GPIO_FUNC_UART);


    unsigned int *mrgTriggerPins = new unsigned int[8] {19, 20, 21, 22, 25, 26, 27, 28};
    for (unsigned int i = 0; i < 4; i++)
    {
        int sm = pio_claim_unused_sm(pio0, true);
        uint offset = pio_add_program(pio0, &buttonmatrix_program);

        buttonmatrix_program_init(pio0, sm, offset, 1, mrgTriggerPins[i * 2], 0);

        pio_sm_set_enabled(pio0, sm, true);
    }


    /*


    unsigned int *mrgTriggerPins = new unsigned int[8] {19, 20, 21, 22, 25, 26, 27, 28};
    unsigned int mNumberOfTriggerPins = 8;
    unsigned int* mrgEchoPins = new unsigned int[16] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    unsigned int mNumberOfEchoPins = 16;
    uint16_t *mrgStatusBits = new uint16_t[mNumberOfTriggerPins]();

    for (unsigned int i = 0; i < mNumberOfTriggerPins; i++)
    {
       gpio_init(mrgTriggerPins[i]); 
       gpio_set_dir(mrgTriggerPins[i], GPIO_OUT);
       gpio_put(mrgTriggerPins[i], 1);
       mrgStatusBits[i] = 0;
    }
    for (unsigned int i = 0; i < mNumberOfEchoPins; i++)
    {
       gpio_init(mrgEchoPins[i]); 
       gpio_set_dir(mrgEchoPins[i], GPIO_IN);
       gpio_pull_up(mrgEchoPins[i]);
    }

    printf("Started!\n");
    sleep_ms(1000);

    const int Bitmask16Bit[16] = {1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15};
    while (true)
    {
        for (unsigned int triggerPinIndex = 0; triggerPinIndex < mNumberOfTriggerPins; triggerPinIndex++)
        {
            gpio_put(mrgTriggerPins[triggerPinIndex], 0);

            // Sleep because it wil take some time to drop the pin
            // We want to sleep 80 ns
            //https://forums.raspberrypi.com/viewtopic.php?t=304922
            // Pico runs at 125 MHZ so one clock cycle is 8ns. So we need 80/8 = 10 nops
            //__asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");

            sleep_us(1);

            uint16_t EchoPins = ~(gpio_get_all() & 0xFFFF);

            gpio_put(mrgTriggerPins[triggerPinIndex], 1);

            if (mrgStatusBits[triggerPinIndex] != EchoPins)
            {
                uint16_t DeltaPins = mrgStatusBits[triggerPinIndex] ^ EchoPins;
                for (unsigned int i = 0; i < 16; i++)
                {
                    int BitMask = Bitmask16Bit[i];
                    bool HasChanged = (DeltaPins & BitMask) > 0;
                    if (HasChanged)
                    {
                        bool IsRising = ((EchoPins & BitMask) > 0);
                        if ((DeltaPins & BitMask) > 0)
                        {
                            int Index = i + triggerPinIndex * 16;
                            printf("Index_%d Trigger_%d Echo_%d %s\n", Index, triggerPinIndex, i, IsRising ? "Rising" : "Falling");
                        }
                    }     
                }
                mrgStatusBits[triggerPinIndex] = EchoPins;
            }

        }
    }*/
}
