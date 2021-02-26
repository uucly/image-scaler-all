
#include "opencv2/opencv.hpp"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <vips/vips8>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using namespace std;
using namespace cv;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"

static size_t jpg_size;
static void *jpg_buffer;

static size_t png_size;
static void *png_buffer;

static vips::VImage myimread;
static vips::VImage myimread_png;

static VipsBlob *blob_jpg;
static VipsBlob *blob_png;

static size_t size_jpg;
static void *buf_jpg;

static size_t size_png;
static void *buf_png;

static Mat opencv_jpg;
static Mat opencv_png;

static char* jpeg_char_buffer;
static char* png_char_buffer;

void handle_png_blob(http_request request) {
	std::TRACE(L"\nhandle GET PNG Vips Blob\n");

	auto image = vips::VImage::thumbnail_buffer(blob_png, 1200);

	std::TRACE(L"\nhandle GET PNG Vips Blob\n");

	size_t size;
	void *buf;
	image.write_to_buffer( ".png", &buf, &size);
	
	std::TRACE(L"\nhandle GET PNG Vips Blob\n");

	uchar* bufferPointer = static_cast<uchar*>(buf);
	std::vector<uchar> v(bufferPointer, bufferPointer + size);
	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(v, std::ios_base::in);
	request.reply(status_codes::OK, txtbuf, "image/png");
}

void my_handle_get_png_vips(http_request request)
{
	std::TRACE(L"\nhandle GET PNG Vips\n");
	
	auto image = vips::VImage::thumbnail("/Users/svenweisker/Downloads/big_png_big.png",1200, vips::VImage::option() 
		-> set("height" , 600));

	auto compressImage = image.pngsave_buffer(vips::VImage::option() 
	-> set("palette", true)
	-> set("compression", 9)
	);
	
	uchar* bufferPointer = static_cast<uchar*>(compressImage->area.data);
	std::vector<uchar> v(bufferPointer, bufferPointer + compressImage->area.length);
	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(v, std::ios_base::in);
	vips_thread_shutdown();

	request.reply(status_codes::OK, txtbuf, "image/png");
}

void handle_get_png_vips_uncompress(http_request request)
{
	std::TRACE(L"\nhandle GET PNG Vips\n");

	auto image = vips::VImage::thumbnail("/Users/svenweisker/Downloads/big_png_big.png",1200, vips::VImage::option() 
		-> set("height" , 600));

	size_t size;
	void *buf;
	image.write_to_buffer( ".png", &buf, &size);
	
	uchar* bufferPointer = static_cast<uchar*>(buf);
	std::vector<uchar> v(bufferPointer, bufferPointer + size);
	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(v, std::ios_base::in);

	request.reply(status_codes::OK, txtbuf, "image/png");
}

void handle_get_jpg_vips_uncompress(http_request request)
{
	std::TRACE(L"\nhandle GET JPG Vips\n");

	auto image = vips::VImage::thumbnail("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg",1200, vips::VImage::option() 
		-> set("height" , 600));

	size_t size;
	void *buf;
	image.write_to_buffer( ".jpg", &buf, &size);
	
	uchar* bufferPointer = static_cast<uchar*>(buf);
	std::vector<uchar> v(bufferPointer, bufferPointer + size);
	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(v, std::ios_base::in);

	request.reply(status_codes::OK, txtbuf, "image/jpg");
}

void my_handle_get_png_vips_resize(http_request request)
{
	VipsImage *out;
	vips_thumbnail_buffer(buf_png, size_png, &out, 1200, NULL);

	size_t size;
	void *buf;
	int i = vips_image_write_to_buffer(out, ".png", &buf, &size, NULL);

	uchar* bufferPointer = static_cast<uchar*>(buf);
	std::vector<uchar> v(bufferPointer, bufferPointer + size);
	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(v, std::ios_base::in);

	vips_thread_shutdown();
	request.reply(status_codes::OK, txtbuf, "image/png");
}

void my_handle_get_png_vips_resize_resize(http_request request)
{	
	auto resized = myimread_png.resize(0.3, vips::VImage::option() ->set( "vscale", 0.3 ));

	size_t size;
	void *buf;
	resized.write_to_buffer(".png", &buf, &size);
	
	uchar* bufferPointer = static_cast<uchar*>(buf);
	std::vector<uchar> v(bufferPointer, bufferPointer + size);
	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(v, std::ios_base::in);

	vips_thread_shutdown();
	request.reply(status_codes::OK, txtbuf, "image/png");
}

void my_handle_get_jpg_vips_resize_resize(http_request request)
{
	std::TRACE(L"\nhandle GET JPG RAM Vips\n");

	auto resized = myimread.resize(0.2, vips::VImage::option() ->set( "vscale", 0.15 ));

	size_t size;
	void *buf;
	resized.write_to_buffer(".jpg", &buf, &size);
	
	uchar* bufferPointer = static_cast<uchar*>(buf);
	std::vector<uchar> v(bufferPointer, bufferPointer + size);
	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(v, std::ios_base::in);

	vips_thread_shutdown();
	request.reply(status_codes::OK, txtbuf, "image/jpg");
}

void my_handle_get_opencv_jpg(http_request request)
{
	std::TRACE(L"\nhandle GET\n");

	/*pplx::create_task([=]() -> std::tuple<bool, std::vector<uchar>> {    
			std::TRACE(L"\nhandle GET PNG Vips in Thread\n");

			Mat out;
			resize(opencv_jpg, out, Size(1200,600));
			vector<uchar> outputvector;
			//auto lauft = haveImageReader("C:/Users/sven/Pictures/dodalens2.jpg");
			imencode(".jpeg", out, outputvector);
			return std::make_tuple(true, outputvector);
		})
        .then([=](pplx::task<std::tuple<bool, std::vector<uchar>>> resultTsk) {
             try {
                auto result = resultTsk.get();     
				concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(std::get<1>(result), std::ios_base::in);
                request.reply(status_codes::OK, txtbuf,"image/png");
            }
            catch(std::exception) {
                request.reply(status_codes::Unauthorized);
            }
        });*/

	//Mat opencv_jpg = imread("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg");

	Mat out;
	resize(opencv_jpg, out, Size(1200,600));
	vector<uchar> outputvector;
	//auto lauft = haveImageReader("C:/Users/sven/Pictures/dodalens2.jpg");
	imencode(".jpeg", out, outputvector);
	//imencode(".jpg", myimread, outputvector);
	//bufferstream input_stream = bufferstream(outputvector[0], outputvector.size());

	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(outputvector,std::ios_base::in);
	request.reply(status_codes::OK, txtbuf, "image/jpg");
	//request.reply(status_codes::OK);

}

void handle_get_opencv_png(http_request request)
{
	std::TRACE(L"\nhandle GET\n");

	Mat out;
	resize(opencv_png, out, Size(1200,600));

	vector<uchar> outputvector;
	imencode(".png", out, outputvector);

	concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(outputvector,std::ios_base::in);
	request.reply(status_codes::OK, txtbuf, "image/png");
}

void handle_asnyc_jpg(http_request message) {
	std::TRACE(L"\nhandle GET JPG Vips\n");
	pplx::create_task([=]() -> std::tuple<bool, std::vector<uchar>> {    
			std::TRACE(L"\nhandle GET JPG Vips in Thread\n");

			auto image = vips::VImage::thumbnail("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg",1200, vips::VImage::option() 
				-> set("height" , 600)
				);

			size_t size;
			void *buf;
			image.write_to_buffer( ".jpg", &buf, &size);
			
			uchar* bufferPointer = static_cast<uchar*>(buf);
			std::vector<uchar> v(bufferPointer, bufferPointer + size);        
			return std::make_tuple(true, v);
		})
        .then([=](pplx::task<std::tuple<bool, std::vector<uchar>>> resultTsk) {
             try {
                auto result = resultTsk.get();     
				concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(std::get<1>(result), std::ios_base::in);
                message.reply(status_codes::OK, txtbuf, "image/jpg");
            }
            catch(std::exception) {
                message.reply(status_codes::Unauthorized);
            }
        });
}

void handle_asnyc_png(http_request message) {
	std::TRACE(L"\nhandle GET PNG Vips\n");
	pplx::create_task([=]() -> std::tuple<bool, std::vector<uchar>> {    
			std::TRACE(L"\nhandle GET PNG Vips in Thread\n");

			auto image = vips::VImage::thumbnail("/Users/svenweisker/Downloads/big_png_big.png",1200, vips::VImage::option() 
				-> set("height" , 600)
				);

			size_t size;
			void *buf;
			image.write_to_buffer( ".png", &buf, &size);
			
			uchar* bufferPointer = static_cast<uchar*>(buf);
			std::vector<uchar> v(bufferPointer, bufferPointer + size);        
			return std::make_tuple(true, v);
		})
        .then([=](pplx::task<std::tuple<bool, std::vector<uchar>>> resultTsk) {
             try {
                auto result = resultTsk.get();     
				concurrency::streams::container_stream<std::vector<uchar>>::buffer_type txtbuf(std::get<1>(result), std::ios_base::in);
                message.reply(status_codes::OK, txtbuf,"image/png");
            }
            catch(std::exception) {
                message.reply(status_codes::Unauthorized);
            }
        });
}

int main(int argc, char **argv)
{
	vips_init(argv[0]);


	myimread = vips::VImage::new_from_file("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg");
	myimread_png = vips::VImage::new_from_file("/Users/svenweisker/Downloads/big_png_big.png");

	blob_jpg = myimread.jpegsave_buffer();
	blob_png = myimread_png.pngsave_buffer();

	myimread.write_to_buffer( ".jpg", &buf_jpg, &size_jpg);
	myimread_png.write_to_buffer( ".png", &buf_png, &size_png);

	jpeg_char_buffer = static_cast<char*>(buf_jpg);
	png_char_buffer = static_cast<char*>(buf_png);

	http_listener listener(http::uri("http://localhost:8080/jpg"));
	http_listener listener_png(http::uri("http://localhost:8080/png"));

//	listener.support(methods::GET, handle_get_jpg_vips_uncompress);
//	listener_png.support(methods::GET, handle_get_png_vips_uncompress);
	listener.support(methods::GET, my_handle_get_opencv_jpg);
	listener_png.support(methods::GET, handle_get_opencv_png);

	opencv_jpg = imread("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg");
	opencv_png = imread("/Users/svenweisker/Downloads/big_png_big.png");

	try
	{
		listener
			.open()
			.then([&listener]() {TRACE(L"\nstarting to listen\n"); })
			.wait();

		listener_png
			.open()
			.then([&listener_png]() {TRACE(L"\nstarting to listen PNG\n"); })
			.wait();

		while (true);
	}
	catch (exception const& e)
	{
		wcout << e.what() << endl;
	}

	vips_shutdown();
	return 1;
}

/*int main(int argc, char **argv)
{
	vips_init(argv[0]);

	auto image = vips::VImage::thumbnail("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg",1200, 
	vips::VImage::option() -> set("height", 600));
          
	image.write_to_file("/Users/svenweisker/private_project/imageScalerGo/vips.jpg");
	vips_shutdown();
	return 0;
}*/