# If build directory exists, clear it. Otherwise make it.
if [[ -d ./build/ ]]; then
  rm -r ./build/*
else
  mkdir ./build/
fi

# Compile source.
cd ./build/
flex ../src/tl13.l
bison -d ../src/tl13.y
gcc -I../src/ -I../external/ ../src/tl13.c tl13.tab.c lex.yy.c -o tl13 -lfl
