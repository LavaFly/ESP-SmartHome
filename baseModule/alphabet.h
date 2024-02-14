const uint_8 alphabet[26][6][5] = {
	{{1,1,1,1,0},{1,0,0,1,0},{1,1,1,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0}}, // A
	{{1,1,0,0,0},{1,0,1,0,0},{1,1,1,0,0},{1,0,1,0,0},{1,0,0,1,0},{1,1,1,1,0}}, // B
	{{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,0}}, // C
	{{1,1,1,0,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,1,1,1,0}}, // D
	{{1,1,1,1,0},{1,0,0,0,0},{1,1,1,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,0}}, // E
	{{1,1,1,1,0},{1,0,0,0,0},{1,1,1,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0}}, // F
	{{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,1,1,1},{1,0,0,0,1},{1,1,1,1,1}}, // G
	{{1,0,0,1,0},{1,0,0,1,0},{1,1,1,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0}}, // H
	{{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0}}, // I
	{{1,1,1,1,0},{0,0,0,1,0},{0,0,0,1,0},{0,0,0,1,0},{1,0,0,1,0},{0,1,1,1,0}}, // J
	{{1,0,1,0,0},{1,0,1,0,0},{1,0,1,0,0},{1,1,1,0,0},{1,0,0,1,0},{1,0,0,1,0}}, // K
	{{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,0}}, // L
	{{0,0,0,0,0},{1,0,1,0,0},{1,1,1,1,0},{1,0,1,0,1},{1,0,0,0,0},{1,0,0,0,0}}, // M
	{{1,0,0,0,1},{1,1,0,0,1},{1,0,1,0,1},{1,0,0,1,1},{1,0,0,1,1},{1,0,0,0,1}}, // N
	{{1,1,1,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,1,1,1,0}}, // O
	{{1,1,1,1,0},{1,0,0,1,0},{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0}}, // P
	{{1,1,1,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,1,1,0},{1,0,0,1,0},{1,1,1,1,1}}, // Q
	{{1,1,1,0,0},{1,0,0,1,0},{1,1,1,1,0},{1,0,1,0,0},{1,0,0,1,0},{1,0,0,0,1}}, // R
	{{1,1,1,1,0},{1,0,0,0,0},{1,1,1,1,0},{0,0,0,1,0},{0,0,0,1,0},{1,1,1,1,0}}, // S
	{{1,1,1,1,1},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0}}, // T
	{{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,1,1,1,0}}, // U
	{{1,0,0,0,0},{0,1,0,0,0},{0,0,1,0,0},{0,0,0,1,0},{0,0,0,0,1},{0,0,0,0,0}}, // V
	{{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,1,0},{0,0,0,1,0}}, // W
	{{1,0,1,0,0},{0,1,1,0,0},{0,0,1,0,0},{0,0,1,1,0},{0,0,1,0,1},{0,0,1,0,0}}, // X
	{{1,0,0,1,0},{0,1,0,1,0},{0,0,1,1,0},{0,0,0,1,0},{0,0,0,1,0},{0,0,0,1,0}}, // Y
	{{1,1,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,1,1}}  // Z
};
