#!/usr/bin/python3

import serial
import pygame
import time

# Initialize pygame mixer for audio
pygame.mixer.init()

# Set up the connection (change the port if necessary)
ser = serial.Serial('/dev/tty.usbserial-1420', 115200)
THRESHOLD = 200  # Threshold value for the sensor
AUDIO_FILE = 'shepard.mp3'  # Audio file to play when the threshold is crossed
TIME_B4_DISCONNECT = 2

is_playing = False
time_below_threshold = None  # Time when the value first went below 200

def play_audio(file_name):
    global is_playing
    if not is_playing:
        pygame.mixer.music.load(file_name)
        pygame.mixer.music.play(-1)  # -1 makes the music loop indefinitely
        is_playing = True

def stop_audio():
    global is_playing
    if is_playing:
        pygame.mixer.music.stop()
        is_playing = False

try:
    while True:
        if ser.inWaiting():
            data_line = ser.readline().decode('utf-8').strip()  # Read a line from the serial port
            values = data_line.split(',')  # Split CSV values
            
            if len(values) == 5:  # Check if all values are present
                sensorValue0 = int(values[0])
                print(sensorValue0)

                if sensorValue0 > 100:
                    play_audio(AUDIO_FILE)
                    time_below_threshold = None  # Reset the timer as value is above 200 again
                else:
                    if time_below_threshold is None:
                        time_below_threshold = time.time()  # Start the timer when value first goes below 200
                    elif time.time() - time_below_threshold > TIME_B4_DISCONNECT:  # Check if 3 seconds have passed
                        stop_audio()

except KeyboardInterrupt:
    ser.close()
    pygame.mixer.quit()  # Ensure the mixer is closed properly