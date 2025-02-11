set(PYTHON_EXECUTABLE "python3")
set(MINIFIED_HTML "${CMAKE_BINARY_DIR}/index_home.min.html")

# Executa o script ANTES da compilação
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/minify.py
    RESULT_VARIABLE result
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "Erro ao minificar HTML!")
endif()

# Agora a variável MINIFIED_HTML pode ser usada no CMake principal
set(EMBED_MINIFIED_HTML ${MINIFIED_HTML} CACHE INTERNAL "Minified HTML file")
