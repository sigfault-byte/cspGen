> WIP. The core parsing engine is implemented for the `<script>` tag, with output formatting and full directive coverage in development.

# cspGen

A high speed HTML scanner written in C.  
Designed as a lowlevel tool to assist in automatic generation of strict Content-Security-Policy (CSP) headers.
Designed for well formatted HTML. Does not attempt to sanitize or parse intentionally obfuscated / malformed code.
Does not follow redirect. 

---

## Purpose

This project parses raw HTML byte-by-byte to:
- Detect `<script>` tags, inline scripts, and suspicious attributes
- Extract JS-related sources and behaviors
- `<style>`, `<img>` , `<iframe>` 
- Map their characteristics to bit-flag directive value
- Prepare structured output for CSP policy generation covering all CSP fetch directives

The goal: a memory-efficient scanner that supports all CSP fetch directives (e.g., `script-src`, `style-src`, `img-src`, `connect-src`), and can be embedded in other security tools

---

## Structure

```text
cspGen/
├── main.c                   # Entry point
├── Makefile
├── html_fetch.[c/h]         # Loads HTML file / buffer input
├── extractor/               # Core logic for scanning and script analysis
├── model/                   # C structs: script, HTML, signature
├── config/settings.h        # Global settings and constants
```

---

## Status

- [x] Byte-level scanning of `<script>` tags
- [x] Struct-based storage of script metadata (inline, external, source, nonce, module, data URI..)
- [x] Detect unsafe inline scripts
- [x] Detect unsafe-eval script
- [x] Engine to flag correct fetch directive (script-src for now)
- [ ] Output formatting for integration
- [ ] style, iframe, connect-src ( fetch etc )...

---

## Build & Run

```bash
make
./cspGen https://github.com
```

## Output - 2025-08-09
 
 ```bash
HTTP status code: 200
Fetch duration (real): 0.437 s
Found 3439 '<' bytes
Scan completed in 0.190 ms
<html> open: 24
<html> close: 553854
<head> open: 247
<head> close: 26548
<body> open: 26559
<body> close: 553846
<script found: 62
</script found: 62
553863 bytes html body head found and pre triage Scan completed in 0.087 ms
Script struct populated linked to their origin in 0.121 ms
Populated 62 script tag(s):
Populated 58 script external
Populated 0 script with sri value
Populated 0 script relative
Populated 0 script dataURI
Populated 0 script inline
Populated 0 script with a nonce value
Populated 4 script datascript
Populated 0 script with  type=module
Finished scanning 131477 B of JS slop in 0.002 ms
Main (after fetch): 534 µs
Total execution time full pipeline: 0.437 s
```
