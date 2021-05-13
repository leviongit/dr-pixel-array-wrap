require 'lib/pixel_array.rb'
require 'lib/bitmap_decoder.rb'
require 'lib/lines.rb'
require 'lib/polygons.rb'

################################################################################
# 1. SETUP :
################################################################################
def setup(args)
  $top_level.include Addons

  # Setup :
  width           = 64
  height          = 64
  args.state.pa1  = PXArray.new( 512, 232,       # x and y position
                                 width, height,  # width and height
                                 4 )             # scale

  args.state.pa2  = PXArray.new( 512, 230,
                                 width, height,
                                 4, "pxarr2")

  # Set some pixels :
  (height >> 1).times do |y|
    width.times do |x|
      args.state.pa1.set_pixel x, y, 0xAAFF0055
    end
  end

  # Drawn some lines :
  args.state.pa1.draw_horizontal_line 1, 62, 31, 0xFF0000FF
  args.state.pa1.draw_vertical_line 31, 1, 62, 0xFF00FF00
  args.state.pa1.draw_line 1, 1, 62, 62, 0xFFFF0000

  # Draw some polygons :
  args.state.pa1.stroke_rectangle 2, 3, 10, 20, 0xFF0000FF
  args.state.pa1.fill_rectangle 4, 5, 7, 17, 0xFF00FF00

  # Copy some pixels :
  args.state.pa2.copy 0, 0,
                    args.state.pa1,
                    0, 0, 30, 16
  args.state.pa2.copy 33, 0,
                    args.state.pa1,
                    0, 16, 31, 16

  args.state.setup_done = true
end



$gtk.reset

################################################################################
# 2. MAIN LOOP :
################################################################################
def tick(args)
  setup(args) unless args.state.setup_done

  # !!! DON'T FORGET TO RENDER THE PIXEL ARRAY !!!
  args.state.pa1.render_as_sprite(args)
  args.state.pa2.render_as_sprite(args)
  # !!! DON'T FORGET TO RENDER THE PIXEL ARRAY !!!

  args.outputs.primitives << $gtk.framerate_diagnostics_primitives

end
