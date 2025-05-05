#ifndef LIBRARY_H
#define LIBRARY_H

#include <cstddef>  // for size_t

#ifdef __cplusplus
extern "C" {
#endif

/// Convert text → haptic durations.
///   - input: NUL‑terminated ASCII (A–Z, 0–9, space). Others skipped.
///   - *outCount set to number of ints returned.
///   - Returns a malloc’d int array; caller must free().
int* stringToHapticMorse(const char* input, size_t* outCount);

/// Convert text → human‑readable Morse string.
///   - input: NUL‑terminated ASCII (A–Z, 0–9, space).
///   - Returns a malloc’d C‑string; caller must free().
char* stringToMorse(const char* input);

#ifdef __cplusplus
}
#endif

#endif // LIBRARY_H
