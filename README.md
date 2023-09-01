# bezierCurves
Small demo program for Bezier curves of various degrees

# Compilation
## Windows (MinGW64)
```
$ g++ main.cpp bezier.cpp olcPixelGameEngine.cpp -o bezier.exe -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
```

## Linux
```
$ g++ main.cpp bezier.cpp olcPixelGameEngine.cpp -o bezier -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
```

# PixelGameEngine
PGE by OneLoneCoder: https://github.com/OneLoneCoder/olcPixelGameEngine