#pragma once

#include <stdio.h>
#include <stdint.h>

void sha256(const unsigned char *data, size_t len, unsigned char *out);