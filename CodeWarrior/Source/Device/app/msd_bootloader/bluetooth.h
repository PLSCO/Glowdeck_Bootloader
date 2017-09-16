
void bluetooth_init(void);
void bluetooth_task(void);
void bluetooth_end(void);

extern void bluetooth_status_isr(void);

/* local routines */
uint32_t bluetooth_getchar(void);
void bluetooth_putchar(uint32_t c);
void bluetooth_putstr(uint8_t *command);
uint32_t bluetooth_peek(void);
uint32_t bluetooth_strcmp( uint8_t * str1, uint8_t * str2);

