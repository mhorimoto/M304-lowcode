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

interval = 0.993sec

            0.993 * 16000000
counter = -------------------
                256

counter = 62063
TCNT1 = 65536 - 62063
      = 3473
