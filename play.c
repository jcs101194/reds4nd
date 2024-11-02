#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>				// sleep()

static libusb_device** list;
static libusb_device* found = NULL;
static libusb_context* context = NULL;		// Need to initialize context first before you can do
struct libusb_device_descriptor desc;		// any usb work...

libusb_device_handle* handle;

const uint16_t APPLE_VENDOR_ID = 0x05ac;
const uint16_t IPHONE_DEVICE_ID = 0x12a8;	// iPhone 5/5C/5S/6/SE/7/8/X/XR	


void print_r(struct libusb_device* device)
{
	libusb_get_device_descriptor(device, &desc);
	printf("pointer: %p, ", device);
	//printf("bus_number: %u", desc.bus_number);
	printf("vendor: %04x, ", desc.idVendor);
	printf("device: %04x, ", desc.idProduct);
	printf("manufacturer: %04x\n", desc.iManufacturer);
}


int main()
{
	int result = libusb_init(&context);
	int err;
	int* transferred;
	libusb_device* desiredDevice = NULL;
	char* dataStream;

	if (result < 0)
	{
		fprintf(stderr, "libusb_init() failed with %d.\n", result);
		return result;
	}

	ssize_t deviceCount = libusb_get_device_list(context, &list);
	printf("Device count is: %zu\n", deviceCount);
	for (int i = 0; i < deviceCount; i++)
	{
		libusb_get_device_descriptor(list[i], &desc);
		print_r(list[i]);
		if (desc.idVendor == APPLE_VENDOR_ID)
		{
			desiredDevice = list[i];
			printf("Device found!\n");
			break;
		}
	}

	// Open device and read byte stream
	err = libusb_open(desiredDevice, &handle);
	if (err)
	{
		printf("Error no: %i", err);
	}

	for (int i = 0; i < 100; i++)
	{
		err = libusb_bulk_transfer(handle, 0x100, dataStream, 0x100, *transferred, 1000);
		printf("%p\n", dataStream);
		sleep(1);
	}

	return 0;
}


