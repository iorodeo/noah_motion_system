from __future__ import print_function
import os 
import sys
from py2gcode import gcode_cmd
from py2gcode import cnc_dxf

feedrate = 150.0
fileName = 'mdf_switch_plate_backing.dxf'
depth = 0.51
startZ = 0.0
safeZ = 0.5
overlap = 0.5
overlapFinish = 0.5
maxCutDepth = 0.15
toolDiam = 3.0/16.0 
direction = 'ccw'
startDwell = 1.0
startCond = 'minX'

prog = gcode_cmd.GCodeProg()
prog.add(gcode_cmd.GenericStart())
prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.FeedRate(feedrate))

param = {
        'fileName'      : fileName,
        'layers'        : ['15/16_through_hole'], 
        'depth'         : depth,
        'startZ'        : startZ,
        'safeZ'         : safeZ,
        'overlap'       : overlap,
        'overlapFinish' : overlapFinish, 
        'toolDiam'      : toolDiam,
        'direction'     : direction,
        'maxCutDepth'   : maxCutDepth,
        'startDwell'    : startDwell,
        }
boundary = cnc_dxf.DxfCircPocket(param)
prog.add(boundary)


prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.End(),comment=True)
baseName, dummy = os.path.splitext(__file__)
fileName = '{0}.ngc'.format(baseName)
print('generating: {0}'.format(fileName))
prog.write(fileName)
