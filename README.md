# 7z android bindings

Library provides simple interface to work with 7z files

```kotlin
val archive = File(filesDir, "archive.7z")
SevenLzma().compress(archive.path, compressingFiles.map { it.path }, "test", 9)
assert(SevenLzma().isPasswordProtected(archive.path))
assert(!SevenLzma().validatePassword(archive.path, "kyky"))
assert(SevenLzma().validatePassword(archive.path, "test"))
File(filesDir, "dst").deleteRecursively()
SevenLzma().extract(archive.path, File(filesDir, "dst").path, "test")
```

```groovy
implementation "implementation 'com.github.awxkee:sevenz-plzma-binding:Tag'"
```