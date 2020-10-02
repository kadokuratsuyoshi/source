#
#	program:	fclock.py
#	contents:	clock display with frequency counter
#	usage:		python fclock.py
#	install:	sudo pip3 install pyaudio (Raspberry Pi)
#				conda install pyaudio (Windows 10)
#
import pyaudio
import datetime
import numpy as np

RATE = 44100
BPM = 1000
L1 = (300 / BPM * 4) #(60 / BPM * 4)
L2, L4, L8 = (L1/2, L1/4, L1/8)

dt_now = datetime.datetime.now()
ftime = dt_now.strftime('%H%M')
fsec = dt_now.strftime('%H%M%S')
old_fsec = fsec
print(dt_now)
print(ftime)

def tone(freq, length, gain):
	slen = int(length  * RATE)
	t = float(freq) * np.pi * 2 / RATE
	return np.sin(np.arange(slen) * t) * gain

def play_wave(stream, samples):
	stream.write(samples.astype(np.float32).tostring())

p = pyaudio.PyAudio()
for index in range(0, p.get_device_count()):
	print(p.get_device_info_by_index(index))

stream = p.open(format = pyaudio.paFloat32,
	channels = 1,	# monoral
	rate = RATE,	# sampling rate
	frames_per_buffer = 1024,
#	output_device_index = 5,
	output = True)

while True:
	dt_now = datetime.datetime.now()
	ftime = dt_now.strftime('%H%M')
	fsec = dt_now.strftime('%H%M%S')
	if ( old_fsec != fsec ):
		print("play")
		play_wave(stream, tone(ftime, L1, 1.0))
		old_fsec = fsec
		print(fsec)

stream.close()

# eof
