import java.io.File

val charset = Charsets.UTF_8

fun formatSingleLine(line: String): String {
    // 0101 -> {0,1,0,1}
    val listOfCharsInLine = line.toCharArray()
    val formatedLine = listOfCharsInLine.joinToString(prefix = "{", separator = ",", postfix = "}") { it.toString() }
    return formatedLine
}

fun main(args: Array<String>){
    val fileContent = File(args[0]).readText(charset)
    val filename = args[0].substringBeforeLast(".")
    val listOfChars = fileContent.split("\n\n")
    var formatedFilecontent = ""
    var formatedContent = ""
    var x = 0
    var y = 0
    var z = 0

    for(i in listOfChars){
        val listOfLines = i.split("\n")
        val letter = listOfLines[0]
        val formattedSymbol: MutableList<String> = ArrayList()

        for(j in listOfLines){
            if(j.matches("\\d+".toRegex())){
                z = j.length
                val formatedLine = formatSingleLine(j)
                formattedSymbol.add(formatedLine)
                y++
            }
        }
        x++
        var formattedSymbolString = formattedSymbol.joinToString(prefix = "{", separator = ",", postfix = "}, // $letter") { it.toString() }
        formattedSymbolString = "\n\t" + formattedSymbolString
        formatedContent += formattedSymbolString

    }

    y /= 2
    print("x = $x = 2, y = $y = 6, z = $z = 5")
    val header = "const uint8_t ${filename}[$x][$y][$z] = {"

    val comlpeteFile = header + formatedContent + "\n};"
    val outputFile = File(filename.plus(".h"))
    outputFile.writeText(comlpeteFile)
}
