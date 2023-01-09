set(ENGINE_SRC
  "src/bitboard.cpp"
  "src/attack_table.cpp"
  "src/prng.cpp"
  "src/game_state.cpp"
  "src/move_generation.cpp"
  "src/make_move.cpp"
  "src/perft.cpp"
  "src/uci.cpp"
 )

add_library (
  chess_engine
  ${ENGINE_SRC}
 )

target_compile_features(chess_engine PRIVATE cxx_std_20)
target_compile_options(chess_engine PUBLIC ${COMPILE_OPTIONS})
set_target_properties(
    chess_engine PROPERTIES
    OUTPUT_NAME chess_engine
    EXPORT_NAME lib
    CXX_EXTENSIONS OFF
)

target_include_directories(chess_engine PUBLIC "include")
