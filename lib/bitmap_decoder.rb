# $gtk.ffi_misc.gtk_dlopen("pixel_array")

module Addons
  class Bitmap
    def self.encode(pixel_array)
      return FFI::CPXArray::encode_pixel_array(pixel_array)
    end

    def self.decode!(uint, scale: 1, name: "decoded", x: 0, y: 0)
      pixel_array = Addons::PXArray.new(x, y, 8, 8, scale, name)
      FFI::CPXArray::decode_uint64(uint, pixel_array.pixel_array_ptr)
    end
  end
end