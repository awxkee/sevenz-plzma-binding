# 7z android bindings

Library provides simple interface to work with 7z files

```kotlin
lifecycleScope.launch(Dispatchers.IO) {
    val sourceFiles = listOf<String>("test_1.jpg", "test_4.jpg", "test_5.jpg")
    val compressingFiles = listOf<File>().toMutableList()
    sourceFiles.forEach { src ->
        val sourceStream = assets.open(src)
        sourceStream.use {
            val buf = it.source().buffer()
            val dstFile = File(filesDir, src)
            compressingFiles.add(dstFile)
            val destStream = FileOutputStream(dstFile)
            destStream.use { fos ->
                val dstBuf = fos.sink().buffer()
                dstBuf.writeAll(buf)
                dstBuf.flush()
            }
        }
    }
    val archive = File(filesDir, "archive.7z")
    SevenLzma().compress(archive.path, compressingFiles.map { it.path }, "test", 9)
    val pfd1 = ParcelFileDescriptor.open(archive, ParcelFileDescriptor.MODE_READ_ONLY)
    pfd1.use { pfd ->
        assert(SevenLzma().isPasswordProtected(pfd))
        assert(!SevenLzma().validatePassword(pfd, "kyky"))
        assert(SevenLzma().validatePassword(pfd, "test"))
        File(filesDir, "dst").deleteRecursively()
        SevenLzma().extract(pfd, File(filesDir, "dst").path, "test")
    }
}
```

```groovy
implementation "implementation 'com.github.awxkee:sevenz-plzma-binding:Tag'"
```