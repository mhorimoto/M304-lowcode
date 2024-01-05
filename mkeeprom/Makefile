all:
	@echo nothing

mklc: mklc/mklc.touch

mklc/mklc.touch: mklc/mklc.ino mklc/listCommand.ino mklc/setCommand.ino mklc/clearpage.ino mklc/help.ino
	arduino-cli compile -b arduino:avr:mega mklc
	touch mklc/mklc.touch

inst: mklc/mklc.touch 
	arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:mega mklc

clean: 
	rm mklc/mklc.touch 

