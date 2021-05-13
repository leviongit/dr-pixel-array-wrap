$gtk.ffi_misc.gtk_dlopen("pixel_array")

module Addons
  class PXArray
    def draw_horizontal_line(x1,x2,y,color)
      FFI::CPXArray::draw_horizontal_line(@ptr,x1,x2,y,color);
    end

    def draw_vertical_line(x,y1,y2,color)
      FFI::CPXArray::draw_vertical_line(@ptr,x,y1,y2,color);
    end

    def draw_line(x1,y1,x2,y2,color)
      FFI::CPXArray::draw_line(@ptr,x1,y1,x2,y2,color);
    end
  end
end
