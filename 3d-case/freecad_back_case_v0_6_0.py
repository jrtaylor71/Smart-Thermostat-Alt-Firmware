#!/usr/bin/env python3
"""
ESP32-S3 Smart Thermostat - BACK CASE (Wall-mounting side) v0.6.0
FreeCAD Python script to generate back case geometry for PCB v0.6.0
This case mounts to the wall and provides wiring/mounting holes
"""

import FreeCAD as App
import Part
import Mesh
import MeshPart
import os

# Create a new document
doc = App.newDocument("BackCaseV0_6_0")

# ---------- Parameters ----------

# PCB dimensions (Thermostat V0.6.0 edge cuts)
pcb_length = 101.488
pcb_width = 83.44
pcb_thickness = 1.6

# PCB mounting holes (relative to lower-left board origin)
pcb_mount_holes = [
    [3.068, 3.040],
    [3.068, 80.390],
    [6.608, 14.230],
    [6.608, 63.130],
    [70.358, 68.790],
    [89.718, 14.230],
    [89.718, 63.130],
    [97.628, 49.950],
    [98.438, 3.040]
]

# Case parameters
side_wall_thickness = 9.0
bottom_thickness = 4.0
wall_thickness = side_wall_thickness  # legacy name used throughout
pcb_clearance = 3.0
standoff_height = 4.0
component_height = 8.0

# Calculated dimensions
inner_length = pcb_length + (2 * pcb_clearance)
inner_width = pcb_width + (2 * pcb_clearance)
case_length = inner_length + (2 * side_wall_thickness)
case_width = inner_width + (2 * side_wall_thickness)
case_height = bottom_thickness + standoff_height + pcb_thickness + component_height

# Wire entry hole
wire_hole_diameter = 22.0
wire_hole_x = case_length / 2.0
wire_hole_y = case_width / 2.0

# Ventilation slots (front-style, 3mm wide)
vent_slot_count = 15
vent_slot_count_side = 10
vent_slot_length = 8.0
vent_slot_width = 2.0
vent_slot_height = 20.0
vent_spacing = 10.5
vent_corner_margin = 15.0

# Wall mounting holes
wall_mount_hole_dia = 4.0
wall_mount_spacing_x = 83.0
wall_mount_spacing_y = 60.0

# Rounded corners
corner_r = 4.0

# USB port hole (on front case top wall)
usb_rel_x = 59.288
usb_case_x = wall_thickness + pcb_clearance + usb_rel_x
usb_width = 13.0
usb_height = 7.0

# Alignment pin holes (side walls)
pin_hole_diameter = 1.0
pin_hole_height = case_height / 2.0  # Mid-height of case

# Perimeter alignment flange (replaces discrete corner L-tabs)
flange_width = 2.0
flange_protrusion_above = 6.0
flange_height = (case_height - bottom_thickness) + flange_protrusion_above

# Tolerance helper
tolerance = 0.1

def near(a, b):
    return abs(a - b) < tolerance

# ---------- Build Back Case ----------

# Outer shell with rounded corners
outer = Part.makeBox(case_length, case_width, case_height)
try:
    corner_edges = []
    for e in outer.Edges:
        v1 = e.Vertexes[0].Point
        v2 = e.Vertexes[1].Point
        if near(v1.x, v2.x) and near(v1.y, v2.y) and not near(v1.z, v2.z):
            if (near(v1.x, 0) or near(v1.x, case_length)) and (near(v1.y, 0) or near(v1.y, case_width)):
                corner_edges.append(e)
    if corner_edges:
        outer = outer.makeFillet(corner_r, corner_edges)
except Exception as ex:
    App.Console.PrintWarning("Corner fillet failed (non-critical): %s\n" % ex)

# Hollow interior
inner = Part.makeBox(inner_length, inner_width, case_height - bottom_thickness + 1)
inner.translate(App.Vector(side_wall_thickness, side_wall_thickness, bottom_thickness))

shell = outer.cut(inner)
App.Console.PrintMessage("Created back case shell\n")

# ---------- Cutouts ----------

# Wire entry hole through back panel
wire_hole = Part.makeCylinder(wire_hole_diameter / 2.0, bottom_thickness + 1.0,
                               App.Vector(wire_hole_x, wire_hole_y, -0.5),
                               App.Vector(0, 0, 1))
shell = shell.cut(wire_hole)
App.Console.PrintMessage("Cut wire entry hole\n")

# Wall mounting holes - 4 round holes at corners
for x_offset in [-wall_mount_spacing_x / 2.0, wall_mount_spacing_x / 2.0]:
    for y_offset in [-wall_mount_spacing_y / 2.0, wall_mount_spacing_y / 2.0]:
        mount_hole = Part.makeCylinder(wall_mount_hole_dia / 2.0, bottom_thickness + 1.0,
                                        App.Vector(case_length / 2.0 + x_offset,
                                                  case_width / 2.0 + y_offset,
                                                  -0.5),
                                        App.Vector(0, 0, 1))
        shell = shell.cut(mount_hole)
App.Console.PrintMessage("Cut 4 wall mounting holes\n")

# 2 additional mounting holes between corner holes, towards center
for y_offset in [-wall_mount_spacing_y / 2.0, wall_mount_spacing_y / 2.0]:
    mount_hole = Part.makeCylinder(wall_mount_hole_dia / 2.0, bottom_thickness + 1.0,
                                    App.Vector(case_length / 2.0,
                                              case_width / 2.0 + y_offset,
                                              -0.5),
                                    App.Vector(0, 0, 1))
    shell = shell.cut(mount_hole)
App.Console.PrintMessage("Cut 2 additional center mounting holes\n")

# ---------- Add continuous perimeter alignment flange ----------
flange_z = bottom_thickness
flange_length_x = case_length - 2 * wall_thickness
flange_length_y = case_width - 2 * wall_thickness

try:
    strip_front = Part.makeBox(flange_length_x, flange_width, flange_height)
    strip_front.translate(App.Vector(wall_thickness, wall_thickness, flange_z))

    # Back strip with notch to clear USB opening
    usb_gap_clearance = 1.0
    usb_gap_width = usb_width + 2 * usb_gap_clearance
    usb_gap_start = usb_case_x - usb_gap_width / 2.0
    usb_gap_end = usb_case_x + usb_gap_width / 2.0
    back_y = case_width - wall_thickness - flange_width

    strips_back = []
    left_len = usb_gap_start - wall_thickness
    if left_len > 0:
        left_strip = Part.makeBox(left_len, flange_width, flange_height)
        left_strip.translate(App.Vector(wall_thickness, back_y, flange_z))
        strips_back.append(left_strip)

    right_len = case_length - wall_thickness - usb_gap_end
    if right_len > 0:
        right_strip = Part.makeBox(right_len, flange_width, flange_height)
        right_strip.translate(App.Vector(usb_gap_end, back_y, flange_z))
        strips_back.append(right_strip)

    strip_left = Part.makeBox(flange_width, flange_length_y, flange_height)
    strip_left.translate(App.Vector(wall_thickness, wall_thickness, flange_z))

    strip_right = Part.makeBox(flange_width, flange_length_y, flange_height)
    strip_right.translate(App.Vector(case_length - wall_thickness - flange_width, wall_thickness, flange_z))

    shell = shell.fuse(strip_front)
    for seg in strips_back:
        shell = shell.fuse(seg)
    shell = shell.fuse(strip_left)
    shell = shell.fuse(strip_right)
    App.Console.PrintMessage("Added continuous perimeter alignment flange with USB notch\n")
    
    # Add alignment pin holes on side flanges (horizontal holes through flange thickness)
    # Left flange pin hole - drilled horizontally from outside edge through the flange
    left_pin_x = -0.1  # Start outside the left wall
    left_pin_y = case_width / 2.0
    left_pin_z = case_height + (flange_z + flange_height - case_height) * 0.5  # Between top of wall and top of flange
    left_pin = Part.makeCylinder(pin_hole_diameter / 2.0, wall_thickness + flange_width + 1.0, 
                                  App.Vector(left_pin_x, left_pin_y, left_pin_z), 
                                  App.Vector(1, 0, 0))
    shell = shell.cut(left_pin)
    App.Console.PrintMessage("Cut left flange alignment pin hole\n")
    
    # Right flange pin hole - drilled horizontally from outside edge through the flange
    right_pin_x = case_length + 0.1  # Start outside the right wall
    right_pin_y = case_width / 2.0
    right_pin_z = case_height + (flange_z + flange_height - case_height) * 0.5  # Between top of wall and top of flange
    right_pin = Part.makeCylinder(pin_hole_diameter / 2.0, wall_thickness + flange_width + 1.0, 
                                   App.Vector(right_pin_x, right_pin_y, right_pin_z), 
                                   App.Vector(-1, 0, 0))
    shell = shell.cut(right_pin)
    App.Console.PrintMessage("Cut right flange alignment pin hole\n")
except Exception as ex:
    App.Console.PrintWarning("Perimeter flange creation failed: %s\n" % ex)

# Ventilation slots - front style (horizontal through walls, 3mm wide)
# Top edge vent slots
for i in range(vent_slot_count):
    x = case_length / 2.0 + (i - (vent_slot_count - 1) / 2.0) * vent_spacing
    if x < vent_corner_margin or x > case_length - vent_corner_margin:
        continue
    slot = Part.makeBox(vent_slot_length, vent_slot_width, vent_slot_height)
    slot.translate(App.Vector(-vent_slot_length / 2.0, -vent_slot_width / 2.0, -vent_slot_height / 2.0))
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(1, 0, 0), 90)
    slot.translate(App.Vector(x, wall_thickness / 2.0, case_height / 2.0))
    try:
        shell = shell.cut(slot)
        App.Console.PrintMessage("Cut top vent slot %d\n" % i)
    except Exception as ex:
        App.Console.PrintWarning("Top vent slot %d failed: %s\n" % (i, ex))

# Bottom edge vent slots
for i in range(vent_slot_count):
    x = case_length / 2.0 + (i - (vent_slot_count - 1) / 2.0) * vent_spacing
    if x < vent_corner_margin or x > case_length - vent_corner_margin:
        continue
    slot = Part.makeBox(vent_slot_length, vent_slot_width, vent_slot_height)
    slot.translate(App.Vector(-vent_slot_length / 2.0, -vent_slot_width / 2.0, -vent_slot_height / 2.0))
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(1, 0, 0), 90)
    slot.translate(App.Vector(x, case_width - wall_thickness / 2.0, case_height / 2.0))
    try:
        shell = shell.cut(slot)
        App.Console.PrintMessage("Cut bottom vent slot %d\n" % i)
    except Exception as ex:
        App.Console.PrintWarning("Bottom vent slot %d failed: %s\n" % (i, ex))

# Left edge vent slots
for i in range(vent_slot_count_side):
    y = case_width / 2.0 + (i - (vent_slot_count_side - 1) / 2.0) * vent_spacing
    if y < vent_corner_margin or y > case_width - vent_corner_margin:
        continue
    slot = Part.makeBox(vent_slot_length, vent_slot_width, vent_slot_height)
    slot.translate(App.Vector(-vent_slot_length / 2.0, -vent_slot_width / 2.0, -vent_slot_height / 2.0))
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(1, 0, 0), 90)
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 90)
    slot.translate(App.Vector(wall_thickness / 2.0, y, case_height / 2.0))
    try:
        shell = shell.cut(slot)
        App.Console.PrintMessage("Cut left vent slot %d\n" % i)
    except Exception as ex:
        App.Console.PrintWarning("Left vent slot %d failed: %s\n" % (i, ex))

# Right edge vent slots
for i in range(vent_slot_count_side):
    y = case_width / 2.0 + (i - (vent_slot_count_side - 1) / 2.0) * vent_spacing
    if y < vent_corner_margin or y > case_width - vent_corner_margin:
        continue
    slot = Part.makeBox(vent_slot_length, vent_slot_width, vent_slot_height)
    slot.translate(App.Vector(-vent_slot_length / 2.0, -vent_slot_width / 2.0, -vent_slot_height / 2.0))
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(1, 0, 0), 90)
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 90)
    slot.translate(App.Vector(case_length - wall_thickness / 2.0, y, case_height / 2.0))
    try:
        shell = shell.cut(slot)
        App.Console.PrintMessage("Cut right vent slot %d\n" % i)
    except Exception as ex:
        App.Console.PrintWarning("Right vent slot %d failed: %s\n" % (i, ex))

# ---------- Mirror on Y-axis ----------
shell = shell.mirror(App.Vector(0, 0, 0), App.Vector(1, 0, 0))
App.Console.PrintMessage("Mirrored shell on Y-axis\n")

# ---------- Export ----------
part_obj = doc.addObject("Part::Feature", "BackCaseShellV0_6_0")
part_obj.Shape = shell

try:
    script_dir = os.path.dirname(os.path.abspath(__file__))
except NameError:
    script_dir = os.getcwd()
out_dir = os.path.join(script_dir, "freecad_outputs")
if not os.path.exists(out_dir):
    os.makedirs(out_dir)

step_path = os.path.join(out_dir, "back_case_wall_freecad_v0_6_0.step")
stl_path = os.path.join(out_dir, "back_case_wall_freecad_v0_6_0.stl")

# Export STEP
Part.export([part_obj], step_path)

# Export STL with high quality mesh for 3D printing
mesh = Mesh.Mesh()
mesh.addFacets(MeshPart.meshFromShape(
    Shape=shell,
    LinearDeflection=0.005,
    AngularDeflection=0.174533,
    Relative=False
).Facets)
mesh.write(stl_path)

App.Console.PrintMessage("Exported STEP to: %s\n" % step_path)
App.Console.PrintMessage("Exported STL to: %s\n" % stl_path)

# Save document
fcstd_path = os.path.join(out_dir, "back_case_wall_freecad_v0_6_0.FCStd")
doc.saveAs(fcstd_path)
App.Console.PrintMessage("Saved FCStd to: %s\n" % fcstd_path)
