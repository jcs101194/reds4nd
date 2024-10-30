#include <stdio.h>
#include <libusb-1.0/libusb.h>

static libusb_device** list;
static libusb_device* found = NULL;
static libusb_context* context = NULL;	// Need to initialize context first before you can do
					// any usb work...

/*
void print_r(struct libusb_device* device)
{
	struct libusb_device dv = &device;
	printf("bus_number: %u", dv.bus_number);

}
*/

int main()
{
	int result = libusb_init(&context);
	struct libusb_device_descriptor* desc;

	if (result < 0)
	{
		fprintf(stderr, "libusb_init() failed with %d.\n", result);
		return result;
	}

	ssize_t deviceCount = libusb_get_device_list(context, &list);
	printf(libusb_strerror(result)); printf("\n");
	printf("hello juan\n");
	printf("Also device count is: %zu\n", deviceCount); 
	//print_r(list[0]);
	for (int i = 0; i < deviceCount; i++)
	{
		printf("pointer: %p\n", list[i]);
	}
	//libusb_get_device_descriptor(list[0], desc);


	return 0;
}


