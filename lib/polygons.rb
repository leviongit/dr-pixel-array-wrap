$gtk.ffi_misc.gtk_dlopen("pixel_array")

module Addons
  class PXArray
    def stroke_rectangle(x,y,width,height,color);
      FFI::CPXArray::stroke_rectangle(@ptr,x,y,width,height,color);
    end

    def fill_rectangle(x,y,width,height,color);
      FFI::CPXArray::fill_rectangle(@ptr,x,y,width,height,color);
    end
  end
end
