#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>			// sleep()
#include <string.h>			// strcmp()

static libusb_device** list;
static libusb_device* found = NULL;
static libusb_context* context = NULL;		// Need to initialize context first before you can do
struct libusb_device_descriptor desc;		// any usb work...

libusb_device_handle* handle;

const uint16_t APPLE_VENDOR_ID = 0x05ac;
const uint16_t IPHONE_DEVICE_ID = 0x12a8;	// iPhone 5/5C/5S/6/SE/7/8/X/XR	
const int BUFFER_SIZE = 0x1000;

void print_r(struct libusb_device* device)
{
	libusb_get_device_descriptor(device, &desc);
	printf("pointer: %p, ", device);
	//printf("bus_number: %u", desc.bus_number);
	printf("vendor: %04x, ", desc.idVendor);
	printf("device: %04x, ", desc.idProduct);
	printf("manufacturer: %04x\n", desc.iManufacturer);
}

int handleCommandLineArgs(int argc, char** argv, char** MODE)
{
	if (argc > 1)
	{
		if (strcmp(argv[1], "READ") == 0)
		{
			*MODE = "READ";
		}
		else if (strcmp(argv[1], "WRITE") == 0)
		{
			*MODE = "WRITE";
		}
		else
		{
			printf("Invalid command line argument\n");
			return 1;
		}
	}
	else
	{
		*MODE = "READ";
	}

	return 0;
}

libusb_device* deviceDiscovery()
{
	ssize_t deviceCount = libusb_get_device_list(context, &list);
	libusb_device* desiredDevice = NULL;

	printf("Device count is: %zu\n", deviceCount);
	for (int i = 0; i < deviceCount; i++)
	{
		libusb_get_device_descriptor(list[i], &desc);
		print_r(list[i]);
		if (desc.idVendor == APPLE_VENDOR_ID)
		{
			desiredDevice = list[i];
			printf("Device found!\n");
			return desiredDevice;
		}
	}

	return NULL;
}

void readDeviceEndpoint(libusb_device* desiredDevice)
{
	int err, transferred;
	char* buffer;
	for (int i = 0; i < 30; i++)
	{
		err = libusb_bulk_transfer(handle, 0x81, buffer, 0x0200, &transferred, 1000);
		printf("i: %d, host address: %p, ", i, &buffer);
		for (int j = 0; j < 10; j++)
		{
			if (j == 0)
			{
				printf("data: 0x%x", *(buffer+j));
			}
			else
			{
				printf(" 0x%x", *(buffer+j));
			}
		}
		printf(", bytes transferred: %d\n", transferred);
		sleep(1);
	}
}

void writeToDevice(libusb_device* desiredDevice)
{
	int err, transferred;
	char* buffer = "God is good!";
	for (int i = 0; i < 30; i++)
	{
		err = libusb_bulk_transfer(handle, 0x02, buffer, 0x0200, &transferred, 1000);
		printf("i: %d, host address: %p, ", i, &buffer);
		for (int j = 0; j < 10; j++)
		{
			if (j == 0)
			{
				printf("data: 0x%x", *(buffer+j));
			}
			else
			{
				printf(" 0x%x", *(buffer+j));
			}
		}
		printf(", bytes transferred: %d\n", transferred);
		sleep(1);
	}

}

int main(int argc, char** argv)
{
	int result = libusb_init(&context);
	if (result < 0)
	{
		fprintf(stderr, "libusb_init() failed with %d.\n", result);
		return result;
	}
	int err;
	char* MODE;
	libusb_device* desiredDevice = NULL;
	//char buffer[BUFFER_SIZE];

	// Uncomment the next line to see if the buffer is getting overwritten
	//*(buffer + 1) = 'a';

	handleCommandLineArgs(argc, argv, &MODE);
	desiredDevice = deviceDiscovery();

	// Open device and read byte stream
	err = libusb_open(desiredDevice, &handle);
	if (err)
	{
		printf("Error no: %i", err);
	}

	if (strcmp(MODE, "READ") == 0)
	{
		readDeviceEndpoint(desiredDevice);
	}
	else if (strcmp(MODE, "WRITE") == 0)
	{
		writeToDevice(desiredDevice);
	}

	// Cleanup
	libusb_close(handle);
	libusb_exit(context);

	return 0;
}


