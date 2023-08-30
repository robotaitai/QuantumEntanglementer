#!/usr/bin/env python3

import serial, time
import colors  # Import the color module
from enum import Enum
import random

ARDUINO_PORT = '/dev/tty.usbmodem14114201'

class Rings(Enum):
    RING_0 = 0
    RING_1 = 1
    RING_2 = 2
    RING_3 = 3
    RING_4 = 4
    RING_5 = 5
    RING_6 = 6
    RING_7 = 7
    RING_8 = 8
    RING_9 = 9
    RING_10 = 10

all_colors = [
    colors.RED, colors.GREEN, colors.BLUE, colors.YELLOW, colors.PURPLE,
    colors.ORANGE, colors.CYAN, colors.MAGENTA, colors.WHITE,
    colors.INDIGO, colors.VIOLET
]
ser = serial.Serial(ARDUINO_PORT, 115200, timeout=1)  # Change 'COM3' to the appropriate port

def set_group_color(group, r, g, b):
    command = f'{group.value - 1}{r:03d}{g:03d}{b:03d}\n'  # Add '\n' to indicate end of line
    ser.write(command.encode())

def set_all_off():
    for i in Rings:
        set_group_color(i, 0, 0, 0)
        time.sleep(0.01)

def send_one_ring(ring, color, TIME):
    set_group_color(ring, color[0], color[1], color[2])
    time.sleep(TIME)
    set_all_off()

def send_another_ring(ring, color, TIME):
    set_group_color(ring, color[0], color[1], color[2])
    time.sleep(TIME)

def pulsating_effect(duration):
    for _ in range(int(duration / 0.2)):  # Adjust the number of steps based on the duration
        for ring in Rings:
            intensity = int(127 + 127 * abs(1 - (_ % 10) / 10))  # Pulsating effect formula
            send_another_ring(ring, (intensity, 0, 0), 0.1)
        time.sleep(0.1)

def chasing_effect(duration):
    for _ in range(int(duration / 0.2)):  # Adjust the number of steps based on the duration
        for ring in Rings:
            send_another_ring(ring, colors.RED, 0.2)
            time.sleep(0.05)
            send_another_ring(ring, colors.BLACK, 0.05)

def alternating_colors(duration, color1, color2):
    for _ in range(int(duration / 0.4)):  # Adjust the number of steps based on the duration
        for ring in Rings:
            if ring.value % 2 == 0:
                send_another_ring(ring, color1, 0.2)
            else:
                send_another_ring(ring, color2, 0.2)

def random_color_burst(duration):
    for _ in range(int(duration / 0.4)):  # Adjust the number of steps based on the duration
        for ring in Rings:
            random_color = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
            send_another_ring(ring, random_color, 0.2)

def color_wave(duration, colors_list):
    for _ in range(int(duration / 0.2)):  # Adjust the number of steps based on the duration
        for i, ring in enumerate(Rings):
            send_another_ring(ring, colors_list[i], 0.2)

def sparkling_stars(duration):
    for _ in range(int(duration / 0.4)):  # Adjust the number of steps based on the duration
        for ring in Rings:
            if random.random() < 0.2:  # Randomly light up 20% of the LEDs in the ring
                random_color = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
                send_another_ring(ring, random_color, 0.2)
            else:
                send_another_ring(ring, colors.BLACK, 0.2)
def following_rainbow():
    send_one_ring(Rings.RING_0, colors.RED, 2)
    send_one_ring(Rings.RING_1, colors.ORANGE, 0.2)
    send_one_ring(Rings.RING_2, colors.YELLOW, 0.2)
    send_one_ring(Rings.RING_3, colors.GREEN, 0.2)
    send_one_ring(Rings.RING_4, colors.BLUE, 0.2)
    send_one_ring(Rings.RING_5, colors.INDIGO, 0.2)
    send_one_ring(Rings.RING_6, colors.VIOLET, 0.2)
    send_one_ring(Rings.RING_7, colors.RED, 0.2)
    send_one_ring(Rings.RING_8, colors.ORANGE, 0.2)
    send_one_ring(Rings.RING_9, colors.YELLOW, 0.2)
    send_one_ring(Rings.RING_10, colors.GREEN, 0.2)

def following_rainbow_2():        
    send_another_ring(Rings.RING_0, colors.PURPLE, 0.2)
    send_another_ring(Rings.RING_1, colors.BLUE, 0.2)
    send_another_ring(Rings.RING_2, colors.GREEN, 0.2)
    send_another_ring(Rings.RING_3, colors.YELLOW, 0.2)
    send_another_ring(Rings.RING_4, colors.ORANGE, 0.2)
    send_another_ring(Rings.RING_5, colors.RED, 0.2)
    send_another_ring(Rings.RING_6, colors.VIOLET, 0.2)
    send_another_ring(Rings.RING_7, colors.INDIGO, 0.2)
    send_another_ring(Rings.RING_8, colors.BLUE, 0.2)
    send_another_ring(Rings.RING_9, colors.GREEN, 0.2)
    send_another_ring(Rings.RING_10, colors.YELLOW, 0.2)

try:
    while True:

        following_rainbow()
        following_rainbow_2()
        print("pulse")
        pulsating_effect(1)  # Pulsating effect for 10 seconds
        print("chase")

        pulsating_effect(1)
        chasing_effect(1)
        
        random.shuffle(all_colors)
        color1, color2 = all_colors[:2]
        
        alternating_colors(1, color1, color2)
        
        random_color_burst(1)
        
        random.shuffle(all_colors)
        color_wave(1, all_colors)
        
        sparkling_stars(10)


except KeyboardInterrupt:
    ser.close()
