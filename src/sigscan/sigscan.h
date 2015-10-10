#ifndef SIGSCAN_H
#define SIGSCAN_H

extern void *SigScan(const char *sig, void *_base);
extern bool SigScanCompare(const char *sig, void *_addr);

#endif