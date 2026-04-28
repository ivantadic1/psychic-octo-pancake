# America: No Peace Beyond the Line – Fix

This project fixes a crash caused by incorrect framebuffer copy logic.

## Status

- DLL patch implemented
- Injector in progress

## Technical summary

Fix replaces:
ADD EDX, EDX → ADD EAX, EAX

Correcting width vs copyWidth mismatch.
