all: upload

compile:
	pio run -e d1_mini

upload:
	pio run -e d1_mini -t upload

bta:
	pio run -e d1_mini_ota

ota:
	pio run -e d1_mini_ota -t upload

clean:
	pio run -t clean
