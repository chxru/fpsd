#include <stdlib.h>
#include <avr/io.h>
#include <avr/delay.h>

#include "fps.h"
#include "usart.h"
#include "lcd.h"

// data sheet
// https://www.electronicscomp.com/datasheet/r307-fingerprint-module-user-manual.pdf

// common parameters
const char FPS_Tx_Header[2] = {FPS_ID_STARTCODE & 0xFFU, (FPS_ID_STARTCODE >> 8) & 0xFFU};
const char FPS_Tx_Address[4] = {FPS_DEFAULT_ADDRESS & 0xFFU, (FPS_DEFAULT_ADDRESS >> 8) & 0xFFU, (FPS_DEFAULT_ADDRESS >> 16) & 0xFFU, (FPS_DEFAULT_ADDRESS >> 24) & 0xFFU};

// Send command to Finger Print Sensor
struct FPSResponse FPSSendCommand(uint8_t command, uint8_t *data, uint16_t dataLength)
{
  /*
   Packet transmitted to the r307 has the following format:

   +--------+---------+-------------------+---------------+----------------+----------+
   | Header | Address | Packet Identifier | Packet Length | Packet Content | Checksum |
   +--------+---------+-------------------+---------------+----------------+----------+
   |      2 |       4 |                 1 |             2 |              - |        2 |
   +--------+---------+-------------------+---------------+----------------+----------+

    Header: 0xEF01, high byte first
    Address: 0xFFFFFFFF (default), high byte first
    Checksum: 2 bytes, high byte first
  */

  // special case for FPS_CMD_VERIFYPASSWORD
  if (command == FPS_CMD_VERIFYPASSWORD)
  {
    data[0] = FPS_DEFAULT_PASSWORD & 0xFFU;
    data[1] = (FPS_DEFAULT_PASSWORD >> 8) & 0xFFU;
    data[2] = (FPS_DEFAULT_PASSWORD >> 16) & 0xFFU;
    data[3] = (FPS_DEFAULT_PASSWORD >> 24) & 0xFFU;

    dataLength = 4;
  }

  // send header
  UsartSendChar(FPS_Tx_Header[1]);
  UsartSendChar(FPS_Tx_Header[0]);

  // send address
  UsartSendChar(FPS_Tx_Address[3]);
  UsartSendChar(FPS_Tx_Address[2]);
  UsartSendChar(FPS_Tx_Address[1]);
  UsartSendChar(FPS_Tx_Address[0]);

  // send packet identifier
  UsartSendChar(FPS_ID_COMMANDPACKET);

  // send packet length
  dataLength += 3; // add command and checksum length
  UsartSendChar((dataLength >> 8) & 0xFFU);
  UsartSendChar(dataLength & 0xFF);

  // send packet command
  UsartSendChar(command);

  // send packet content
  if (data != NULL || dataLength != 0)
  {
    for (uint16_t i = dataLength - 3; i > 0; i--)
    {
      UsartSendChar(data[i]);
    }
  }

  // send checksum
  uint16_t checksum = FPS_ID_COMMANDPACKET + dataLength + command;
  for (uint16_t i = 0; i < dataLength; i++)
  {
    checksum += data[i];
  }
  UsartSendChar(checksum >> 8);
  UsartSendChar(checksum & 0xFF);

  LCD_Message("FPS", "Request Sent");

  _delay_ms(1000);

  // prepare for receiving response
  uint8_t temp = 0;

  // create response with default values
  struct FPSResponse response;
  response.ResponseCode = FPS_RX_BADPACKET;

  // receive FPS_Tx_Header[1]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Header[1])
  {
    LCD_Message("Error", "FPS_Tx_Header[1]");

    return response;
  }

  // receive FPS_Tx_Header[0]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Header[0])
  {
    LCD_Message("Error", "FPS_Tx_Header[0]");
    return response;
  }

  // receive FPS_Tx_Address[3]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[3])
  {
    LCD_Message("Error", "FPS_Tx_Address[3]");
    return response;
  }

  // receive FPS_Tx_Address[2]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[2])
  {
    LCD_Message("Error", "FPS_Tx_Address[2]");
    return response;
  }

  // receive FPS_Tx_Address[1]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[1])
  {
    LCD_Message("Error", "FPS_Tx_Address[1]");
    return response;
  }

  // receive FPS_Tx_Address[0]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[0])
  {
    LCD_Message("Error", "FPS_Tx_Address[0]");
    return response;
  }

  LCD_Message("FPS", "Parsing Response");

  response = FPSReceivePacket(command);

  return response;
}

struct FPSResponse FPSReceivePacket(uint8_t command)
{
  struct FPSResponse response;

  // receive package identifier
  response.ResponseCode = UsartReceiveChar();

  // pid is not a ack
  if (response.ResponseCode != FPS_ID_ACKPACKET)
  {
    return response;
  }

  // receive packet length
  response.DataLength = UsartReceiveChar();
  response.DataLength <<= 8;
  response.DataLength |= UsartReceiveChar();

  // receive confirmation command
  response.ResponseCode = UsartReceiveChar();

  if (response.ResponseCode != FPS_RX_OK)
  {
    // something is f'd
    return response;
  }

  if (command == FPS_CMD_SCANANDFULLSEARCH)
  {
    // receive page id
    response.Data = UsartReceiveChar();
    response.Data <<= 8;
    response.Data |= UsartReceiveChar();

    // ignore match score
    UsartReceiveChar();
    UsartReceiveChar();
  }

  // ignore checksum
  UsartReceiveChar();
  UsartReceiveChar();

  return response;
}