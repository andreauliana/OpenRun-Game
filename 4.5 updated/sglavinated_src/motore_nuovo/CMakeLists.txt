# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.10)
project(sglavinated LANGUAGES C CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Target: sglavinated_core — nucleo C++17 indipendente dalla piattaforma.
# Nessuna dipendenza da SDL, solo librerie standard.
# ---------------------------------------------------------------------------
add_library(sglavinated_core
    src/core/frame_counter.h
    src/core/rng.h
    src/core/state.h
    src/core/json_mini.h
    src/core/replay.h
    src/core/replay.cpp
    src/road/road_segment.h
    src/road/road.h
    src/road/road.cpp
    src/render/framebuffer.h
    src/render/framebuffer.cpp
    src/render/palette.h
    src/render/palette.cpp
    src/render/sprite.h
    src/render/renderer.h
    src/render/renderer.cpp
    src/render/sprite_raw.h
    src/render/sprite_raw.cpp
    src/input/pad.h
    src/audio/mixer.h
    src/audio/mixer.cpp
    src/player/player.h
    src/player/player.cpp
    src/game/game.h
    src/game/game.cpp
    # ---------------------------------------------------------------------------
    # Modulo gameplay (blocco D): interfaccia verso il nucleo (show.h).
    # Scritto da @traduttore in src/gameplay/; @coder lo collega al ciclo di gioco
    # e lo aggiunge alla build del core (CMakeLists.txt).
    # ---------------------------------------------------------------------------
    src/gameplay/escalation.h
    src/gameplay/frasi.h
    src/gameplay/frasi.cpp
    src/gameplay/personaggi.h
    src/gameplay/show.h
    src/gameplay/show.cpp
    src/gameplay/ufo.h
    # Disegno show (blocco D/P3): scena personaggio/fumetto/UFO in px, zero SDL.
    src/gameplay/disegno_show.h
    src/gameplay/disegno_show.cpp
    # Testi fumetti da dati/frasi.json (letto una volta in init).
    src/gameplay/testi_frasi.h
    src/gameplay/testi_frasi.cpp
)

target_include_directories(sglavinated_core PRIVATE src)

# ---------------------------------------------------------------------------
# Target: sglavinated_runner — runner desktop/terminal con SDL2 (solo test).
# Richiede SDL2 (video + audio). Non e' parte del core.
# ---------------------------------------------------------------------------
find_package(SDL2 QUIET)
find_package(SDL2_image CONFIG QUIET) # PNG (cartelloni sponsor) nel runner di test; target SDL2_image::SDL2_image
if(SDL2_FOUND)
    add_executable(sglavinated_runner
        src/runner/main.cpp
        src/runner/sponsor_loader.cpp
    )
    target_include_directories(sglavinated_runner PRIVATE src)
    target_link_libraries(sglavinated_runner PRIVATE sglavinated_core SDL2::SDL2 SDL2::SDL2main)
    if(TARGET SDL2_image::SDL2_image)
        target_link_libraries(sglavinated_runner PRIVATE SDL2_image::SDL2_image)
        target_compile_definitions(sglavinated_runner PRIVATE SGL_HAVE_SDL2_IMAGE)
    elseif(TARGET SDL2::SDL2_image)
        target_link_libraries(sglavinated_runner PRIVATE SDL2::SDL2_image)
        target_compile_definitions(sglavinated_runner PRIVATE SGL_HAVE_SDL2_IMAGE)
    else()
        message(STATUS "SDL2_image non trovata: target runner compilato senza decodifica PNG "
                       "(cartelloni sponsor disabilitati, gioco comunque funzionante).")
    endif()
else()
    message(STATUS "SDL2 non trovata: target sglavinated_runner escluso. "
                   "Installare libsdl2-dev (o brew install sdl2) e rilanciare cmake.")
endif()

# ---------------------------------------------------------------------------
# Target: sglavinated_libretro — core libretro (stub documentato, BLOCCO C).
# Da attivare quando i header libretro saranno disponibili nella cartella
# del progetto (es. libreria/libretro/ o submodule).
# ---------------------------------------------------------------------------
option(BUILD_LIBRETRO "Compila il core libretro (stub)" OFF)
if(BUILD_LIBRETRO)
    add_library(sglavinated_libretro MODULE
        src/libretro/sglavinated_libretro.cpp
    )
    target_include_directories(sglavinated_libretro PRIVATE src)
    set_target_properties(sglavinated_libretro PROPERTIES
        PREFIX "" SUFFIX "_libretro.so")
    message(STATUS "Core libretro costruito come stub (vedi src/libretro/sglavinated_libretro.cpp).")
endif()

# ---------------------------------------------------------------------------
# Target: prova_gate — test automatico del gate Tappa 1 (blocco P1e/H).
# Guida da solo 10 s e salva 3 screenshot (partenza/curva/fuori-strada) in
# out/collaudatore/ con velocita' e posizione nel log.
# Il file sorgente motore_nuovo/test/prova_gate.cpp e' consegnato da @debugger.
# Core-only: nessun SDL, output PPM puro senza librerie esterne.
# Il target si attiva solo quando prova_gate.cpp esiste.
# ---------------------------------------------------------------------------
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/prova_gate.cpp")
    add_executable(prova_gate test/prova_gate.cpp)
    target_include_directories(prova_gate PRIVATE src)
    target_link_libraries(prova_gate PRIVATE sglavinated_core)

    if(BUILD_TESTING)
        set(GATE_TEST_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/test-results")
        foreach(level lv_01_neon lv_02_atomico lv_03_foglie)
            add_test(
                NAME gate_smoke_${level}
                COMMAND prova_gate "${GATE_TEST_OUTPUT}/${level}" ${level}
            )
            set_tests_properties(gate_smoke_${level} PROPERTIES
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
            )
        endforeach()

        foreach(mode accelerazione crash sterzo curve)
            add_test(
                NAME gate_${mode}
                COMMAND prova_gate "${GATE_TEST_OUTPUT}/${mode}" lv_01_neon ${mode}
            )
            set_tests_properties(gate_${mode} PROPERTIES
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
            )
        endforeach()
    endif()
endif()
