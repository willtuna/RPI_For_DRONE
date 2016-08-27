#include "Weiting_uart.c"
#include "mavlink.h"
static void communication_receive(uart &);



int main (int argc, char* argv[] ){

uart uart0;
		
mavlink_system_t mavlink_system;
 
mavlink_system.sysid = 20;                   ///< ID 20 for this airplane
mavlink_system.compid = MAV_COMP_ID_IMU;     ///< The component sending the message is the IMU, it could be also a Linux process
 
//Define the system type, in this case an airplane
uint8_t system_type = MAV_TYPE_FIXED_WING;
uint8_t autopilot_type = MAV_AUTOPILOT_GENERIC;
 
uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
uint32_t custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
uint8_t system_state = MAV_STATE_STANDBY; ///< System ready for flight
  
  // Initialize the required buffers
mavlink_message_t msg;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];
   
   // Pack the message
mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid,
   &msg, system_type, autopilot_type, system_mode, custom_mode,
   system_state);
    
    // Copy the message to the send buffer
uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
     
     // Send the message with the standard UART send function
     // uart0_send might be named differently depending on
      // the individual microcontroller / library in use.
uart0.uart0_open();

uart0.uart0_send(buf, len);

communication_receive(uart0);



 }


static int packet_drops = 0;
static int mode = 0;//MAV_MODE_UNINIT; /* Defined in mavlink_types.h, which is included by mavlink.h */
 
/**
  * @brief Receive communication packets and handle them
  *
  * This function decodes packets on the protocol level and also handles
  * their value by calling the appropriate functions.
  */
static void communication_receive(uart & uart0)
{
mavlink_message_t msg;
mavlink_status_t status;
						 
// COMMUNICATION THROUGH EXTERNAL UART PORT (XBee serial)
						 
		while(uart0.uart0_char_available())
		{
				uint8_t c = uart0.uart0_get_char();
// Try to get a new message
		if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
				
				switch(msg.msgid)
				case MAVLINK_MSG_ID_HEARTBEAT:
		{
				{// E.g. read GCS heartbeat and go into
				// comm lost mode if timer times out
				
				}
						break;
				case MAVLINK_MSG_ID_COMMAND_LONG:
								// EXECUTE ACTION
						break;
				default:
								//Do nothing
						break;
		}

		}

		}
/*																						 
// And get the next one
// Update global packet drops counter
packet_drops += status.packet_rx_drop_count;
// COMMUNICATION THROUGH SECOND UART
										 
		while(uart1_char_available())
		{
		uint8_t c = uart1_get_char();
// Try to get a new message
		if(mavlink_parse_char(MAVLINK_COMM_1, c, &msg, &status))
		{
// Handle message the same way like in for UART0
// you can also consider to write a handle function like
// handle_mavlink(mavlink_channel_t chan, mavlink_message_t* msg)
// Which handles the messages for both or more UARTS
		}
// And get the next one
		}		
												 
// Update global packet drops counter

packet_drops += status.packet_rx_drop_count;
*/
}
