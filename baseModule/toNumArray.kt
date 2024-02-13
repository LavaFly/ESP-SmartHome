import java.io.File

val charset = Charsets.UTF_8

fun formatSingleLine(line: String): String {
    // 0101 -> {0,1,0,1}
    val listOfCharsInLine = line.toCharArray()
    val formatedLine = listOfCharsInLine.joinToString(prefix = "{", separator = ",", postfix = "}") { it.toString() }
    return formatedLine
}

// A
// 0101
// 1010
// -> {{0,1,0,1},{1,0,1,0}}, // A

fun main(args: Array<String>){
    val fileContent = File(args[0]).readText(charset)
    val filename = args[0].substringBeforeLast(".")
    val listOfChars = fileContent.split("\n\n")
    var formatedFilecontent = ""
    var formatedContent = ""

    for(i in listOfChars){
        val listOfLines = i.split("\n")
        val letter = listOfLines[0]

        val formattedSymbol: MutableList<String> = ArrayList()
        for(j in listOfLines){
            if(j.matches("\\d+".toRegex())){
                val formatedLine = formatSingleLine(j)
                formattedSymbol.add(formatedLine)
            }
        }
        var formattedSymbolString = formattedSymbol.joinToString(prefix = "{", separator = ",", postfix = "}, // $letter") { it.toString() }
        formattedSymbolString = "\n\t" + formattedSymbolString
        formatedContent += formattedSymbolString
    }
    val x = 0
    val y = 0
    val z = 0

    val header = "const uint_8 $filename [$x][$y][$z] = {"

    val comlpeteFile = header + formatedContent + "\n};"
    val outputFile = File(filename.plus(".h"))
    outputFile.writeText(comlpeteFile)
}
