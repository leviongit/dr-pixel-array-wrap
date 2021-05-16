# A simple pixel array wrapper for DragonRuby.

## Setup:

```ruby
args.state.pxa        = PXArray.new 512, 232, # x and y position
                                    64, 64,   # width and height
                                    4         # scale
```

## Access pixels:

```ruby
# Set a pixel :
args.state.pxa.set_pixel 32, 32, 0xFF0000FF

# Get a pixel :
pv = args.state.pxa.get_pixel 32, 32
```

## Draw lines:

```ruby
# Horizontal line :
args.state.pxa.draw_horizontal_line 1, 62, 31, 0xFF0000FF

# Vertical line :
args.state.pxa.draw_vertical_line 1, 62, 31, 0xFF00FF00

# Arbitrary line :
args.state.pxa.draw_line 1, 1, 62, 62, 0xFFFF0000
```

## Draw polygons:

```ruby
# Stroke rectangle :
args.state.pxa.stroke_rectangle 2, 3, 10, 20, 0xFF0000FF

# Fill rectangle :
args.state.pxa.fill_rectangle 4, 5, 7, 17, 0xFF00FF00

# Stroke arbitrary (up to 64 vertices) CONVEX polygon :
args.state.pa1.stroke_polygon [[4,5], [2,15], [17,18], [19,3]], 0xFFFFFF00

# Fill arbitrary (up to 64 vertices) CONVEX polygon :
args.state.pa1.fill_polygon [[4,5], [2,15], [17,18], [19,3]], 0xFFFF0000
```
