# Target

iPhone 5s model

	-1.3 GHz
	-64 bit
	-Apple A7 processor
	-Model release date: September 20, 2013


Usb Protocol:



# Todo:

```
Done -include libusb library
Done -get usb bytestream
In Progress -interpret bytestream
-inject fun stuff:

	-make phone execute assembly instruction
	-make phone execute command

```

# Compilation

```

gcc play.c -lusb-1.0				# I have much to learn about linux :(
gcc play.c -lusb-1.0 -fno-stack-protector 	# In case of stack smashing canary
./a.out


```

# References

https://www.keil.com/pack/doc/mw/usb/html/_u_s_b__descriptors.html

