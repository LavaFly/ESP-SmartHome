all: upload

compile:
	pio run -e d1_mini

upload:
	pio run -e d1_mini -t upload

bta:
	pio run -e d1_mini_ota

ota:
	pio run -e d1_mini_ota -t upload

characters:
	kotlinc toNumArray.kt -include-runtime -d toNumArray.jar
	java -jar toNumArray.jar alphabet

websocket:
	websocat ws://led.local/ws

clean:
	pio run -t clean
