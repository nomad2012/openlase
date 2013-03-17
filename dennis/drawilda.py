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

import sys

points_left = 0
include_dark_points = 0
gbl_time = 0
gbl_frames = 0
ftime = 0
save_x = 0
save_y = 0
save_color = 0

BPM=135
AB=(135.0/60.0)


def cutoff(x, y, color):
	points_dot = 0;
	if (not(include_dark_points) and (color == C_BLACK)):
		if (points_left):
			return
		x = save_x;
		y = save_y;
	
	if (points_left):
		points_left -= 1
		save_x = x
		save_y = y
		save_color = color
		points_dot = 200
	else:
		x = save_x;
		y = save_y;
		if (points_dot):
			color = C_WHITE
			points_dot -= 1
	        else:
			color = C_BLACK


def render():
        global gbl_frames
        global gbl_time
        global audiotime
        
	ftime = ol.renderFrame(150)
	gbl_frames += 1
	gbl_time += ftime

	audiotime = gbl_time * AB

        print "Frame time: {}, FPS:{}, Avg FPS:{}, Audio: {}".format(ftime, 1/ftime, gbl_frames/gbl_time, audiotime)
	return ftime


def count_active_points(ild):
	count = 0
	for p in ild.points:
		if (include_dark_points or p.is_blank):
			count += 1
	return count



if ol.init(10) < 0:
	sys.exit(1)
        
params = ol.RenderParams()
params.render_flags = ol.RENDER_NOREORDER | ol.RENDER_GRAYSCALE
params.on_speed = 2/120.0
params.flatness = 0.000001
params.off_speed = 2.0/20.0
params.start_wait = 8
params.end_wait = 1
ol.setRenderParams(params)


ild_file_name = sys.argv[1]
ild = ol.loadIlda(ild_file_name)


include_dark_points = 0
#count = count_active_points(ild)
cur_draw = 0

#ol.setVertexShader(cutoff)

while(cur_draw <= 30):
	points_left = cur_draw
	ol.drawIlda(ild)
	render()
	cur_draw += ftime
        
#ol.setVertexShader(NULL)
bright = 300.0

while(bright >= 0):
	if (bright > 255.0):
		col = C_WHITE
        else:
		col = C_GREY(bright)
	ol.pushColor()
	ol.multColor(col)
	ol.drawIlda(ild)
	ol.popColor()
	render()
	bright -= ftime * 40.0
