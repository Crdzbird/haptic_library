#include "library.h"
#include <cctype>   // toupper
#include <cstring>  // strlen, memcpy
#include <cstdlib>  // malloc, realloc, free

// Morse mapping A–Z then 0–9
static const char* MORSE_CODE[36] = {
        ".-",   "-...", "-.-.", "-..",  ".",    // A–E
        "..-.", "--.",  "....", "..",   ".---",// F–J
        "-.-",  ".-..", "--",   "-.",   "---", // K–O
        ".--.", "--.-", ".-.",  "...",  "-",   // P–T
        "..-",  "...-", ".--",  "-..-", "-.--",// U–Y
        "--..",                               // Z
        ".----","..---","...--","....-",".....", // 1–5
        "-....","--...","---..","----.","-----"  // 6–0
};

enum {
    DOT_DURATION     = 100,
    DASH_DURATION    = 300,
    GAP_SYMBOL       = 100,  // between symbols in same letter
    GAP_LETTER       = 300,  // between letters
    GAP_WORD         = 700   // between words
};

int* stringToHapticMorse(const char* input, size_t* outCount) {
    if (!input || !outCount) return nullptr;
    size_t len = strlen(input);
    size_t cap = len * 10 + 1;
    int* buf = (int*)malloc(cap * sizeof(int));
    if (!buf) return nullptr;

    size_t idx = 0;
    int firstWord = 1;

    for (size_t i = 0; input[i] != '\0'; ++i) {
        int c = toupper((unsigned char)input[i]);
        if (c == ' ') {
            if (!firstWord) buf[idx++] = GAP_WORD;
            firstWord = 0;
            continue;
        }
        firstWord = 0;

        int mapIndex = -1;
        if (c >= 'A' && c <= 'Z')       mapIndex = c - 'A';
        else if (c >= '0' && c <= '9')  mapIndex = (c == '0' ? 35 : (c - '1' + 26));
        if (mapIndex < 0) continue;

        const char* code = MORSE_CODE[mapIndex];
        for (size_t j = 0; code[j] != '\0'; ++j) {
            buf[idx++] = (code[j] == '.') ? DOT_DURATION : DASH_DURATION;
            if (code[j+1] != '\0') buf[idx++] = GAP_SYMBOL;
            // grow if needed
            if (idx + 10 > cap) {
                cap *= 2;
                int* tmp = (int*)realloc(buf, cap * sizeof(int));
                if (!tmp) { free(buf); return nullptr; }
                buf = tmp;
            }
        }
        buf[idx++] = GAP_LETTER;
    }

    // finalize length and shrink-to-fit safely
    *outCount = idx;
    size_t newBytes = idx * sizeof(int);
    int* tmp = (int*)realloc(buf, newBytes);
    if (tmp) buf = tmp;
    return buf;
}

char* stringToMorse(const char* input) {
    if (!input) return nullptr;
    size_t len = strlen(input);
    size_t cap = len * 6 + 1;
    char* out = (char*)malloc(cap);
    if (!out) return nullptr;

    size_t pos = 0;
    int firstToken = 1;

    for (size_t i = 0; input[i] != '\0'; ++i) {
        int c = toupper((unsigned char)input[i]);
        if (c == ' ') {
            if (!firstToken) {
                const char* sep = " / ";
                size_t sl = strlen(sep);
                if (pos + sl + 1 > cap) {
                    cap = (pos + sl + 1) * 2;
                    char* tmpc = (char*)realloc(out, cap);
                    if (!tmpc) { free(out); return nullptr; }
                    out = tmpc;
                }
                memcpy(out + pos, sep, sl);
                pos += sl;
                firstToken = 1;
            }
            continue;
        }

        int mapIndex = -1;
        if (c >= 'A' && c <= 'Z')       mapIndex = c - 'A';
        else if (c >= '0' && c <= '9')  mapIndex = (c == '0' ? 35 : (c - '1' + 26));
        if (mapIndex < 0) continue;

        const char* code = MORSE_CODE[mapIndex];
        size_t cl = strlen(code);
        if (!firstToken) {
            if (pos + 1 + cl + 1 > cap) {
                cap = (pos + 1 + cl + 1) * 2;
                char* tmpc = (char*)realloc(out, cap);
                if (!tmpc) { free(out); return nullptr; }
                out = tmpc;
            }
            out[pos++] = ' ';
        } else if (pos + cl + 1 > cap) {
            cap = (pos + cl + 1) * 2;
            char* tmpc = (char*)realloc(out, cap);
            if (!tmpc) { free(out); return nullptr; }
            out = tmpc;
        }

        memcpy(out + pos, code, cl);
        pos += cl;
        firstToken = 0;
    }

    // NUL terminate and shrink-to-fit
    out[pos] = '\0';
    char* tmpc = (char*)realloc(out, pos + 1);
    if (tmpc) out = tmpc;
    return out;
}

////OK
//#include "library.h"
//#include <cctype>   // toupper
//#include <cstring>  // strlen, strcpy
//#include <cstdlib>  // malloc, realloc, free
//
//// Morse mapping A–Z then 0–9
//static const char* MORSE_CODE[36] = {
//        ".-", "-...", "-.-.", "-..", ".",   // A–E
//        "..-.", "--.", "....", "..", ".---",// F–J
//        "-.-", ".-..", "--",  "-.",  "---",  // K–O
//        ".--.", "--.-", ".-.", "...", "-",   // P–T
//        "..-",  "...-", ".--", "-..-", "-.--",// U–Y
//        "--..",                               // Z
//        ".----","..---","...--","....-",".....", // 1–5
//        "-....","--...","---..","----.","-----"  // 6–0
//};
//
//enum {
//    DOT_DURATION     = 100,
//    DASH_DURATION    = 300,
//    GAP_SYMBOL       = 100,  // between dots/dashes in same letter
//    GAP_LETTER       = 300,  // between letters
//    GAP_WORD         = 700   // between words
//};
//
//int* stringToHapticMorse(const char* input, size_t* outCount) {
//    if (!input || !outCount) return NULL;
//    size_t len = strlen(input);
//    // worst‑case: each char ≈ 10 ints; words add a few more
//    size_t cap = len * 10 + 1;
//    int* buf = (int*)malloc(cap * sizeof(int));
//    if (!buf) return NULL;
//
//    size_t idx = 0;
//    int firstWord = 1;
//
//    for (size_t i = 0; input[i] != '\0'; ++i) {
//        char c = toupper((unsigned char)input[i]);
//        if (c == ' ') {
//            if (!firstWord) {
//                buf[idx++] = GAP_WORD;
//            }
//            firstWord = 0;
//            continue;
//        }
//        firstWord = 0;
//
//        int mapIndex = -1;
//        if (c >= 'A' && c <= 'Z')         mapIndex = c - 'A';
//        else if (c >= '0' && c <= '9')    mapIndex = (c == '0' ? 35 : (c - '1' + 26));
//        if (mapIndex < 0)
//            continue;  // skip unsupported
//
//        const char* code = MORSE_CODE[mapIndex];
//        // dot/dash + intra‑symbol gaps
//        for (size_t j = 0; code[j] != '\0'; ++j) {
//            buf[idx++] = (code[j] == '.') ? DOT_DURATION : DASH_DURATION;
//            if (code[j+1] != '\0')
//                buf[idx++] = GAP_SYMBOL;
//        }
//        // letter gap
//        buf[idx++] = GAP_LETTER;
//
//        // grow if needed
//        if (idx + 10 > cap) {
//            cap *= 2;
//            buf = (int*)realloc(buf, cap * sizeof(int));
//            if (!buf) return NULL;
//        }
//    }
//
//    // shrink to fit
//    *outCount = idx;
//    buf = (int*)realloc(buf, idx * sizeof(int));
//    return buf;
//}
//
//char* stringToMorse(const char* input) {
//    if (!input) return NULL;
//    size_t len = strlen(input);
//    // each char → up to 5 symbols + 1 space; words → 3 chars " / "
//    size_t cap = len * 6 + 1;
//    char* out = (char*)malloc(cap);
//    if (!out) return NULL;
//
//    size_t pos = 0;
//    int firstToken = 1;
//    for (size_t i = 0; input[i] != '\0'; ++i) {
//        char c = toupper((unsigned char)input[i]);
//        if (c == ' ') {
//            // word separator
//            if (!firstToken) {
//                const char* sep = " / ";
//                size_t sl = strlen(sep);
//                memcpy(out + pos, sep, sl);
//                pos += sl;
//                firstToken = 1;
//            }
//            continue;
//        }
//
//        int mapIndex = -1;
//        if (c >= 'A' && c <= 'Z')         mapIndex = c - 'A';
//        else if (c >= '0' && c <= '9')    mapIndex = (c == '0' ? 35 : (c - '1' + 26));
//        if (mapIndex < 0)
//            continue;
//
//        if (!firstToken) {
//            out[pos++] = ' ';
//        }
//        firstToken = 0;
//
//        const char* code = MORSE_CODE[mapIndex];
//        size_t cl = strlen(code);
//        memcpy(out + pos, code, cl);
//        pos += cl;
//    }
//    out[pos] = '\0';
//    // shrink
//    out = (char*)realloc(out, pos + 1);
//    return out;
//}
