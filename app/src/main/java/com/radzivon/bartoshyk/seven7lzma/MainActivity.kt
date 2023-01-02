package com.radzivon.bartoshyk.seven7lzma

import android.os.Bundle
import com.google.android.material.snackbar.Snackbar
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import androidx.navigation.ui.setupActionBarWithNavController
import android.view.Menu
import android.view.MenuItem
import androidx.lifecycle.lifecycleScope
import com.github.awxkee.sevenzlzma.SevenLzma
import com.radzivon.bartoshyk.seven7lzma.databinding.ActivityMainBinding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import okio.buffer
import okio.sink
import okio.source
import java.io.File
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {

    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        setSupportActionBar(binding.toolbar)

        val navController = findNavController(R.id.nav_host_fragment_content_main)
        appBarConfiguration = AppBarConfiguration(navController.graph)
        setupActionBarWithNavController(navController, appBarConfiguration)

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
            assert(SevenLzma().isPasswordProtected(archive.path))
            assert(!SevenLzma().validatePassword(archive.path, "kyky"))
            assert(SevenLzma().validatePassword(archive.path, "test"))
            File(filesDir, "dst").deleteRecursively()
            SevenLzma().extract(archive.path, File(filesDir, "dst").path, "test")
        }

        binding.fab.setOnClickListener { view ->
            Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                .setAction("Action", null).show()
        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.nav_host_fragment_content_main)
        return navController.navigateUp(appBarConfiguration)
                || super.onSupportNavigateUp()
    }
}