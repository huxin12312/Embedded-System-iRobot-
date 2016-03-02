/** 
 * Open Interface API - Provides a set of functions for controlling the Create 
 * Documentation: http://www.irobot.com/filelibrary/pdfs/hrd/create/create%20open%20interface_v2.pdf
 *
 * void main() {
 *    oi_sensors_t *robot = oi_alloc();
 *    oi_init(robot);
 *
 *    // ... your code ...
 *
 *    free(robot);
 * }
 * 
 * @author See "Robotics Primer Workbook" project hosted on SourceForge.Net; Edited for clarity by Chad Nelson
 * @date 06/26/2012
 */

#ifndef CREATE_H
#define CREATE_H

#define FOSC 16000000

#include <inttypes.h>
#include <avr/io.h>

#define OI_OPCODE_START            128
#define OI_OPCODE_BAUD             129
#define OI_OPCODE_CONTROL          130
#define OI_OPCODE_SAFE             131
#define OI_OPCODE_FULL             132
#define OI_OPCODE_POWER            133
#define OI_OPCODE_SPOT             134
#define OI_OPCODE_CLEAN            135
#define OI_OPCODE_MAX              136
#define OI_OPCODE_DRIVE            137
#define OI_OPCODE_MOTORS           138
#define OI_OPCODE_LEDS             139
#define OI_OPCODE_SONG             140
#define OI_OPCODE_PLAY             141
#define OI_OPCODE_SENSORS          142
#define OI_OPCODE_FORCEDOCK        143

#define OI_OPCODE_PWM_MOTORS       144
#define OI_OPCODE_DRIVE_WHEELS     145
#define OI_OPCODE_DRIVE_PWM        146
#define OI_OPCODE_OUTPUTS          147
#define OI_OPCODE_STREAM           148
#define OI_OPCODE_QUERY_LIST       149
#define OI_OPCODE_DO_STREAM        150
#define OI_OPCODE_SEND_IR_CHAR     151
#define OI_OPCODE_SCRIPT           152
#define OI_OPCODE_PLAY_SCRIPT      153
#define OI_OPCODE_SHOW_SCRIPT      154
#define OI_OPCODE_WAIT_TIME        155
#define OI_OPCODE_WAIT_DISTANCE    156
#define OI_OPCODE_WAIT_ANGLE       157
#define OI_OPCODE_WAIT_EVENT       158

// Contains Packets 7-26
#define OI_SENSOR_PACKET_GROUP0 0
// Contains Packets 7-16
#define OI_SENSOR_PACKET_GROUP1 1
// Contains Packets 17-20
#define OI_SENSOR_PACKET_GROUP2 2
// Contains Packets 21-26
#define OI_SENSOR_PACKET_GROUP3 3
// Contains Packets 27-34
#define OI_SENSOR_PACKET_GROUP4 4
// Contains Packets 35-42
#define OI_SENSOR_PACKET_GROUP5 5
// Contains Packets 7-42
#define OI_SENSOR_PACKET_GROUP6 6

#define MIN(a,b) ((a < b) ? (a) : (b))
#define MAX(a,b) ((a > b) ? (a) : (b))

#define PIN_0 0x01
#define PIN_1 0x02
#define PIN_2 0x04
#define PIN_3 0x08
#define PIN_4 0x10
#define PIN_5 0x20
#define PIN_6 0x40
#define PIN_7 0x80

/// iRobot Create Sensor Data
typedef struct {
	// Sensor statuses (booleans)
	uint8_t bumper_right : 1;
	uint8_t bumper_left : 1;
	uint8_t wheeldrop_right : 1;
	uint8_t wheeldrop_left : 1;
	uint8_t wheeldrop_caster : 1;
	uint8_t wall; // not virtual wall
	uint8_t cliff_left;
	uint8_t cliff_frontleft;
	uint8_t cliff_frontright;
	uint8_t cliff_right;
	uint8_t virtual_wall; // omni-directional IR sensor
	
	// Over current information
	uint8_t overcurrent_ld1 : 1;
	uint8_t overcurrent_ld0 : 1;
	uint8_t overcurrent_ld2 : 1;
	uint8_t overcurrent_driveright : 1;
	uint8_t overcurrent_driveleft : 1;

	uint16_t unused_bytes;
	
	uint8_t infrared_byte;
	uint8_t button_play : 2;
	uint8_t button_advance : 1;

	int16_t distance; // in millimeters
	int16_t angle;    // in degrees; counterclockwise is positive; clockwise is negative

	// Battery information
	uint8_t charging_state;
	uint16_t voltage; // mV
	int16_t current; // mA
	int8_t temperature; // Celcius
	uint16_t charge;
	uint16_t capacity; // mA-hrs
	
	// Light sensor signal strengths
	uint16_t wall_signal;
	uint16_t cliff_left_signal;
	uint16_t cliff_frontleft_signal;
	uint16_t cliff_frontright_signal;
	uint16_t cliff_right_signal;
	
	// Cargo bay info
	uint8_t cargo_bay_io0 : 1;
	uint8_t cargo_bay_io1 : 1;
	uint8_t cargo_bay_io2 : 1;
	uint8_t cargo_bay_io3 : 1;
	uint8_t cargo_bay_baud : 1;
	uint16_t cargo_bay_voltage;
	
	uint8_t internal_charger_on : 1;
	uint8_t home_base_charger_on : 1;
	
	uint8_t oi_mode; // off, passive, safe, full
	
	uint8_t song_number;
	uint8_t song_playing;
	
	uint8_t number_packets;
	int16_t requested_velocity;
	int16_t requested_radius;
	int16_t requested_right_velocity;
	int16_t requested_left_velocity;
} oi_t;

typedef oi_t oi_sensors_t;

/// Allocate memory for the oi_sensor_t struct 
oi_t * oi_alloc();

/// Initialize the Create. This must be called first.
void oi_init(oi_t *self);

void oi_free(oi_t *self);

/// Update the Create. This will update all the sensor data.
void oi_update(oi_t *self);

/// \brief Set the LEDS on the Create
/// \param play_led 0=off, 1=on
/// \param advance_led 0=off, 1=on
/// \param power_color (0-255), 0=green, 255=red
/// \param power_intensity (0-255) 0=off, 255=full intensity
void oi_set_leds(uint8_t play_led, uint8_t advance_led, uint8_t power_color, uint8_t power_intensity);

/// \brief Set direction and speed of the robot's wheels
/// \param linear velocity in mm/s values range from -500 -> 500 of right wheel
/// \param linear velocity in mm/s values range from -500 -> 500 of left wheel
void oi_set_wheels(int16_t right_wheel, int16_t left_wheel);

/// \brief Transmit a byte of data over the serial connection to the Create 
/// \param value 8-bit value to transmit to the Create
void oi_byte_tx(unsigned char value);

/// \brief Receive a byte of data from the Create serial connection. Blocks 
/// until a byte is received.
/// \return 8-bit value returned from the Create
unsigned char oi_byte_rx(void);

/// \brief Load song sequence
/// \param song_index integer value from 0 - 15 that acts as a label for note sequence
/// \param num_notes integer value from 1 - 16 indicating the number of notes in the sequence
/// \param notes pointer to a sequence of notes stored as integer values
/// \param duration pointer to a sequence of durations that correspond to the notes
void oi_load_song(int song_index, int num_notes, unsigned char  *notes, unsigned char  *duration);

/// \brief Play song
/// \param An integer value from 0 - 15 that is a previously establish song index
void oi_play_song(int index);

/// Calls in built in demo to send the iRobot to an open home base
/// This will cause the iRobot to enter the Passive state
void go_charge(void);

#endif