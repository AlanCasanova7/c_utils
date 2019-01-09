#define ASM_RAND(out){          \
    asm(                        \
        "XOR %%EAX, %%EAX\r\n"  \
        "CLC\r\n"               \
        "RDRAND %%EAX"          \
        : "=a"(out)             \
    );                          \
}
// NOT WORKING, RDRAND RETURNS ALWAYS THE SAME NUMBER.

#define ASM_GET_CPU_NAME(out){                                                                          \
    int string_array [4];                                                                               \
    asm(                                                                                                \
        "CPUID\r\n"                                                                                     \
        : "=a"(string_array[0]), "=c"(string_array[1]), "=d"(string_array[2]), "=b"(string_array[3])    \
        : "a"(0)                                                                                        \
    );                                                                                                  \
    out = (char*)string_array;                                                                          \
}