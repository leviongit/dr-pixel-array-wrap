$gtk.ffi_misc.gtk_dlopen("pixel_array")

module Addons
  class PXArray
    def initialize(x,y,width,height,scale,name="pixel_array")
      @ptr  = FFI::CPXArray::new_pixel_array(width, height, name)
      @x, @y          = x, y
      @name = name
      @width          = width
      @height         = height
      @scale          = scale
      @render_width   = scale * width
      @render_height  = scale * height
    end

    def pixel_array_ptr
      @ptr
    end

    def __render
      FFI::CPXArray::upload_pixel_array(@ptr)
    end

    def render_as_sprite(args, r: nil, g: nil, b: nil)
      __render
      args.outputs.sprites << {
        x: @x,
        y: @y,
        w: @render_width,
        h: @render_height,
        path: @name.to_sym,
        r: r || 255,
        g: g || 255,
        b: b || 255
      }
    end

    def render_as_primitive(args, r: nil, g: nil, b: nil)
      __render
      args.outputs.primitives << {
        x: @x,
        y: @y,
        w: @render_width,
        h: @render_height,
        path: @name.to_sym,
        r: r || 255,
        g: g || 255,
        b: b || 255
      }.sprite
    end

    def clear
      FFI::CPXArray::clear_pixel_array(@ptr)
    end

    def get_pixel(x,y)
      color = FFI::CPXArray::get_pixel(@ptr,x,y);
      [color&0xFF, (color>>8)&0xFF, (color>>16)&0xFF]
    end

    def get_pixel_bt(x,y)
      color = FFI::CPXArray::get_pixel_bt(@ptr,x,y);
      [color&0xFF, (color>>8)&0xFF, (color>>16)&0xFF]
    end

    def __set_overload(color)
      case color.length
      when 1
        color[0]
      when 3
        0xFF000000 | (color[2].to_byte << 16) | (color[1].to_byte << 8) | color[0].to_byte
      when 4
        (color[3].to_byte << 24) | (color[2].to_byte << 16) | (color[1].to_byte << 8) | color[0].to_byte
      end
    end

    def set_pixel(x,y,*color)
      FFI::CPXArray::set_pixel(@ptr, x, y, __set_overload(color))
    end

    def set_pixel_bt(x,y,*color)
      FFI::CPXArray::set_pixel_bt(@ptr, x, y, __set_overload(color))
    end

    def copy(x,y,source,source_x,source_y,source_w,source_h)
      if !$gtk.production
        puts "source: #{source.pixel_array_ptr}"
        puts "dest(self): #{@ptr}"
        puts "x:#{x} - y:#{y} - sx:#{source_x} - sy:#{source_y} - sw:#{source_w} - sh:#{source_h}"
      end
      FFI::CPXArray::copy( source.pixel_array_ptr,
      @ptr,
      source_x,
      source_y,
      source_w,
      source_h,
      x, y )
    end

    def dumpdata
      @height.times do |y|
        @width.times do |x|
          puts get_pixel(x,y)
        end
      end
    end
  end
end