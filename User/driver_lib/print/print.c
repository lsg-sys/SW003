#include "print.h"
#include "print_if.h"

void print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = PRINT_vsnprintf(print_output_buffer, PRINT_OUTPUT_BUFFER_SIZE, format, args);
    va_end(args);
    serial_output(print_output_buffer, len);
}

int snprint(char *buffer, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = PRINT_vsnprintf(buffer, size, format, args);
    va_end(args);
    return (len);
}

int scan(char *buffer, size_t size, uint32_t timeout_ms) {
    #if PRINT_scan_enable
        return PRINT_scan(buffer, size, timeout_ms);
    #endif /* PRINT_scan_enable */
    #if PRINT_scan_disable
        return -1;
    #endif /* PRINT_scan_disable */
}

void test_print(void) {
    print("hello world\n");

    print("char: %c\n", 'A');
    print("int positive: %d\n", 1234567);
    print("int negative: %d\n", -1234567);
    print("unsigned int: %u\n", 7654321);
    print("string: %s\n", "hello");
    print("hex: %x\n", 0x12345678);

#if PRINT_scan_enable
    char testBuffer[32];
    for (int i = 0; i < 3; i++) {
        int len = scan(testBuffer, sizeof(testBuffer), PRINT_SCAN_TIMEOUT_NEVER);
        print("scan (%d/%d): %s\n", len, sizeof(testBuffer), testBuffer);
    }
#endif
}
