#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use]
extern crate lazy_static;
#[macro_use]
extern crate rocket;

use image::ImageFormat;
use libvips::{ops, VipsApp, VipsImage};
use rocket::http::ContentType;
use rocket::response::Content;
use libvips::ops::ThumbnailImageOptions;

pub mod a_mod {
    use image::DynamicImage;
    use libvips::VipsImage;
    use std::borrow::Borrow;

    lazy_static! {
        pub static ref PERIPHERALS_JPG: DynamicImage = image::open("/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg").expect("");
        pub static ref PERIPHERALS_PNG: DynamicImage = image::open("/Users/svenweisker/Downloads/big_png_big.png").expect("");
    }
}

#[get("/jpeg")]
fn jpg() -> Content<Vec<u8>> {
    let dynamic_image = image::open("pexels-artem-beliaikin-853199.jpg").expect("");

    let mut bytes: Vec<u8> = Vec::new();
    dynamic_image
        .thumbnail(1200, 600)
        .write_to(&mut bytes, image::ImageOutputFormat::from(ImageFormat::Jpeg)).expect("");

    Content(ContentType::JPEG, bytes)
}

#[get("/jpg")]
fn jpg_vips() -> Content<Vec<u8>> {
    let new_image = VipsImage::new_from_file("pexels-artem-beliaikin-853199.jpg").expect("");
    let resized_image = ops::thumbnail_image(&new_image, 1200).expect("");

    let bytes = ops::jpegsave_buffer(&resized_image).expect("");
    Content(ContentType::JPEG, bytes)
}

#[get("/peng")]
fn png() -> Content<Vec<u8>> {
    let dynamic_image = &*a_mod::PERIPHERALS_PNG;
    //let dynamic_image = image::open(&"/Users/svenweisker/Downloads/big_png_big.png").expect("");
    let mut bytes: Vec<u8> = Vec::new();
    dynamic_image
        .thumbnail(1200, 600)
        .write_to(&mut bytes, image::ImageOutputFormat::from(ImageFormat::Png)).expect("");
    Content(ContentType::PNG, bytes)
}

#[get("/png")]
fn png_vips() -> Content<Vec<u8>> {
    let new_image = VipsImage::new_from_file(&"/Users/svenweisker/Downloads/big_png_big.png").expect("");
    let resized_image = ops::thumbnail_image(&new_image, 1200).expect("");

    let bytes = ops::pngsave_buffer(&resized_image).expect("");
    Content(ContentType::PNG, bytes)
}

fn main() {

    // let new_image = VipsImage::new_from_file(&"/Users/svenweisker/Downloads/pexels-artem-beliaikin-853199.jpg").expect("");
    // ThumbnailImageOptions::default();
    // let options = ops::ThumbnailImageOptions {
    //     height: 600,
    //     ..ops::ThumbnailImageOptions::default()
    // };
    // let resized_image = ops::thumbnail_image_with_opts(&new_image, 1200,&options).expect("");
    // resized_image.image_write_to_file("output.jpg");
    //let bytes = ops::jpegsave_buffer(&resized_image).expect("");
    //let _app = VipsApp::new("Test Libvips", false).expect("Cannot initialize libvips");
    rocket::ignite().mount("/", routes![jpg,jpg_vips,png,png_vips]).launch();
}

