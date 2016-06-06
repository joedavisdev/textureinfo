grep -e "#define.*0x.*" *.h | sed 's/#define \(.*\)/X\(\1\)/' | sed 's/ 0x/,=0x/' > gl.def
