require 'RMagick'
include Magick

image = Image::read(ARGV[0]).first
image_name = File.basename(ARGV[0], File.extname(ARGV[0]))
puts "PROGMEM const unsigned char #{image_name}[] = {"

(0...image.rows).each do |y|
  print '  B'
  (0...image.columns).each do |x|
    pixel = image.pixel_color(x, y)
    print pixel.red == 0 ? '0' : '1'
    print ', B' if (x + 1) % 8 == 0
  end
  print '0' * (8 - (image.columns % 8))
  puts ','
end

puts '};'
