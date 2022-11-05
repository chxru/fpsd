#include <stdlib.h>
#include <avr/io.h>
#include "fps.h"
#include "usart.h"

// data sheet
// https://www.rajguruelectronics.com/Product/1276/R307%20Fingerprint%20Module.pdf

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
  UsartSendChar(dataLength >> 8);
  UsartSendChar(dataLength & 0xFF);

  // send packet command
  UsartSendChar(command);

  // send packet content
  if (data != NULL)
  {
    for (uint16_t i = 0; i < dataLength; i++)
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

  // prepare for receiving response
  uint8_t *dataBuffer = (uint8_t *)malloc(FPS_DEFAULT_RX_DATA_LENGTH);
  uint8_t highBuffer[FPS_DEFAULT_SERIAL_BUFFER_LENGTH] = {0};
  uint16_t highBufferLength = 0;
  uint8_t byteBuffer = 0;
  uint8_t temp = 0;

  // create response with default values
  struct FPSResponse response;
  response.ResponseCode = FPS_RX_BADPACKET;

  // receive FPS_Tx_Header[1]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Header[1])
  {
    return response;
  }

  // receive FPS_Tx_Header[0]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Header[0])
  {
    return response;
  }

  // receive FPS_Tx_Address[3]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[3])
  {
    return response;
  }

  // receive FPS_Tx_Address[2]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[2])
  {
    return response;
  }

  // receive FPS_Tx_Address[1]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[1])
  {
    return response;
  }

  // receive FPS_Tx_Address[0]
  temp = UsartReceiveChar();
  if (temp != FPS_Tx_Address[0])
  {
    return response;
  }

  response = FPSReceivePacket(command);

  return response;
}

struct FPSResponse FPSReceivePacket(uint8_t command)
{
  struct FPSResponse response;
  uint8_t temp;

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
    temp = UsartReceiveChar();
    temp = UsartReceiveChar();
  }

  // ignore checksum
  temp = UsartReceiveChar();
  temp = UsartReceiveChar();
}