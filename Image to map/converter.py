from PIL import Image

def image_to_tilemap(image_path):
    # Load the image
    img = Image.open(image_path)

    # Ensure the image is 32x16 pixels
    if img.size != (32, 16):
        raise ValueError("Image size must be 32x16 pixels.")
    
    # Convert image to RGB
    img = img.convert("RGB")

    # Map to store unique colors and their corresponding tile numbers
    color_map = {'#F00000': 1, '#FF0000': 2, '#FFF000': 3, '#FFFF00': 4, '#FFFFF0': 5,'#0FFFFF': 6,'#A00000': 7, '#AA0000': 8, '#AAA000': 9, '#AAAA00': 10,'#0000BB': 11,'#BB0000': 12,'#00BB00': 13, '#FFFFFF': 14,'#AAAAAA':15,'#AAAAA0':16,'#CCCCCC':17
                 }
    tile_number = 18 # Start tile number for unmapped colors

    # Generate the tilemap
    tilemap = []
    for y in range(img.height):
        row = []
        for x in range(img.width):
            # Get the RGB color of the pixel
            pixel = img.getpixel((x, y))
            # Convert the color to a hex code
            hex_color = f"#{pixel[0]:02X}{pixel[1]:02X}{pixel[2]:02X}"
            # Assign a unique number to the color if not already mapped
            if hex_color not in color_map:
                color_map[hex_color] = tile_number
                tile_number += 1
            # Append the corresponding number to the row
            row.append(color_map[hex_color])
        tilemap.append(row)

    # Transpose the tilemap (swap rows and columns)
    tilemap = [list(row) for row in zip(*tilemap)]

    return tilemap, color_map


def save_tilemap_to_file(tilemap, color_map, output_path):
    with open(output_path, "w") as file:
        # Write the tilemap
        file.write("int tilemap[32][16] = {\n")  # Adjusted dimensions to match transposed data
        for row in tilemap:
            file.write("    {" + ", ".join(map(str, row)) + "},\n")
        file.write("};\n\n")

        # Write the color map
        file.write("// Color map (tile number -> hex color)\n")
        for hex_color, tile_number in color_map.items():
            file.write(f"// {tile_number}: {hex_color}\n")


# Example usage
image_path = "C:/Users/gouja/Documents/game/Image to map/level9.png"  # Path to your 32x16 pixel image in the same folder as the script
output_path = "C:/Users/gouja/Documents/game/Image to map/main.h"  # Path to save the tilemap in the same folder

try:
    tilemap, color_map = image_to_tilemap(image_path)
    save_tilemap_to_file(tilemap, color_map, output_path)
    print(f"Tilemap successfully saved to {output_path}")
except Exception as e:
    print(f"Error: {e}")
