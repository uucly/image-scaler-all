using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Net.Mime;
using Microsoft.AspNetCore.Mvc;
using NetVips;

// For more information on enabling Web API for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace ImageScalerCSharp.Controllers
{
    [ApiController]
    [Route("/")]
    public class ImgeScalerController : ControllerBase
    {
        private ImageScalerService _imageScalerService;

        NetVips.Image vipsImage_png = NetVips.Image.NewFromFile("big_png_big.png");
        NetVips.Image vipsImage_jpg = NetVips.Image.NewFromFile("pexels-artem-beliaikin-853199.jpg");

        public ImgeScalerController(ImageScalerService imageScalerService)
        {
            _imageScalerService = imageScalerService;
        }


        /* [HttpGet("png")]
         [Produces(MediaTypeNames.Image.Jpeg)]
         public IActionResult GetSync()
         {
             using (var memoryStream = new MemoryStream())
             {
                 var image = Image.Thumbnail("big_png_big.png", 1200, 600);
                 image.WriteToStream(memoryStream, ".png");
                 return File(memoryStream.ToArray(), "image/png", "myimage_3.png");
             }
         }*/

        public bool ThumbnailCallback()
        {
            return false;
        }

        [HttpGet("png")]
        [Produces("image/jpg")]
        public IActionResult GetDrawing()
        {
            System.Drawing.Image imgPhoto = System.Drawing.Image.FromFile("pexels-artem-beliaikin-853199.jpg");
            using (var ms = new MemoryStream())
            {
                var image = ResizeImage(imgPhoto, 1200, 600);
                image.Save(ms, image.RawFormat);
                return File(ms.ToArray(), "image/jpg", "image.jpg");
            }
            

           // NetVips.Image vipsImage_png = NetVips.Image.NewFromFile("big_png_big.png");
           // var resizedImage = vipsImage_png.ThumbnailImage(1200, height: 600);
           // return File(resizedImage.WriteToBuffer(".png"), "image/png", "image.png");
        }

        [HttpGet("jpg")]
        [Produces(MediaTypeNames.Image.Jpeg)]
        public IActionResult GetJpg()
        {
            var vipsImage_jpg = NetVips.Image.NewFromFile("pexels-artem-beliaikin-853199.jpg");
            var resizedImage = vipsImage_jpg.ThumbnailImage(1200, height: 600);

            return File(resizedImage.WriteToBuffer(".jpg"), "image/jpg", "pexels-artem-beliaikin-853199.jpg");   
        }

        // POST api/values
        [HttpPost]
        public void Post([FromBody] string value)
        {
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }

        public static Bitmap ResizeImage(System.Drawing.Image image, int width, int height)
{
    var destRect = new Rectangle(0, 0, width, height);
    var destImage = new Bitmap(width, height);

    destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

    using (var graphics = Graphics.FromImage(destImage))
    {
        graphics.CompositingMode = CompositingMode.SourceCopy;
        graphics.CompositingQuality = CompositingQuality.HighQuality;
        graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
        graphics.SmoothingMode = SmoothingMode.HighQuality;
        graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

        using (var wrapMode = new ImageAttributes())
        {
            wrapMode.SetWrapMode(WrapMode.TileFlipXY);
            graphics.DrawImage(image, destRect, 0, 0, image.Width,image.Height, GraphicsUnit.Pixel, wrapMode);
        }
    }

    return destImage;
}
    }
}
