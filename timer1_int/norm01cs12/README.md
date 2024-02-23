65536(16bits max count) * 256(prescaler)
---------------------------------------- = counter interval
      16000000 (16MHz)

counter interval = 1.048576


           interval(1sec) * 16000000 (16MHz)
counter = ----------------------------------
               256 (prescaler)

counter = 62500
TCNT1 = 65536 - 62500
      = 3036

