#include "si2_uart_for_pi.c"
#include "mavlink.h"
#include <stdio.h>


int main(){
   mavlink_message_t msg0;
   uint8_t buf[MAVLINK_MAX_PACKET_LEN];
   mavlink_msg_message_interval_pack(255, 0, &msg0, 0,1000);
   uint16_t len = mavlink_msg_to_send_buffer(buf, &msg0);
   uart0_send(buf, len);

   mavlink_message_t msg;
    mavlink_status_t status;

   while(1){
    while(uart0_char_available())
    {
        uint8_t c = uart0_get_char();
        if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {

            switch(msg.msgid)
            {
                    case MAVLINK_MSG_ID_HEARTBEAT:
                    {
                       printf("Got them!");
                    }
                    break;
            default:
                break;
            }
        }

    }
    }

}
