# 0013 — The visor covers only the 4:3 centre at 16:9

Status: open (2026-11-17). Owner: psxport `RecordRasterizer` (render-to-texture of a displayed buffer); the title's part
waits on it.

## Observed
First mission (`replays/first-mission/menu-to-mission-run-right-left.pad`, 2,050 frames), R2 held for 30 frames, 16:9:
the green visor and its rounded frame fill x 86..600 of 684 (`scratch/skips/visor/before-16x9.png`). The margins show
the untinted world, so the picture reads as a 4:3 visor between two plain strips.

## What the guest draws
All in ordering-table slot 2 of the camera's table (walk of `scratch/skips/visor2/visor.ram`, `ot.txt`), 512-wide
buffers at (0,0) and (0,256):
- two black 512x32 letterbox bars (GP0 0x60, built by 0x8008F940), which the canvas already continues into the margins;
- a subtractive 0x080808 rectangle over rows 32..207 (0x62, E1 abr 2);
- 176 one-row additive textured sprites (0x66, E1 0x330/0x334, 15-bit pages at (0,256) and (256,256)), two 256-wide
  halves per even row 32..206, each sampling the same row of the other display buffer (the previous frame) with a
  green modulation: the tint and its afterglow;
- per row 32..207 two subtractive gouraud lines (0x52) from each buffer edge inwards, white to black: the rounded frame.

## Cause
The tint is the previous frame read back as a texture. psxport presents a texture read from a displayed buffer at its
4:3 extent (`docs/presentation.md`, Render-to-texture): the other buffer's margins exist only in its host canvas, so
no sprite the title could add in the margins has margin pixels to sample. The frame lines start at the buffer edge,
so they also stay at the 4:3 edge.

## Proper fix
1. psxport: a textured draw that maps a displayed buffer one texel per pixel samples that buffer's canvas at scale,
   margins included, as the read-back texture does for a CPU-converted page (`RecordReadBack`), here for the device's
   own 15-bit sampling. That is `record_rasterizer.cpp`/`record.frag`.
2. Title: at 16:9 the row sprites and the subtractive rectangle extend across the drawn window, and each row's left
   and right frame lines move to the drawn picture's edge (`HostPackets::moveX`), so the frame bends at the screen
   edge.
