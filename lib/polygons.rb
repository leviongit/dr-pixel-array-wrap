$gtk.ffi_misc.gtk_dlopen("pixel_array")

module Addons
  class PXArray
    def stroke_rectangle(x,y,width,height,color);
      FFI::CPXArray::stroke_rectangle(@ptr,x,y,width,height,color);
    end

    def fill_rectangle(x,y,width,height,color);
      FFI::CPXArray::fill_rectangle(@ptr,x,y,width,height,color);
    end

    def stroke_polygon(vertices,color)
      vertices.each_cons(2) { |pair| FFI::CPXArray.draw_line(*(pair.flatten)) }
      FFI::CPXArray.draw_line(*(vertices.last + vertices.first).flatten,color)
    end

    def get_polygon_vertices_array
      puts 'in get polygon vertices array'
      get_polygon_vertices_array(@ptr)
    end

    def set_polygon_vertex_count(count)
      set_polygon_vertex_count(@ptr, count)
    end

    def fill_polygon(vertices,color)
      puts @vertices
      FFI::CPXArray.set_polygon_vertex_count(@ptr, vertices.length)
      vertices.length.times do |i|
        @vertices[2*i]    = vertices[i][0]
        @vertices[2*i+1]  = vertices[i][1]
      end
      FFI::CPXArray.fill_polygon(@ptr,color)
    end
  end
end
