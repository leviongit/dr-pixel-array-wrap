require 'lib/pixel_array.rb'
require 'lib/bitmap_decoder.rb'

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

  # Copy some pixels :
  # args.state.pa2.copy 0, 0,
  #                   args.state.pa1,
  #                   0, 0, 30, 16
  # args.state.pa2.copy 33, 0,
  #                   args.state.pa1,
  #                   0, 16, 31, 16

  # ! this is bugged, things above a certain value get mashed by ruby, I need to find a way of unmashing them
  # ! I need to find help from alex, this is definitely some ruby dark magic
  args.state.pxarray = Addons::Bitmap.decode!(0b1111111111111111111111111111111111111111111111111111111111111111, scale: 16, x: 200, y: 200)
  # puts(Addons::Bitmap.encode(args.state.pxarray.pixel_array_ptr).to_i)
  # args.state.pxarray = Addons::PXArray.new(0,0,8,8,16)


  args.state.setup_done = true
end



$gtk.reset

################################################################################
# 2. MAIN LOOP :
################################################################################
def tick(args)
  setup(args) unless args.state.setup_done

  # !!! DON'T FORGET TO RENDER THE PIXEL ARRAY !!!
  args.state.pxarray.render_as_sprite(args, r: 255, g: 80, b: 80)
  # args.state.pa1.render_as_sprite(args)
  # args.state.pa2.render_as_sprite(args)
  # !!! DON'T FORGET TO RENDER THE PIXEL ARRAY !!!

  args.outputs.primitives << $gtk.framerate_diagnostics_primitives

end
