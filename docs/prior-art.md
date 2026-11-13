# Prior art — C-12: Final Resistance (USA `SCUS_946.66`, SHA-256 `0b93d073ecc211a51431ee8e3eaf8e72f3aa33e6bc0b985a408c0f37a4cc9c87`, `title.json`)

There is no decompilation of this executable. Refreshed 2026-11-13. A borrowed address is a lead until measured on our image.

| project | URL | kind | license | target build and match | what we may take | verified features | last push |
|---|---|---|---|---|---|---|---|
| spartaque12/c12-recomp | https://github.com/spartaque12/c12-recomp | static recompilation (own Python MIPS-to-C recompiler `tools/recomp/`, own Windows runtime), mods, VR | none | PAL `SCES_033.64` (`recomp.json`: entry `0x800A7CA0`, overlays `RELOCS/*.LVB`); different build from ours (entry `0x800A8454`); generated `gen/` and the claimed `re/main_decomp.c` are not committed | evidence only | widescreen projection and X-saturation culling widening in `runtime/gte.cpp`; frame interpolation option `interp` in `runtime/main.cpp`; game glue in `runtime/game_c12.cpp` | 2026-11-13 |
| Dedok179/C-12-Final-Resistance-Level-Geometry-Extractor | https://github.com/Dedok179/C-12-Final-Resistance-Level-Geometry-Extractor | level geometry extractor (C#), reads the executable and the MWD archive | none | region not stated; matched on our image (2026-10-13): load base `0x80010000`; level table `0x800D1F78` (92-byte entries, section count `+0xF`, pointer `+0x10`) with a data ref from `FUN_8003777c` (`0x80037870`); MWD file table at file offset `0xB0830` (`0x800D0030`, 36-byte entries) whose sector/size fields chain exactly | evidence only: table layouts, PP20 decompression | | 2025-10-17 |
| RHDN forum, C-12 control modification | https://www.romhacking.net/forum/index.php?topic=39935.0 | camera and aim control patch (removes inverted vertical aim) | none stated | not checked | evidence only | | |

Not found (2026-11-13): a decompilation, splat config, symbol map, Ghidra/IDA database, PC port of the USA build, RAM map or modding SDK for `SCUS_946.66` on GitHub, decomp.dev or the web.
