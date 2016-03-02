/// Initializes PORTA to communicate with LCD controller
void lcd_init(void);

/// Sets cursor position to left side of a given line
void lcd_home_line1(void);
void lcd_home_line2(void);
void lcd_home_line3(void);
void lcd_home_line4(void);

/// Prints a string to the lcd; Google "printf" for documentation.
void lprintf(const char *formatter, ...);

/// Prints a string of characters starting at the current cursor position
void lcd_puts(char *data);

/// Prints one character at the current cursor position
void lcd_putc(char data);

/// Clears the LCD
void lcd_clear(void);

/// Submits command to LCD controller
void lcd_command(char data);
