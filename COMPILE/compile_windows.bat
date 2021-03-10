set SDL2_flags=-I SDL2\x86_64-w64-mingw32\include -L SDL2\x86_64-w64-mingw32\lib
set SDL2_ttf_flags=-I SDL2_ttf\x86_64-w64-mingw32\include -L SDL2_ttf\x86_64-w64-mingw32\lib



gcc -o .\LogoLike.exe ^
%SDL2_flags% %SDL2_ttf_flags% ^
-I SDL2\x86_64-w64-mingw32\include\SDL2 ^
..\main.c ..\OBJECTS\objects.c ..\LEVEL\loadProject.c ..\LEVEL\cmdToken\execute\execute.c ..\LEVEL\cmdToken\tokenizer\tokenizer.c ^
../map/src/map.c ^
-lmingw32 ^
-lSDL2main ^
-lSDL2 ^
-lSDL2_ttf ^
-lm -Os



::gcc -o ..\LEVEL\cmdToken\cmdToken.o ^
::-I SDL2\x86_64-w64-mingw32\include\SDL2 ^
::%SDL2_flags% %SDL2_ttf_flags% ^
::..\LEVEL\cmdToken\execute\execute.c ..\LEVEL\cmdToken\tokenizer\tokenizer.c ^
::-lSDL2 ^
::-lSDL2_ttf ^
::-lm -Os

::g++ -o .\LogoLike_output ^
::%SDL2_flags% ^
::..\main.c ..\OBJECTS\objects.c ..\LEVEL\loadProject.c ..\LEVEL\cmdToken\cmdToken.o ^
::..\map\src\map.c ^

::-lSDL2_ttf ^
::-lm -Os




