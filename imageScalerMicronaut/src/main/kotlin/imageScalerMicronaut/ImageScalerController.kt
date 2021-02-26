package imageScalerMicronaut

import com.criteo.vips.VipsImage
import com.criteo.vips.enums.VipsImageFormat
import io.micronaut.http.HttpResponse
import io.micronaut.http.MediaType
import io.micronaut.http.annotation.Controller
import io.micronaut.http.annotation.Get
import org.imgscalr.Scalr
import java.io.ByteArrayOutputStream
import java.io.File
import java.nio.file.Files
import javax.imageio.ImageIO


@Controller
class ImageScalerController {

    //val image = ImageIO.read(File("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg"))
    //val png = ImageIO.read(File("/Users/svenweisker/Downloads/big_png_big.png"))


    @Get("/jpg", produces = [MediaType.IMAGE_JPEG])
    fun index(): HttpResponse<ByteArray> {

        val image = ImageIO.read(File("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg"))
         val sizedImage = Scalr.resize(image, Scalr.Method.BALANCED, 1200, 600)
         val baos = ByteArrayOutputStream()
         ImageIO.write(sizedImage, "jpg", baos)
         return HttpResponse.ok(baos.toByteArray()).contentType(MediaType.IMAGE_JPEG)
      //  return HttpResponse.ok(vipsJpg()).contentType(MediaType.IMAGE_JPEG)
    }

    @Get("/png", produces = [MediaType.IMAGE_PNG])
    fun getPNG(): HttpResponse<ByteArray> {

        val png = ImageIO.read(File("/Users/svenweisker/Downloads/big_png_big.png"))
         val sizedImage = Scalr.resize(png, Scalr.Method.BALANCED, 1200, 600)
         val baos = ByteArrayOutputStream()
         ImageIO.write(sizedImage, "png", baos)
         return HttpResponse.ok(baos.toByteArray()).contentType(MediaType.IMAGE_PNG)
      //  return HttpResponse.ok(vips_png()).contentType(MediaType.IMAGE_PNG)
    }

    private fun vipsJpg(): ByteArray {
        val contents: ByteArray = getByteArray("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg")

        VipsImage(contents, contents.size).use { image ->
            image.resize(1200, 600, true)
            return image.writeToArray(VipsImageFormat.JPG, false)
        }
    }

    private fun vips_png(): ByteArray {
        //VipsContext.setLeak(true)
        //val contents: ByteArray = getByteArray("/Users/svenweisker/Downloads/big_png_big.png")

        VipsImage(contents_png, contents_png.size).use { image ->

            image.resize(1200, 600, true)
            return image.writeToArray(VipsImageFormat.PNG, false)
        }
    }

    private fun getByteArray(path: String): ByteArray {
        return Files.readAllBytes(File(path).toPath())
    }

    companion object {
        private fun getByteArray(path: String): ByteArray {
            return Files.readAllBytes(File(path).toPath())
        }

        val contents_jpg: ByteArray = getByteArray("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg")

        val contents_png: ByteArray = getByteArray("/Users/svenweisker/Downloads/big_png_big.png")
    }
}