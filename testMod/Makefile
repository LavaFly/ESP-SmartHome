webpage := include/webpage.html
mock :=	mock_sensor

all: upload

ota:
	pio run -e d1_mini_ota

webpage: toByteArray.jar $(webpage)
	java -jar toByteArray.jar $(webpage)

mock: toByteArray.jar $(mock)
	java -jar toByteArray.jar $(mock)

toByteArray.jar: toByteArray.kt
	kotlinc toByteArray.kt -include-runtime -d toByteArray.jar

compile:
	pio run -e d1_mini

upload:
	pio run -e d1_mini -t upload

webserver:
	python3 -m http.server 8080

characters:
	kotlinc toNumArray.kt -include-runtime -d toNumArray.jar
	java -jar toNumArray.jar alphabet_lower

clean:
	rm -f toByteArray.jar
	rm -f include/webpage.h
	rm -f toNumArray.jar
	pio run -t clean
