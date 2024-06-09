import java.io.File
import java.io.ByteArrayOutputStream
import java.util.zip.GZIPOutputStream

val charset = Charsets.UTF_8

fun ByteArray.toHexString() : String = joinToString(separator = ",", postfix = "};") {eachByte -> "0x%02X".format(eachByte)}

fun gzip(content: String): ByteArray {
    val byteArray = ByteArrayOutputStream()
    GZIPOutputStream(byteArray).bufferedWriter(charset).use { it.write(content) }
    return byteArray.toByteArray()
}

fun insertNewlines2(input: String, lineLength: Int): String {
    val stringBuilder = StringBuilder()
    for (i in input.indices) {
        stringBuilder.append(input[i])
        if ((i + 1) % lineLength == 0) {
            stringBuilder.append('\n')
        }
    }
    return stringBuilder.toString()
}

fun main(args: Array<String>) {
    val completeFileName = args[0]
    val fileName = completeFileName.substringAfterLast("/").substringBeforeLast(".")
    val fileExtension = completeFileName.substring(completeFileName.indexOf(".") + 1, completeFileName.length)

    val fileContent = File(completeFileName).readText(Charsets.UTF_8)
    val gzippedFileContent = gzip(fileContent)
    val hexString = gzippedFileContent.toHexString()

    val len = gzippedFileContent.size

    val include = "#include <pgmspace.h>\n"
    val define = "#define ${fileName + "_" + fileExtension + "_gz_len"} $len\nconst uint8_t ${fileName + "_" + fileExtension + "_gz[] PROGMEM = {\n"}"
    val header = include + define

    val finalContent = header + insertNewlines2(hexString, 80)

    val outputFileName = fileName + "_" + fileExtension + ".h"
    val outputFile = File("include/" + outputFileName)
    outputFile.writeText(finalContent)
}
