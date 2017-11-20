#include<windows.h>
#include<stdio.h>
#include <iostream>
#include <atlbase.h>

bool writeDataToSerial(LPCSTR port, char data[], size_t size) {
	HANDLE hComm;

	USES_CONVERSION;
	hComm = CreateFile(A2W(port),                //port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices

	DCB dcbSerialParams = { 0 }; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	bool Status = GetCommState(hComm, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None

	SetCommState(hComm, &dcbSerialParams);

	//char data[] = { '<', char(1) ,'>' }; // "A";
	DWORD dNoOFBytestoWrite;         // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	dNoOFBytestoWrite = size; // sizeof(data);

	Status = WriteFile(hComm,        // Handle to the Serial port
		data,     // Data to be written to the port
		dNoOFBytestoWrite,  //No of bytes to write
		&dNoOfBytesWritten, //Bytes written
		NULL);

	CloseHandle(hComm);//Closing the Serial Port

	return Status;
}

int main()
{
	char data[13] = { '<', 
		char(255), char(255), char(255), char(255), char(255),
		char(255), char(255), char(255), char(255), char(255),
		'>', '\0' };

	bool status = writeDataToSerial("COM4", data, sizeof(data));

	if (status == true)
		printf("Write data FF. Status OK\n");
	else
		printf("Write data FF. Status FAIL\n");

	// Skip first and last. Since they are start and end of communication.
	for (size_t i = 1; i < 11; i++)
		data[i] = char(1);

	status = writeDataToSerial("COM4", data, sizeof(data));

	if (status == true)
		printf("Write data 01. Status OK\n");
	else
		printf("Write data 01. Status FAIL\n");

	getchar();

	return 0;
}