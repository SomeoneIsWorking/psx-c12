# 0013 — The visor covers only the 4:3 centre at 16:9

Status: fixed (2026-11-17). Owners: psxport `RecordPlanner`/`record.frag` (a displayed-buffer read) and
`PacketChainWalker` (the in-between capture draws host moves); the title's `VisorFrame`.

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

## Fix
- psxport: a 15-bit textured draw mapping a displayed buffer one texel per pixel reads that buffer's canvas
  (`docs/presentation.md`, Displayed-buffer read), and the sprites and the 0x62 tile continue into the margins as a
  read-back sprite does, so the tint and afterglow cover the drawn window with no title code.
- psxport: `PacketChainWalker::capture` drew a redraw's chain without its host moves and replacements, so every
  in-between showed the frame lines at the 4:3 edge; it now draws each node through `drawnPacket`, as the DrawOTag walk
  does.
- Title: `VisorFrame` (`game/render/visor_frame.*`) records where `Visor_Draw` put the 176 rows of frame lines and moves
  the left one of each row by the left margin and the right one by the right margin (`HostPackets::moveX`), on the real
  frame and again in each in-between redraw (`DrawSection::draw`).

## Proof
Same replay, R2 held, then turning left with the visor up, 16:9, fps60, ires 0, 3840x2160 sink, unpaced: the tint,
afterglow and rounded frame reach both screen edges on real and in-between presents alike
(`scratch/visor13/edges-turn-169-seq.png`, six consecutive presents' margins; `turn-169-3.png`, real/in-between/real).
While turning, each in-between's margins sit halfway: the shift from the real present before equals the shift to the
one after (29/30, 35/35, 40/40 quarter-pixels left; 30/31, 35/36, 36/40 right). In-betweens refused: 0. 4:3 draws
the same visor as before (`turn-43-3.png`).
