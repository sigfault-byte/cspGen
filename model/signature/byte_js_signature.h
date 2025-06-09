#pragma once

# include <stdio.h>

// Unsafe JavaScript function signatures for inline script detection
// setTimeout() needs special care for unsafe-eval, the first argument is passed to eval behind the scene.
static const unsigned char SIG_EVAL[]        = "eval(";
static const unsigned char SIG_FUNCTION[]    = "Function(";
static const unsigned char SIG_SETTIMEOUT[]  = "setTimeout(";
static const unsigned char SIG_SETINTERVAL[] = "setInterval(";
static const unsigned char SIG_NEWFUNC[]     = "new Function(";
// static const unsigned char SIG_WRITE[]       = "document.write(";
// static const unsigned char SIG_INNERHTML[]   = "innerHTML=";
// static const unsigned char SIG_OUTERHTML[]   = "outerHTML=";

static const struct {
    const char* label;
    const unsigned char* pattern;
    size_t length;
} js_signatures[] = {
    { "eval",        SIG_EVAL,        sizeof(SIG_EVAL) - 1 },
    { "Function",    SIG_FUNCTION,    sizeof(SIG_FUNCTION) - 1 },
    { "setTimeout",  SIG_SETTIMEOUT,  sizeof(SIG_SETTIMEOUT) - 1 },
    { "setInterval", SIG_SETINTERVAL, sizeof(SIG_SETINTERVAL) - 1 },
    { "newFunction", SIG_NEWFUNC,     sizeof(SIG_NEWFUNC) - 1 },
    // { "docWrite",    SIG_WRITE,       sizeof(SIG_WRITE) - 1 },
    // { "innerHTML",   SIG_INNERHTML,   sizeof(SIG_INNERHTML) - 1 },
    // { "outerHTML",   SIG_OUTERHTML,   sizeof(SIG_OUTERHTML) - 1 }
};