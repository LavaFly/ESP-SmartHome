webpage := include/webpage.html

all: upload

webpage: toByteArray.jar $(webpage)
	java -jar toByteArray.jar $(webpage)

toByteArray.jar: toByteArray.kt
	kotlinc toByteArray.kt -include-runtime -d toByteArray.jar

compile:
	pio run -e d1_mini

upload:
	pio run -e d1_mini -t upload

bta:
	pio run -e d1_mini_ota

ota:
	pio run -e d1_mini_ota -t upload

clean:
	rm -f toByteArray.jar
	rm -f include/webpage.h
	pio run -t clean
