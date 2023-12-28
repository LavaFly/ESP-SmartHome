import java.io.File
import java.io.ByteArrayOutputStream
import java.util.zip.GZIPOutputStream

val charset = Charsets.UTF_8

fun ByteArray.toHexString() : String = joinToString(separator = ",", postfix = "}") {eachByte -> "0x%02X".format(eachByte)}

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
    val fileContent = File(args[0]).readText(Charsets.UTF_8)
    val gzippedFileContent = gzip(fileContent)
    val hexString = gzippedFileContent.toHexString()
    val len = gzippedFileContent.size
    val header = "#include <pgmspace.h>\n#define webpage_gz_len $len\nconst uint8_t webpage_html_gz[] PROGMEM = {\n"
    val finalContent = header + insertNewlines2(hexString, 80)
    val outputFile = File(args[0].substring(0, args[0].length - 3))
    outputFile.writeText(finalContent)
}
