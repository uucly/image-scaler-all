package main

import (
	"github.com/davidbyttow/govips/vips"
	"io/ioutil"
)

func main() {

/*	pngSrc, _ := bimg.Read("big_png_big.png")

	resize, _ := bimg.Resize(pngSrc, bimg.Options{
		Width: 1200,
	})

	_ = bimg.Write("resized_big.png", resize)*/

	vips.Startup(nil)
	defer vips.Shutdown()

	image1, _ := vips.NewImageFromFile("pexels-artem-beliaikin-853199.jpg")

	_ = image1.Thumbnail(1200, 600, vips.InterestingNone)
	image1bytes, _, _ := image1.ExportNative()
	_ = ioutil.WriteFile("output.jpg", image1bytes, 0644)
}
