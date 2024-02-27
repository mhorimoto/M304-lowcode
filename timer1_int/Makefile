all:
	@echo nothing

norm01cs10: norm01cs10/norm01cs10.touch

norm01cs10/norm01cs10.touch: norm01cs10/norm01cs10.ino
	arduino-cli compile -b arduino:avr:mega norm01cs10
	touch norm01cs10/norm01cs10.touch

instnorm01cs10: norm01cs10/norm01cs10.touch 
	arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:mega norm01cs10

cleannorm01cs10: 
	rm norm01cs10/norm01cs10.touch 

norm01cs12: norm01cs12/norm01cs12.touch

norm01cs12/norm01cs12.touch: norm01cs12/norm01cs12.ino
	arduino-cli compile -b arduino:avr:mega norm01cs12
	touch norm01cs12/norm01cs12.touch

instnorm01cs12: norm01cs12/norm01cs12.touch 
	arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:mega norm01cs12

cleannorm01cs12: 
	rm norm01cs12/norm01cs12.touch 

norm02: norm02/norm02.touch

norm02/norm02.touch: norm02/norm02.ino
	arduino-cli compile -b arduino:avr:mega norm02
	touch norm02/norm02.touch

instnorm02: norm02/norm02.touch 
	arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:mega norm02

cleannorm02: 
	rm norm02/norm02.touch 

