#!/usr/bin/python
# -*- coding: utf-8 -*-
#         OpenLase - a realtime laser graphics toolkit
#
# Copyright (C) 2009-2011 Hector Martin "marcan" <hector@marcansoft.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 or version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

import pylase as ol
import serial
import sys
import time
import subprocess


#
# STATE CONSTANTS
#

RESET = 0
RUNNING = 1
STOPPED = 2

# PENALTY

PENALTY_SECONDS = 10.0

#
# SERIAL INPUT CONSTANTS
#

START_COMMAND = "S"
PENALTY_COMMAND = "P"
STOP_COMMAND = "F"
RESET_COMMAND = "R"

def play_sound(filename):
    pipe = subprocess.Popen(['alsaplayer', '-E', filename])
    time.sleep(0.01)
    pipe2 = subprocess.Popen(['alsaplayer', '--jump', '1'])

#
# MAIN ROUTINE
#

if len(sys.argv) > 2:
    port = sys.argv[2]
else:
    port = '/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_6493433313535170E0A0-if00'

serial_port = serial.Serial(port=port, baudrate=57600, timeout=0)


if ol.init(10) < 0:
    sys.exit(1)

params = ol.RenderParams()
params.render_flags = ol.RENDER_NOREORDER | ol.RENDER_GRAYSCALE
params.on_speed = 2/120.0
params.off_speed = 2/30.0
params.flatness = 0.000001
ol.setRenderParams(params)

if len(sys.argv) > 1:
    height = float(sys.argv[1])
else:
    height = 0.5

yoff = (1.0/2.0) * height
t0 = time.time()
t = 0.0
t1 = -2.0
text = "{0:02d}:{1:02d}.{2:02d}".format(int(t//60),int(t%60), int((t - int(t)) * 100))
font = ol.getDefaultFont()
width = ol.getStringWidth(font, height, text)
state = RESET
penalty = 0.0
new_penalty = False

while True:
    
    # check for input
    input_bytes = serial_port.readline()
    if len(input_bytes) > 0:
        print input_bytes,
        if input_bytes[0] == START_COMMAND:
            if state != RUNNING:
                t0 = time.time()
                penalty = 0.0
                state = RUNNING
                play_sound("tick3a.wav")
        elif input_bytes[0] == STOP_COMMAND:
            if state != STOPPED:
                state = STOPPED
                play_sound("laser1a.wav")
        elif input_bytes[0] == PENALTY_COMMAND:
            if state == RUNNING:
                penalty = penalty + PENALTY_SECONDS
                new_penalty = True
                #play_sound("buzzer1.wav")
        elif input_bytes[0] == RESET_COMMAND:
            t = 0.0
            t1 = -2.0
            state = RESET

    # enact state machine
    if state == RUNNING:
        t = time.time() - t0 + penalty
        if new_penalty:
            play_sound("buzzer1.wav")
            new_penalty = False
        elif int(t) > int(t1):
            if int(t) % 2 == 0:
                play_sound("tick3a.wav")
            else:
                play_sound("tick2a.wav")
                
            

    # draw timer
    text = "{0:02d}:{1:02d}.{2:02d}".format(int(t//60),int(t%60), int((t - int(t)) * 100))
    ol.drawString(font, (-width/2,yoff), height, ol.C_WHITE, text)
    ftime = ol.renderFrame(60)

    t1 = t