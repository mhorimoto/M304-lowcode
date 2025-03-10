all:
	@echo nothing

main: main/main.touch

main/main.touch: main/main.ino main/cmnd_dump.ino main/cmnd_setbyte.ino main/xmldecode.ino main/resNodeScan.ino main/opeRUN.ino main/opeEEPROM.ino main/opeNET.ino main/opeRTC.ino main/opeSCH.ino main/opeHttpd.ino main/recv16520.ino main/common.ino
	arduino-cli compile -b arduino:avr:mega main
	touch main/main.touch

inst: main/main.touch 
	arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:mega main

clean: 
	rm main/main.touch 

