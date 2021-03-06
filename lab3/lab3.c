#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "keyboard_macros.h"
#include "utils.h"

extern uint8_t bb[];
extern bool two_byte;
extern int size;
extern int kbd_error;
extern uint32_t no_interrupts;
uint8_t kbc_cmd_byte=0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status, r;
  uint8_t keyboard_sel;
  message msg;
  bool make;

  if(kbd_subscribe_int(&keyboard_sel))
    return 1;
  int irq_set = BIT(keyboard_sel);
  int process = 1;

    while( process ) { /* You may want to use a different condition */
     /* Get a request message. */
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
     if (is_ipc_notify(ipc_status)) { /* received notification */
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE: /* hardware interrupt notification */       
                 if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                      kbc_ih();/* process it */
                      if(two_byte || kbd_error){
                        continue;
                      }
                      keyboard_get_code(&make, bb);
                      kbd_print_scancode(make, size, bb);
                 }
                 break;
             default:
                 break; /* no other notifications expected: do nothing */ 
         }
         if(keyboard_check_esc(bb))
            process=0;
     } else { /* received a standard message, not a notification */
         /* no standard messages expected: do nothing */
     }
  }
  kbd_unsubscribe_int();
  kbd_print_no_sysinb(getCounter());
  return OK;
}

int(kbd_test_poll)() {
  int processing = 1;
  bool make;

  while(processing){
    kbd_polling();/* process it */
    if(two_byte|| kbd_error){
      continue;
    }
    keyboard_get_code(&make, bb);
    kbd_print_scancode(make, size, bb);
    if(keyboard_check_esc(bb)){
        processing=0;
    }
  }

  kbd_enable_int();
  kbd_print_no_sysinb(getCounter());
  return OK;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r;
  uint8_t keyboard_sel;
  uint8_t timer0_sel;
  message msg;
  bool make;

  if(kbd_subscribe_int(&keyboard_sel)) return 1;
  if(timer_subscribe_int(&timer0_sel)) return 1;
  int kbd_irq_mask = BIT(keyboard_sel);
  int timer0_irq_mask = BIT(timer0_sel);
  
  uint8_t time = 0;
  uint8_t frequency = 60;

  int process = 1;

    while( process ) { /* You may want to use a different condition */
     /* Get a request message. */
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
     if (is_ipc_notify(ipc_status)) { /* received notification */
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & timer0_irq_mask){
                  timer_int_handler();
                  if (no_interrupts % frequency == 0) time++;
                  if (time >= n) process = 0;
                }
                if (msg.m_notify.interrupts & kbd_irq_mask) { /* subscribed interrupt */
                    kbc_ih();/* process it */
                    if(two_byte || kbd_error){
                      continue;
                    }
                    keyboard_get_code(&make, bb);
                    kbd_print_scancode(make, size, bb);
                    no_interrupts = 0;
                    time = 0;
                }
                break;
             default:
                 break; /* no other notifications expected: do nothing */ 
         }
         if(keyboard_check_esc(bb))
            process=0;
     } else { /* received a standard message, not a notification */
         /* no standard messages expected: do nothing */
     }
  }
  kbd_unsubscribe_int();
  timer_unsubscribe_int();
  return OK;
}

