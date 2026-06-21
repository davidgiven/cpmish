#include <cpm.h>
#include <stdio.h>

extern void set_vid_mode(uint8_t mode);
extern void set_pixel_x(uint8_t x);
extern void set_pixel_y(uint8_t y);
extern void set_pixel_data(uint8_t c);
extern void set_vid_page(uint8_t p);
extern void set_pal_color(uint8_t c);
extern void set_pal_r(uint8_t r);
extern void set_pal_g(uint8_t g);
extern void set_pal_b(uint8_t b);

static uint8_t buffer[128];

int main(void) {
    uint8_t i,j, pos;
    uint8_t color;
    
    // Open file
    cpm_fcb.cr = 0;

    if(cpm_open_file(&cpm_fcb) == 0xff) {
        printf("imgview <imagefile>\n");
        return;
    }
    
    // Set graphics mode
    set_vid_mode(0x01);
    
    // Load and set palette, 768 bytes = 6 blocks
    color = 0;
    pos = 0;
    for(i=0; i<6; i++) {
        cpm_set_dma(&buffer);
        cpm_read_sequential(&cpm_fcb);
        
        for(j=0; j<128; j++) {
            if(pos == 0) {
                set_pal_color(color);
                set_pal_r(buffer[j]);
                color++;
                pos++;
            } else if(pos == 1) {
                set_pal_g(buffer[j]);
                pos++;
            } else if(pos == 2) {
                set_pal_b(buffer[j]);
                pos=0;
            }

        }
    }

    // Load and set image data, 19200 bytes = 150 blocks
    set_pixel_x(0);
    set_pixel_y(0);
    set_vid_page(0);

    for(i=0; i<150; i++) {
        cpm_set_dma(&buffer);
        cpm_read_sequential(&cpm_fcb);
        for(j=0; j<128; j++) {
            set_pixel_data(buffer[j]);
        }
    }

    // Wait for keypress
    while(!cpm_const());

    // Return to text mode
    set_vid_mode(0x00);
}
