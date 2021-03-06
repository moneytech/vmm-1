/*

how this thing works:

	+-----------+
	|  our own  |
	|  text     |
	|  buffer   |
	|           |
  #=|===========|=#
  I |           | I
  I |           | I  Overton window: defined by TTY_OFFSET
  I |           | I  text from here is shown into the terminal
  #=|===========|=#
	|           |
	|           |
	+-----------+

Once bottom boundary of the owindow hits the buffer's bottom we do:
1) copy a bottom half of the buffer to the buffer
2) fix offset

*/

// todo: rename this file
// todo: make ansi escape sequences great again

#include <jinet/tty.h>
#include <jinet/vga.h>
#include <jinet/memory.h>
#include <jinet/module.h>

MODULE("VBE_TTY");

static size_t tty_x, tty_y;
static vga_color tty_bg, tty_fg;
static void* TTY_BUFFER;// = 0x7c00;
static int TTY_OFFSET = 0;

int TTY_WIDTH, TTY_HEIGHT;

void tty_clear();
static void tty_putsymb(uint8_t c, vga_color bg, vga_color fg, int x, int y);

int tty_init(void* tty_fb, int width, int height)
{
	TTY_WIDTH = width / 8;
	TTY_HEIGHT = height / 16;
	TTY_BUFFER = tty_fb;
	// for(int x = 0; x<TTY_WIDTH; x++)
	// 	for(int y = 0; y<TTY_MAX_LINES; y++)
	// 	{
	// 		//mprint("%d %d", x, y);
	// 		tty_putsymb(0, tty_bg, tty_fg, x, y);
	// 	}
	tty_clear();

	COM_putc('t');
	COM_putc('t');
	COM_putc('y');
	tty_reset_color();
	return 0;
}

void tty_clear()
{
	tty_x = 0;
	tty_y = 0;
	mprint("%llx", TTY_BUFFER+8);
	memset(TTY_BUFFER+8,0,sizeof(tty_char)*TTY_WIDTH*TTY_MAX_LINES);
}

static void tty_putsymb(uint8_t c, vga_color bg, vga_color fg, int x, int y)
{
	volatile tty_char* s = (uint64_t)TTY_BUFFER;
	s += TTY_WIDTH*y+x;
	s->symb = c;
	s->bg = bg;
	s->fg = fg;
}

void tty_refresh_sym(int x, int y)
{
	volatile tty_char *s = (uint64_t)TTY_BUFFER;
	s += TTY_WIDTH*(y+TTY_OFFSET)+x;
	vbe_putc(s->symb, s->bg, s->fg, x, y);
}

static void tty_half_mix()
{
	if(TTY_OFFSET <= TTY_MAX_LINES / 2) return;
	tty_char* s = TTY_BUFFER;
	s += TTY_WIDTH*TTY_MAX_LINES/2;
	memcpy(TTY_BUFFER, (void *)s, sizeof(tty_char)*TTY_WIDTH*TTY_MAX_LINES/2);
	memset(s, 0, sizeof(tty_char)*TTY_WIDTH*TTY_MAX_LINES/2);
	tty_y -= TTY_MAX_LINES/2;
	TTY_OFFSET -= TTY_MAX_LINES/2;
	tty_refresh_all();
}

void tty_putc(uint8_t a)
{
	if(a == '\n') {tty_y++, tty_x = 0; }
	if(tty_x > TTY_WIDTH - 1) tty_x = 0, tty_y++;
	if(a != '\n')
	{
		tty_putsymb(a, tty_bg, tty_fg, tty_x, tty_y);
		tty_refresh_sym(tty_x, tty_y - TTY_OFFSET);
		tty_x++;
	}
	if(tty_y - TTY_OFFSET > TTY_HEIGHT-1)
	{
		TTY_OFFSET+=10;
		tty_refresh_all();
	}
	if(TTY_MAX_LINES - tty_y <= 10) // critical
	{
		tty_half_mix();
	}
}

void tty_reset_color()
{
	tty_bg = VC_BLACK;
	tty_fg = VC_WHITE;
}


void tty_setfg(vga_color fg)
{
	tty_fg = fg;
}

vga_color tty_getfg()
{
	return tty_fg;
}

void tty_setbg(vga_color bg)
{
	tty_bg = bg;
}

vga_color tty_getbg()
{
	return tty_bg;
}

void tty_refresh_all()
{
	//return;
	for(int i = 0; i < TTY_WIDTH; i++)
		for(int j = 0; j < TTY_HEIGHT; j++)
			tty_refresh_sym(i,j);
}