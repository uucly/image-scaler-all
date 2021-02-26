package main

import (
	"fmt"
	"github.com/disintegration/imaging"
	"github.com/gin-gonic/gin"
	"github.com/h2non/bimg"
	"net/http"
	"time"
)

//var pngSrc, _ = bimg.Read("/Users/svenweisker/Downloads/big_png_big.png")
//var jpgSrc, _ = bimg.Read("pexels-artem-beliaikin-853199.jpg")

//var pngSrc, _ = imaging.Open("/Users/svenweisker/Downloads/big_png_big.png")
//var src, _ = imaging.Open("pexels-artem-beliaikin-853199.jpg")
//pngSrc, _ := bimg.Read("/Users/svenweisker/Downloads/big_png_big.png")
//src, err := imaging.Open("pexels-artem-beliaikin-853199.jpg")
//image, _ := vips.NewImageFromFile("pexels-artem-beliaikin-853199.jpg")

func main() {
	fmt.Println("### Started ###")
	gin.SetMode(gin.ReleaseMode)
	bimg.Initialize()
	defer bimg.Shutdown()
	r := gin.Default()

	//pngSrc, _ := bimg.Read("big_png_big.png")
	//pngSrc, _ := imaging.Open("big_png_big.png")
	//src, _ := imaging.Open("pexels-artem-beliaikin-853199.jpg")
	//image, _ := vips.NewImageFromFile("pexels-artem-beliaikin-853199.jpg")


	r.GET("/png", func(c *gin.Context) {
		pngSrc, _ := imaging.Open("big_png_big.png")
		// Resize the cropped image to width = 200px preserving the aspect ratio.
		pngSrc = imaging.Resize(pngSrc, 1200, 600, imaging.NearestNeighbor)

		extension, _ := imaging.FormatFromExtension(".png")
		_ = imaging.Encode(c.Writer, pngSrc, extension)
		c.Writer.Header().Add("Content-Disposition", "attachment; filename=bla.png" )
	})

	r.GET("/jpg", func(c *gin.Context) {
		src, _ := imaging.Open("pexels-artem-beliaikin-853199.jpg")
		//src = imaging.Resize(src, 1200, 600, imaging.NearestNeighbor)

		extension, _ := imaging.FormatFromExtension(".jpg")
		_ = imaging.Encode(c.Writer, src, extension)
		c.Writer.Header().Add("Content-Disposition", "attachment; filename=bla.jpg" )
	})

	r.GET("/jpeg", func(c *gin.Context) {

		jpgSrc, _ := bimg.Read("pexels-artem-beliaikin-853199.jpg")
		resizeImage, _ := bimg.Resize(jpgSrc, bimg.Options{Width: 1200})

		c.Writer.Header().Add("Content-Disposition", "attachment; filename=bla.jpg" )
		c.Data(200, "image/jpg", resizeImage)
	})

	r.GET("/peng", func(c *gin.Context) {

		pngSrc, _ := bimg.Read("big_png_big.png")

		resize, err := bimg.Resize(pngSrc, bimg.Options{
			Width: 1200,
		})

		if err != nil {
			_, _ = c.Writer.Write([]byte(fmt.Sprintf("failed to resize %s: %v", "blub", err)))
			c.Writer.WriteHeader(http.StatusInternalServerError)
			return
		}

		c.Writer.Header().Add("Content-Disposition", "attachment; filename=bla.png" )
		c.Data(200, "image/png", resize)

	})

	_ = r.Run() // listen and serve on 0.0.0.0:8080 (for windows "localhost:8080")

}

func makeTimestamp() int64 {
	return time.Now().UnixNano() / int64(time.Millisecond)
}
