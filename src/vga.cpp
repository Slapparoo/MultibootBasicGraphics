/*

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
	
*/	

/*

Basic graphics utility methods

*/

#include <vga.h>

/*

As the is no memory management the Offscreen buffer is allocated elsewhere and passed in.

This class could be re-worked as a Canvas with out to many changes

*/            
VideoGraphicsArray::VideoGraphicsArray(const multiboot_header * boot_header, u32 * _buffer) {
    width = boot_header->framebuffer_width;
    height = boot_header->framebuffer_height;
    screen = (u32*)boot_header->framebuffer_addr;
    
    buffer = _buffer;

    // initialiase to 0    
    for (u32 i = 0; i < (width * (height)); i++) {
        buffer[i] = (u32)0;
    }
}


void VideoGraphicsArray::PutPixel(i32 x, i32 y, u32 color) {
    if (x < 0 || width <= x || y < 0 || height <= y)
        return;
        
    buffer[ width * y + x] = color;
}

void VideoGraphicsArray::FillRectangle(i32 x, i32 y, u32 w, u32 h, u32 color) {
    u32 i = width* (y - 1);
    
    // test if the Rectangle will be clipped (will it be fully in the screen or partially)
    if (x >= 0 && x+w < width && y >= 0 && y+h < height) {
        // fully drawn
        i += x + w;
        for(i32 yy = h; yy > 0; yy--) {
            i += width - w;
            for(i32 xx = w; xx > 0; xx--) {
               buffer[i++] = color;
            }
        }
    } else {
        // clipped
        for(i32 yy = y; yy < y+h; yy++) {
            i += width;
            for(i32 xx = x; xx < x+w; xx++) {
                if (xx >= 0 && xx < width && yy >= 0 && yy < height) 
                    buffer[i + xx] = color;
            }
        }
    }
}

/**
 * Copy the screen buffer to the screen
 */
void VideoGraphicsArray::bufferToScreen() {
    
    // do the multiply once and test against 0
    for(int i = width * height; i >= 0; i--) {
        screen[i] = buffer[i];
    }
    
    
    
    // clear the buffer once copied
    for(int i = width * height; i >= 0; i--) {
        buffer[i] = 0;
    }
}

void VideoGraphicsArray::PutChar(char ch, i32 x, i32 y, u32 color) {
    u32 px = 0;  // position in the charactor - an 8x8 font = 64 bits
    u64 bCh = FONT[ch];
    
    // check if it will be drawn off screen
    if (x+8 < 0 || x > width || y+8 < 0 || y > height)
        return;
    
    // test if the charactor will be clipped (will it be fully in the screen or partially)
    if (x >= 0 && x+8 < width && y >= 0 && y+8 < height) {
        // fully in the screen - pre calculate some of the values
        // so there is less going on in the loop
        int i = width * (y - 1) + x + 8;
        int incAmount = width - 8;
        for (int yy = 7; yy >= 0; yy-- ) {
            i += incAmount;
            for (int xx = 7; xx >= 0; xx-- ) {
                // test if a pixel is drawn here
                if ((bCh >> px++) & 1) {
                    buffer[i] = color;
                }
                i++;
            }
        } 
    } else {
        // partially in the screen
        int xpos = 0;
        int i = width* (y - 1);    
        for (int yy = 0; yy < 8; yy++ ) {
            i += width;
            xpos = x;
            for (int xx = 7; xx >= 0; xx-- ) {
                // test if a pixel is drawn here
                if ((bCh >> px++) & 1) {
                    // test if the pixel will be off screen
                    if (xpos > 0 && xpos < width && yy+y > 0 && yy+y < height)
                        buffer[ i + xpos] = color;
                }
                xpos++;
            }
        } 
    }
}


void VideoGraphicsArray::PutStr(char* ch, i32 x, i32 y, u32 color) {
    for (i32 i; ch[i] != 0; i++, x+=8) {
        PutChar(ch[i], x, y, color);
    }
}

