# FreeCAD script to build front case shell with edge-only fillet at face/wall junction
# Run with: freecadcmd case/freecad_front_case_v0_6_0.py

import FreeCAD as App
import Part
import Mesh
import MeshPart
import os

# ---------- Parameters (Thermostat V0.6.0) ----------
# Edge cuts bbox: min (110.682,65.910) span (101.488 x 83.440)
pcb_length = 101.488
pcb_width = 83.44
pcb_thickness = 1.6
pcb_clearance = 3.0

# PCB mounting holes (relative to lower-left board origin)
# Use ONLY H1..H4 MountingHole footprints (true PCB mounts)
pcb_mount_holes = [
    [3.068, 3.040],    # H1
    [98.438, 3.040],   # H2
    [70.358, 68.790],  # H3
    [3.068, 80.390]    # H4
]

# Case parameters
face_thickness = 1.8
wall_thickness = 9.0
standoff_height = 17.3
standoff_diameter = 7.0
hole_diameter = 2.5

display_clearance = 18.9
# Inner cavity dimensions (what PCB needs)
inner_length = pcb_length + (2 * pcb_clearance)
inner_width = pcb_width + (2 * pcb_clearance)
# Outer case dimensions (cavity + walls)
case_length = inner_length + (2 * wall_thickness)
case_width = inner_width + (2 * wall_thickness)
case_height = face_thickness + display_clearance

# Display opening — center aligned to display mount holes H5–H8
# H5..H8 relative to Edge.Cuts min (from KiCad):
# H5 (6.608,14.230), H6 (89.718,14.230), H7 (89.718,63.130), H8 (6.608,63.130)
# Center of the rectangle = ((6.608+89.718)/2, (14.230+63.130)/2) = (48.163, 38.680)
display_center_rel_x = 48.163
display_center_rel_y = 38.680

# Keep opening size for now; we can refine once module window is confirmed
display_width = 68.0
display_height = 50.0

# Convert center from board-relative to case coordinates and offset left by 3.46 mm
display_center_x = wall_thickness + pcb_clearance + display_center_rel_x - 3.46
display_center_y = wall_thickness + pcb_clearance + display_center_rel_y
display_x = display_center_x - display_width / 2.0
display_y = display_center_y - display_height / 2.0

# Sensor holes (positions from Thermostat.kicad_pcb V0.6.0)
ldr_diameter = 5.5
# LDR center abs: (113.44, 137.01) → rel to bbox min: (2.758, 71.100)
ldr_x = wall_thickness + pcb_clearance + 2.758
ldr_y = wall_thickness + pcb_clearance + 71.100

sensor_box_width = 20.0
sensor_box_height = 15.0
sensor_box_depth = 18.0
sensor_box_wall = 1.5
# AHT20 abs: (208.75, 145.78) → rel to bbox min: (98.068, 79.870)
sensor_x = wall_thickness + pcb_clearance + 98.068 + 0.5
sensor_y = wall_thickness + pcb_clearance + 79.870

# Button holes (positions from Thermostat.kicad_pcb V0.6.0)
# SW1 (Reset) abs: (160.528, 143.002) → rel to bbox min: (49.846, 77.092)
# SW2 (Boot) abs: (208.788, 127.508) → rel to bbox min: (98.106, 61.598)
button_diameter = 2.0
reset_x = wall_thickness + pcb_clearance + 49.846
reset_y = wall_thickness + pcb_clearance + 77.092
boot_x = wall_thickness + pcb_clearance + 98.106
boot_y = wall_thickness + pcb_clearance + 61.598

corner_r = 4.0
face_edge_radius = 4.0  # Outer fillet radius at face/wall junction
inner_face_edge_radius = 0.5  # Smaller radius for inner reinforcement fillet (to avoid BRep failures)

# Alignment pin holes for case assembly
pin_hole_diameter = 1.0

# USB port hole (bottom wall for cable access)
# J1 (USB-C connector) abs: (169.97, 144.925) → rel to bbox min: (59.288, 79.015)
usb_rel_x = 59.288
usb_rel_y = 79.015
usb_case_x = wall_thickness + pcb_clearance + usb_rel_x
usb_case_y = wall_thickness + pcb_clearance + usb_rel_y
usb_width = 13.0  # USB-C connector is ~8.5mm wide; add clearance
usb_height = 7.0  # USB-C connector is ~6.5mm tall; add clearance

# ---------- Helpers ----------
TOL = 1e-6

def near(a, b, tol=TOL):
    return abs(a - b) <= tol

# ---------- Build outer and inner boxes ----------
doc = App.newDocument("FrontCaseV0_6_0")

# Outer solid (origin at lower-left-back)
outer = Part.makeBox(case_length, case_width, case_height)

# Inner box to create wall cavity (preserves straight walls and face thickness)
inner = Part.makeBox(inner_length, inner_width, case_height - face_thickness)
inner.Placement.Base = App.Vector(wall_thickness, wall_thickness, face_thickness)

shell = outer.cut(inner)

# Fillet the 4 vertical corner edges to match rounded corners
try:
    corner_edges = []
    for e in shell.Edges:
        v1 = e.Vertexes[0].Point
        v2 = e.Vertexes[1].Point
        # Vertical edge: x and y same at both vertices, z different
        if near(v1.x, v2.x) and near(v1.y, v2.y) and not near(v1.z, v2.z):
            # At the corners: x is 0 or case_length; y is 0 or case_width
            if (near(v1.x, 0) or near(v1.x, case_length)) and (near(v1.y, 0) or near(v1.y, case_width)):
                corner_edges.append(e)
    if corner_edges:
        shell = shell.makeFillet(corner_r, corner_edges)
except Exception as ex:
    App.Console.PrintWarning("Corner fillet failed (non-critical): %s\n" % ex)

# ---------- Cutouts (subtract from shell) ----------
# LDR07 hole through front face
ldr_hole = Part.makeCylinder(ldr_diameter / 2.0, face_thickness + 2, 
                             App.Vector(ldr_x, ldr_y, -1), App.Vector(0, 0, 1))
shell = shell.cut(ldr_hole)

# Reset and Boot button holes through front face
reset_hole = Part.makeCylinder(button_diameter / 2.0, face_thickness + 0.2,
                               App.Vector(reset_x, reset_y, -0.1), App.Vector(0, 0, 1))
boot_hole = Part.makeCylinder(button_diameter / 2.0, face_thickness + 0.2,
                              App.Vector(boot_x, boot_y, -0.1), App.Vector(0, 0, 1))
shell = shell.cut(reset_hole)
shell = shell.cut(boot_hole)
App.Console.PrintMessage("Added Reset and Boot button holes\n")

# LDR tube/collar - 10mm tall, 6mm inner diameter
ldr_tube_height = 10.0
ldr_tube_inner_dia = 6.0
ldr_tube_outer_dia = 8.0  # 1mm wall thickness
ldr_tube_outer = Part.makeCylinder(ldr_tube_outer_dia / 2.0, ldr_tube_height,
                                    App.Vector(ldr_x, ldr_y, 0), App.Vector(0, 0, 1))
ldr_tube_inner = Part.makeCylinder(ldr_tube_inner_dia / 2.0, ldr_tube_height + 0.2,
                                    App.Vector(ldr_x, ldr_y, -0.1), App.Vector(0, 0, 1))
ldr_tube = ldr_tube_outer.cut(ldr_tube_inner)
try:
    shell = shell.fuse(ldr_tube)
    App.Console.PrintMessage("Added LDR light tube\n")
except Exception as ex:
    App.Console.PrintWarning("LDR tube fuse failed: %s\n" % ex)

# Temp/Humidity sensor front opening (AHT20 footprint 12.5 x 6.0, rotated 90°)
# Match SCAD: center at origin, rotate, then translate to sensor_x, sensor_y
sensor_opening_shape = Part.makeBox(12.5, 6.0, face_thickness + 0.2)
sensor_opening_shape.translate(App.Vector(-12.5/2.0, -6.0/2.0, -0.1))
sensor_opening_shape = sensor_opening_shape.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 90)
sensor_opening_shape.translate(App.Vector(sensor_x, sensor_y, 0))
shell = shell.cut(sensor_opening_shape)

# Add protective lattice mesh across sensor opening - thin ribs that can be cut out if needed
mesh_thickness = 0.4  # Thin mesh ribs
mesh_depth = face_thickness / 3.0  # 1/3 face thickness
mesh_spacing = 2.5  # Spacing between diagonal bars
lattice_bars = []

# Create diagonal bars in one direction (/)
for offset in range(-3, 4):  # Multiple diagonal bars
    bar = Part.makeBox(mesh_thickness, 15.0, mesh_depth)  # Long enough to cover opening
    bar.translate(App.Vector(offset * mesh_spacing - mesh_thickness/2, -7.5, 0))
    bar = bar.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 45)
    lattice_bars.append(bar)

# Create diagonal bars in other direction (\)
for offset in range(-3, 4):  # Multiple diagonal bars
    bar = Part.makeBox(mesh_thickness, 15.0, mesh_depth)
    bar.translate(App.Vector(offset * mesh_spacing - mesh_thickness/2, -7.5, 0))
    bar = bar.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), -45)
    lattice_bars.append(bar)

# Combine all bars
lattice_mesh = lattice_bars[0]
for bar in lattice_bars[1:]:
    try:
        lattice_mesh = lattice_mesh.fuse(bar)
    except:
        pass

# Rotate 90° and translate to sensor position (same as opening)
lattice_mesh = lattice_mesh.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 90)
lattice_mesh.translate(App.Vector(sensor_x, sensor_y, 0))
try:
    shell = shell.fuse(lattice_mesh)
    App.Console.PrintMessage("Added lattice mesh to sensor opening\n")
except Exception as ex:
    App.Console.PrintWarning("Sensor mesh fuse failed: %s\n" % ex)

# Sensor protection box: 4-wall frame protruding from face, with vents (rotated 90° to match opening)
# Match SCAD: build centered at origin, rotate, then translate to sensor position
# Outer frame dimensions - centered at origin
sensor_box_outer = Part.makeBox(sensor_box_width, sensor_box_height, sensor_box_depth)
sensor_box_outer.translate(App.Vector(-sensor_box_width/2, -sensor_box_height/2, 0))

# Inner cavity (hollow interior through full depth)
sensor_box_inner = Part.makeBox(sensor_box_width - 2 * sensor_box_wall,
                                 sensor_box_height - 2 * sensor_box_wall,
                                 sensor_box_depth + 0.2)
sensor_box_inner.translate(App.Vector(-(sensor_box_width - 2*sensor_box_wall)/2,
                                      -(sensor_box_height - 2*sensor_box_wall)/2,
                                      -0.1))

sensor_box = sensor_box_outer.cut(sensor_box_inner)

# Match SCAD: already centered at origin from construction, rotate then translate
sensor_box = sensor_box.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 90)
sensor_box.translate(App.Vector(sensor_x, sensor_y, 0))

# Add 2mm notch to sensor box BEFORE fusing to shell
# Based on position that was working: back of box at (sensor_y - sensor_box_width/2)
# Just make a small notch instead of cutting the whole wall
notch = Part.makeBox(2.2, 2.5, 3.8)
notch.translate(App.Vector(sensor_x + 3.8, sensor_y - sensor_box_width/2 - 0.1, case_height - face_thickness - 4.3))
sensor_box = sensor_box.cut(notch)
App.Console.PrintMessage("Added notch at sensor box back rim\n")

# Add same notch to the other wall (front side)
# Wall is 22.2mm from the outside wall on X-axis (left side) - testing with 4x4x4 solid block
notch2 = Part.makeBox(2.2, 2.5, 3.8)
notch2.translate(App.Vector(case_length - 22.5, sensor_y - sensor_box_width/2 - 0.1 + 14.5, case_height - face_thickness - 4.3))
sensor_box = sensor_box.cut(notch2)
App.Console.PrintMessage("Added notch at sensor box front rim\n")

# Add sensor box to shell
try:
    shell = shell.fuse(sensor_box)
except Exception as ex:
    App.Console.PrintWarning("Sensor box fuse failed: %s\n" % ex)

# Display opening
display_opening = Part.makeBox(display_width, display_height, face_thickness + 0.2)
display_opening.Placement.Base = App.Vector(display_x, display_y, -0.1)
shell = shell.cut(display_opening)

# Fillet the display opening edges (outer edge at z=0) - important for touch sensitivity
# Try filleting edges individually since batch filleting fails on complex geometry
display_fillet_radius = 1.0  # Small radius for display opening
display_edges = []

for e in shell.Edges:
    if len(e.Vertexes) >= 2:
        v1 = e.Vertexes[0].Point
        v2 = e.Vertexes[1].Point
        # Edges at z=0 within the display opening rectangle
        if near(v1.z, 0) and near(v2.z, 0):
            # Check if edge is within display opening bounds
            in_display_x = (display_x <= v1.x <= display_x + display_width and 
                           display_x <= v2.x <= display_x + display_width)
            in_display_y = (display_y <= v1.y <= display_y + display_height and 
                           display_y <= v2.y <= display_y + display_height)
            
            if (in_display_x or in_display_y) and not (v1.x == v2.x and v1.y == v2.y):
                display_edges.append(e)

if display_edges:
    successes = 0
    for edge in display_edges:
        try:
            shell = shell.makeFillet(display_fillet_radius, [edge])
            successes += 1
        except:
            pass  # Some edges can't be filleted due to geometry - skip silently
    if successes > 0:
        App.Console.PrintMessage("Display opening fillet: %d/%d edges filleted\n" % (successes, len(display_edges)))

# Top edge vent slots - 20mm long horizontal slots through wall (rotate 90° around X)
vent_slot_count = 15
vent_slot_length = 8.0
vent_slot_width = 2.0
vent_slot_height = 9.0  # Reduced depth to avoid cutting into sensor box
vent_spacing = 10.5  # Match back case spacing for alignment
vent_corner_margin = 15.0  # Skip vents near corners for strength
for i in range(vent_slot_count):
    x = case_length / 2.0 + (i - (vent_slot_count - 1) / 2.0) * vent_spacing
    if x < vent_corner_margin or x > case_length - vent_corner_margin:
        continue
    # Create vertical slot then rotate to be horizontal (through Y wall)
    slot = Part.makeBox(vent_slot_length, vent_slot_width, vent_slot_height)
    slot.translate(App.Vector(-vent_slot_length / 2.0, -vent_slot_width / 2.0, -vent_slot_height / 2.0))
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(1, 0, 0), 90)  # Rotate around X-axis
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

# Left edge vent slots - rotate 90° around X then 90° around Z
vent_slot_count_side = 10
for i in range(vent_slot_count_side):
    y = case_width / 2.0 + (i - (vent_slot_count_side - 1) / 2.0) * vent_spacing
    if y < vent_corner_margin or y > case_width - vent_corner_margin:
        continue
    slot = Part.makeBox(vent_slot_length, vent_slot_width, vent_slot_height)
    slot.translate(App.Vector(-vent_slot_length / 2.0, -vent_slot_width / 2.0, -vent_slot_height / 2.0))
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(1, 0, 0), 90)  # First rotate around X
    slot = slot.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 90)  # Then rotate around Z
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

# ---------- USB port hole (top wall) ----------
# Cut a rectangular opening in the top wall for USB-C connector access
# Position at top of wall for cable clearance
usb_hole = Part.makeBox(usb_width, wall_thickness + 0.2, usb_height)
usb_hole.translate(App.Vector(usb_case_x - usb_width / 2.0, case_width - wall_thickness - 0.1, case_height - usb_height))
shell = shell.cut(usb_hole)
App.Console.PrintMessage("Cut USB port hole on top wall (at top of wall)\n")

# ---------- Add standoffs ----------
# Identify the standoff closest to the display opening center
standoff_positions = []
for hole in pcb_mount_holes:
    standoff_positions.append(
        (
            wall_thickness + hole[0] + pcb_clearance,
            wall_thickness + hole[1] + pcb_clearance,
        )
    )

# Compute nearest standoff index to display center
nearest_idx = 0
min_dist2 = 1e9
for idx, (sx, sy) in enumerate(standoff_positions):
    dx = (display_center_x - sx)
    dy = (display_center_y - sy)
    d2 = dx * dx + dy * dy
    if d2 < min_dist2:
        min_dist2 = d2
        nearest_idx = idx

# Create standoffs and trim the one facing the display to avoid collision
for idx, hole in enumerate(pcb_mount_holes):
    x = wall_thickness + hole[0] + pcb_clearance
    y = wall_thickness + hole[1] + pcb_clearance
    # Standoff cylinder (hollow with hole)
    standoff_outer = Part.makeCylinder(
        standoff_diameter / 2.0, standoff_height,
        App.Vector(x, y, face_thickness),
        App.Vector(0, 0, 1)
    )
    standoff_hole = Part.makeCylinder(
        hole_diameter / 2.0, standoff_height + 0.2,
        App.Vector(x, y, face_thickness - 0.1),
        App.Vector(0, 0, 1)
    )
    standoff = standoff_outer.cut(standoff_hole)

    # If this is the standoff nearest to the display, trim the side facing the display opening
    if idx == nearest_idx:
        try:
            # Determine primary direction towards display (X or Y)
            dx = display_center_x - x
            dy = display_center_y - y

            trim_flat_amount = 1.2  # mm to flatten off the facing side
            trim_len = standoff_diameter  # generous box length to fully clip the side
            trim_wid = standoff_diameter
            trim_hgt = standoff_height + 0.3

            # Position the trim box so it removes the side facing the display
            if abs(dx) >= abs(dy):
                # Trim in X direction
                if dx > 0:
                    bx = x + standoff_diameter / 2.0 - trim_flat_amount
                else:
                    bx = x - standoff_diameter / 2.0 - (trim_len - trim_flat_amount)
                by = y - standoff_diameter / 2.0
            else:
                # Trim in Y direction
                if dy > 0:
                    by = y + standoff_diameter / 2.0 - trim_flat_amount
                else:
                    by = y - standoff_diameter / 2.0 - (trim_wid - trim_flat_amount)
                bx = x - standoff_diameter / 2.0

            bz = face_thickness - 0.1
            trim_box = Part.makeBox(trim_len, trim_wid, trim_hgt)
            trim_box.translate(App.Vector(bx, by, bz))
            standoff = standoff.cut(trim_box)
            App.Console.PrintMessage("Trimmed standoff near display to prevent collision\n")
        except Exception as ex:
            App.Console.PrintWarning("Standoff trim failed (non-critical): %s\n" % ex)

    try:
        shell = shell.fuse(standoff)
    except Exception as ex:
        App.Console.PrintWarning("Standoff fuse failed: %s\n" % ex)


# Add alignment pin holes on side walls (horizontal holes through wall thickness) BEFORE rotation
# Left side wall pin hole - drilled horizontally from outside edge
left_pin_x = -0.1  # Start outside the left wall
left_pin_y = case_width / 2.0
left_pin_z = case_height - 3.0  # Place TOP of 1mm hole ~2.5mm below wall top
left_pin = Part.makeCylinder(pin_hole_diameter / 2.0, wall_thickness + 5.0, 
                              App.Vector(left_pin_x, left_pin_y, left_pin_z), 
                              App.Vector(1, 0, 0))
shell = shell.cut(left_pin)
App.Console.PrintMessage("Cut left side alignment pin hole\n")

# Right side wall pin hole - drilled horizontally from outside edge
right_pin_x = case_length + 0.1  # Start outside the right wall
right_pin_y = case_width / 2.0
right_pin_z = case_height - 3.0  # Place TOP of 1mm hole ~2.5mm below wall top
right_pin = Part.makeCylinder(pin_hole_diameter / 2.0, wall_thickness + 5.0, 
                               App.Vector(right_pin_x, right_pin_y, right_pin_z), 
                               App.Vector(-1, 0, 0))
shell = shell.cut(right_pin)
App.Console.PrintMessage("Cut right side alignment pin hole\n")

# ---------- Rotate entire shell 180° around Z-axis ----------
# Rotate 180° around vertical Z-axis to flip face front-to-back
# This keeps all features properly oriented while reversing which face is "front"
shell = shell.rotate(App.Vector(0, 0, 0), App.Vector(0, 0, 1), 180)
App.Console.PrintMessage("Rotated shell 180° around Z-axis\n")

# ---------- Fillet the perimeter edges (AFTER mirror) ----------

# First: Inner edge reinforcement at z=face_thickness (inside where face meets cavity)
try:
    inner_edges = []
    App.Console.PrintMessage("Searching for inner reinforcement edges at z=%.2f...\n" % face_thickness)
    
    # Tolerance for inner edge detection - increased to catch edges near but not exactly on perimeter
    edge_tolerance = 1.0  # 1mm tolerance
    
    for e in shell.Edges:
        if len(e.Vertexes) >= 2:
            v1 = e.Vertexes[0].Point
            v2 = e.Vertexes[1].Point
            if near(v1.z, face_thickness) and near(v2.z, face_thickness):
                # Skip edges near display opening (they're broken/complex and can't be filleted)
                # Display is at x: display_x to display_x+display_width, y: display_y to display_y+display_height
                # After mirror, display_x becomes negative
                display_margin = 5.0  # mm margin around display to exclude
                v1_near_display = (-display_x - display_width - display_margin < v1.x < -display_x + display_margin and
                                  display_y - display_margin < v1.y < display_y + display_height + display_margin)
                v2_near_display = (-display_x - display_width - display_margin < v2.x < -display_x + display_margin and
                                  display_y - display_margin < v2.y < display_y + display_height + display_margin)
                
                if v1_near_display or v2_near_display:
                    continue  # Skip edges near display
                
                # Inner perimeter (cavity edge) - after mirror, around x = -wall_thickness or -(case_length - wall_thickness)
                # Use broader tolerance to catch edges broken by display opening
                v1_inner = (abs(v1.x + wall_thickness) < edge_tolerance or abs(v1.x + (case_length - wall_thickness)) < edge_tolerance or 
                           abs(v1.y - wall_thickness) < edge_tolerance or abs(v1.y - (case_width - wall_thickness)) < edge_tolerance)
                v2_inner = (abs(v2.x + wall_thickness) < edge_tolerance or abs(v2.x + (case_length - wall_thickness)) < edge_tolerance or 
                           abs(v2.y - wall_thickness) < edge_tolerance or abs(v2.y - (case_width - wall_thickness)) < edge_tolerance)
                
                if v1_inner and v2_inner:
                    inner_edges.append(e)
    
    App.Console.PrintMessage("Found %d inner edges to fillet with radius %.2f mm\n" % (len(inner_edges), inner_face_edge_radius))
    if inner_edges:
        fallback_radius = max(0.3, min(inner_face_edge_radius * 0.5, 0.8))  # Smaller backup radius for stability
        successes = 0
        failures = 0
        for idx, edge in enumerate(inner_edges):
            try:
                shell = shell.makeFillet(inner_face_edge_radius, [edge])
                successes += 1
            except Exception as ex:
                try:
                    shell = shell.makeFillet(fallback_radius, [edge])
                    successes += 1
                except Exception as ex2:
                    failures += 1
                    # Silently skip - these are edges broken by display opening that can't be filleted
        if successes > 0:
            App.Console.PrintMessage("Inner fillet per-edge applied: %d success, %d skipped\n" % (successes, failures))
        else:
            App.Console.PrintMessage("Inner fillet: all edges skipped (likely broken by cutouts)\n")
except Exception as ex:
    App.Console.PrintWarning("Inner edge selection failed: %s\n" % ex)

# Second: Outer edge rounding at z=0 (outside where face meets outer walls)
try:
    outer_edges = []
    App.Console.PrintMessage("Searching for outer fillet edges at z=0 (face bottom, outer perimeter)...\n")
    
    for e in shell.Edges:
        if len(e.Vertexes) >= 2:
            v1 = e.Vertexes[0].Point
            v2 = e.Vertexes[1].Point
            # Both vertices at z=0 (bottom of face, top of outer wall)
            if near(v1.z, 0) and near(v2.z, 0):
                # Outermost perimeter - after mirror: x from -case_length to 0, y from 0 to case_width
                v1_outer = (near(v1.x, 0) or near(v1.x, -case_length) or 
                           near(v1.y, 0) or near(v1.y, case_width))
                v2_outer = (near(v2.x, 0) or near(v2.x, -case_length) or 
                           near(v2.y, 0) or near(v2.y, case_width))
                
                if v1_outer and v2_outer:
                    outer_edges.append(e)
                    App.Console.PrintMessage("  Outer edge: (%.2f,%.2f,%.2f) to (%.2f,%.2f,%.2f)\n" % 
                                           (v1.x, v1.y, v1.z, v2.x, v2.y, v2.z))
    
    App.Console.PrintMessage("Found %d outer edges to fillet with radius %.2f mm\n" % (len(outer_edges), face_edge_radius))
    if outer_edges:
        try:
            shell = shell.makeFillet(face_edge_radius, outer_edges)
            App.Console.PrintMessage("Outer fillet applied successfully\n")
        except Exception as ex:
            App.Console.PrintWarning("Outer fillet failed: %s\n" % ex)
    else:
        App.Console.PrintWarning("No outer edges found for fillet\n")
except Exception as ex:
    App.Console.PrintWarning("Outer edge selection failed: %s\n" % ex)

# ---------- Export ----------
# Create a Part feature to visualize in FreeCAD GUI if desired
part_obj = doc.addObject("Part::Feature", "FrontCaseShellV0_6_0")
part_obj.Shape = shell

# Determine output directory
try:
    script_dir = os.path.dirname(os.path.abspath(__file__))
except NameError:
    script_dir = os.getcwd()
out_dir = os.path.join(script_dir, "freecad_outputs")
if not os.path.exists(out_dir):
    os.makedirs(out_dir)

step_path = os.path.join(out_dir, "front_case_display_freecad_v0_6_0.step")
stl_path = os.path.join(out_dir, "front_case_display_freecad_v0_6_0.stl")

# Export STEP
Part.export([part_obj], step_path)

# Export STL with highest quality mesh for 3D printing
# Use same method as FreeCAD GUI export with premium settings
mesh = Mesh.Mesh()
mesh.addFacets(MeshPart.meshFromShape(
    Shape=shell,
    LinearDeflection=0.005,      # 5 microns - very fine detail
    AngularDeflection=0.174533,  # 10 degrees in radians - smooth curves
    Relative=False
).Facets)
mesh.write(stl_path)

App.Console.PrintMessage("Exported STEP to: %s\n" % step_path)
App.Console.PrintMessage("Exported STL to: %s\n" % stl_path)

# Save doc (optional)
doc_file = os.path.join(out_dir, "front_case_display_freecad_v0_6_0.FCStd")
doc.saveAs(doc_file)
App.Console.PrintMessage("Saved FCStd to: %s\n" % doc_file)
