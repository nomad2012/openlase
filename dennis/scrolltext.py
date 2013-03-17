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

if ol.init(10) < 0:
	sys.exit(1)
params = ol.RenderParams()
params.render_flags = ol.RENDER_NOREORDER | ol.RENDER_GRAYSCALE
params.on_speed = 2/120.0
params.off_speed = 2/30.0
params.flatness = 0.000001
ol.setRenderParams(params)

height = float(sys.argv[1])
xpos = float(sys.argv[2])
ypos = float(sys.argv[3])
x_scroll_rate = float(sys.argv[4])
y_scroll_rate = float(sys.argv[5])
lines = sys.argv[6:]
lc = len(lines)

font = ol.getDefaultFont()
yoff = (lc/2.0) * height

while True:
	for i,line in enumerate(lines):
		w = ol.getStringWidth(font, height, line)
		ol.drawString(font, (-w/2+xpos,yoff-i*height+ypos), height, ol.C_WHITE, line)

	ftime = ol.renderFrame(30)
        xpos = xpos + x_scroll_rate
        if xpos < -3.0:
           xpos = 3.0
        if xpos > 3.0:
           xpos = -3.0
        ypos = ypos + y_scroll_rate
        if ypos < -3.0:
           ypos = 3.0
        if ypos > 3.0:
           ypos = -3.0
