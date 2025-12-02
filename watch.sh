#!/bin/sh

ls src/*.cpp | entr -r sh -c "
    echo '🔧 Rebuilding...'
    cd build && make -j
    if [ \$? -eq 0 ]; then
        echo '🚀 Running...'
        ./cg_with_opengl
    fi
"
