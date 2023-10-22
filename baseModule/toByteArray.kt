import java.io.File
import java.io.ByteArrayOutputStream
import java.util.zip.GZIPOutputStream

val charset = Charsets.UTF_8

fun ByteArray.toHex() : String = joinToString(separator = "") {eachByte -> "%02X".format(eachByte)}

fun gzip(content: String): ByteArray {
    val byteArray = ByteArrayOutputStream()
    GZIPOutputStream(byteArray).bufferedWriter(charset).use { it.write(content) }
    return byteArray.toByteArray()
}

fun main(args: Array<String>) {

    //val fileContent = File("include/webpage.html").inputStream().readBytes().toHex()
    val fileContent = File("include/webpage.html").readText(charset)
    val gzippedFileContent = gzip(fileContent).toHex()

    println(gzippedFileContent)
}

