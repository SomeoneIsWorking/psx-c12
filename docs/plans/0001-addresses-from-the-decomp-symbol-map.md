# 0001 — Guest addresses from the decomp symbol map

C-12's decomp is `external/c12-decomp` (MIT, ours); its symbol map is `config/symbols/scus_946.66.txt`, splat
`symbol_addrs` format, and it already names every address `game/facts/title_facts.h` holds. The port still reads hand
copies.

## Generator

The shared psxport generator of Tomba! 2's plan (psx-tomba `docs/plans/0065-addresses-from-the-decomp-symbol-map.md`):
`tools/symbol_constants.py` reads splat `symbol_addrs` files into one header per image under `build/`
(`namespace c12::sym::scus_946_66`), through a CMake function. It does not exist yet; C-12 is a consumer, not the
place it is written.

## Order

1. Once the generator lands, add `psxport_symbol_constants(c12_runtime IMAGE scus_946.66 MAP
   external/c12-decomp/config/symbols/scus_946.66.txt)` to `CMakeLists.txt`.
2. `game/facts/title_facts.h`: each address constant becomes a use of the generated name; sizes, strides, counts and
   the packet pool window stay. `kWorldHeaderFlagsAddress` is `g_WorldHeader + 4`.
3. The comments that name a `FUN_` address in `game/` take the map's name.
4. Build with Clang; boot to the first mission (`replays/first-mission/`) to show no binding moved.

Until then a new guest address goes into the decomp's map first (decomp commit, submodule bump in the port change that
uses it), then into `title_facts.h`.
