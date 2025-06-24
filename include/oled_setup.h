// Define OLED_SETUP_H to prevent multiple inclusions
#ifndef OLED_SETUP_H
#define OLED_SETUP_H  

extern volatile int oled_needs_update; // Flag to indicate if OLED needs update
extern char linha1_buf[32]; // Buffer for first line of OLED text
extern char linha2_buf[32]; // Buffer for second line of OLED text

void setup_oled(void);  // Function to setup OLED display

void update_oled_text(const char* line1, const char* line2); // Function to update OLED text

#endif  // End of inclusion guard
