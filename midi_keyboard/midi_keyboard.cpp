#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/uart.h"

#include "bsp/board.h"
#include "tusb.h"

#include "buttonmatrix.pio.h"

void pio_irq_handler()
{
    for (uint sm = 0; sm < 4; sm++)
    {
        if (pio_interrupt_get(pio0, sm))
        {
            uint rx_level = pio_sm_get_rx_fifo_level(pio0, sm);
            // Check if SM has triggered the interrupt
            while (rx_level > 0)
            {
                if (rx_level == 1)
                {
                    pio_sm_clear_fifos(pio0, sm); //should never occur, but flush if so
                    break;
                }
                
                uint32_t stateNew = pio_sm_get_blocking(pio0, sm); // Read FIFO data
                uint32_t stateOld = pio_sm_get_blocking(pio0, sm); // Read FIFO data
                if (stateOld != stateNew)
                {
                    uint32_t diffBits = stateOld ^ stateNew; // Bits that differ between stateOld and stateNew
                    uint32_t risingBits = stateNew & diffBits; // Bits that went from 0 to 1
                    uint32_t fallingBits = stateOld & diffBits; // Bits that went from 1 to 0

                    // Handle data (e.g., signal main loop, process data, etc.)
                    //printf("SM %d 0x%08X -> 0x%08X\n", sm, stateOld, stateNew);

                    for (uint i = 0; i < 32; ++i)
                    {
                        uint fullIndex = i + 32 * sm;
                        if (risingBits & (1 << i))
                        {
                            printf("%03d rising\n", fullIndex);
                        }
                        else if (fallingBits & (1 << i))
                        {
                            printf("%03d falling\n", fullIndex);
                        }
                    }
                }
                
                rx_level -= 2;
            }
            pio_interrupt_clear(pio0, sm); // Clear interrupt for this state machine
        }
    }
}


int main()
{    
    board_init();
    tusb_init();

    // Initialise UART 0
    uart_init(uart0, 31250);
 
    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(16, GPIO_FUNC_UART);


    uint offset = pio_add_program(pio0, &buttonmatrix_program);

    unsigned int *mrgTriggerPins = new unsigned int[8] {19, 20, 21, 22, 25, 26, 27, 28}; //25 28 27 19 20 21 22 26   
    unsigned int* mrgEchoPins = new unsigned int[16] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; 
    for (unsigned int i = 0; i < 4; i++)
    {
        int sm = pio_claim_unused_sm(pio0, true);
        buttonmatrix_program_init(pio0, sm, offset, mrgTriggerPins[i * 2], mrgEchoPins[0]);
    }

    // start from end to start
    for (unsigned int i = 0; i < 4; i++)
    {
        pio_sm_set_enabled(pio0, 3 - i, true);
    }

    irq_set_exclusive_handler(PIO0_IRQ_0, pio_irq_handler);
    irq_set_enabled(PIO0_IRQ_0, true);
    pio_set_irq0_source_mask_enabled(pio0, (1 << pis_interrupt0) | (1<<pis_interrupt1) | (1 << pis_interrupt2) | (1 << pis_interrupt3), true);

    while (true)
    {
        tud_task(); // tinyusb device task
    }
}
