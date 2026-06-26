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
extern uint8_t get_pal_r(void);
extern uint8_t get_pal_g(void);
extern uint8_t get_pal_b(void);

static uint8_t buffer[128];
static uint8_t pal[768];
static uint8_t vid_mode;
FCB img_fcb;

void print_info(void) {
    printf("imgview <gfxmode> <imagefile>\n");
    printf("gfxmode == 0 -> 160x120x8\n");
    printf("gfxmode == 1 -> 320x200x8\n");
    cpm_exit();
}

int main(int argc, const char *argv[]) {
    uint16_t i, blocks, n;
    uint8_t j, pos;
    uint8_t color;
    
    if(argc != 3) print_info();

    if(*argv[1]=='0') {
        vid_mode = 0x01;;
        blocks = 150;
    } else if(*argv[1]=='1') {
        vid_mode = 0x02;
        blocks = 500;
    } else print_info();
    
    cpm_parse_filename(&img_fcb, argv[2]);
    if(cpm_open_file(&img_fcb) == 0xff) print_info();    

    // Set graphics mode
    set_vid_mode(vid_mode);
    
    // Load and set palette, 768 bytes = 6 blocks
    color = 0;
    pos = 0;
    n = 0;
    for(i=0; i<6; i++) {
        cpm_set_dma(&buffer);
        cpm_read_sequential(&img_fcb);
        
        for(j=0; j<128; j++) {
            if(pos == 0) {
                set_pal_color(color);
                pal[n] = get_pal_r();
                n++;      
                set_pal_r(buffer[j]);
                color++;
                pos++;
            } else if(pos == 1) {
                pal[n] = get_pal_g();
                n++;
                set_pal_g(buffer[j]);
                pos++;
            } else if(pos == 2) {
                pal[n] = get_pal_b();
                n++;
                set_pal_b(buffer[j]);
                pos=0;
            }

        }
    }

    // Load and set image data, 19200 bytes = 150 blocks
    set_pixel_x(0);
    set_pixel_y(0);
    set_vid_page(0);

    for(i=0; i<blocks; i++) {
        cpm_set_dma(&buffer);
        cpm_read_sequential(&img_fcb);
        for(j=0; j<128; j++) {
            set_pixel_data(buffer[j]);
        }
    }

    // Wait for keypress
    while(!cpm_const());

    // Return to text mode
    set_vid_mode(0x00);

    // Restore palette
    n=0;
    for(i=0; i<256; i++) {
        set_pal_color(i);
        set_pal_r(pal[n]); n++;
        set_pal_g(pal[n]); n++;
        set_pal_b(pal[n]); n++;
    }
}
