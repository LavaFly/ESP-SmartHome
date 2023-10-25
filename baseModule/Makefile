#########################################################################
upload: website
	pio run -e d1_mini

website: include/webpage.html converter
	java -jar toByteArray.jar

converter: toByteArray.kt
	kotlinc toByteArray.kt -include-runtime -d toByteArray.jar

clean:
	rm -f toByteArray.jar
	pio run -t clean
