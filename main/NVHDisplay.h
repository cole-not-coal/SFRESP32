#ifndef SFR_NVHDISPLAY
#define SFR_NVHDISPLAY

#include "main.h"
#include "./NVHDisplay/EVE.h"
#include "./NVHDisplay/EVE_supplemental.h"

#include <string.h>

/* --------------------------- Function prototypes -------------------------- */
void NVHDisplay_init(void);
void initStaticBackground(void);
void TFT_display(void);

#endif // SFR_NVHDISPLAY