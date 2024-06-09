html := website/webpage.html
js := website/webpage.js
css := website/webpage.css

all: html js css upload

html: toByteArray.jar $(html)
	java -jar toByteArray.jar $(html)

js: toByteArray.jar $(js)
	java -jar toByteArray.jar $(js)

css: toByteArray.jar $(css)
	java -jar toByteArray.jar $(css)

toByteArray.jar: toByteArray.kt
	kotlinc toByteArray.kt -include-runtime -d toByteArray.jar

compile: html js css
	pio run -e d1_mini

upload:
	pio run -e d1_mini -t upload

bta:
	pio run -e d1_mini_ota

ota:
	pio run -e d1_mini_ota -t upload

characters:
	kotlinc toNumArray.kt -include-runtime -d toNumArray.jar
	java -jar toNumArray.jar alphabet_lower

clean:
	rm -f toByteArray.jar
	rm -f include/webpage.h
	rm -f toNumArray.jar
	pio run -t clean
