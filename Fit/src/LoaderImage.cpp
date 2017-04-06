#include "Loader.h"
#include "Image.h"
#include <fstream>

#ifdef USE_FREEIMAGE
#include <FreeImagePlus.h>
#endif

using namespace fit;

bool fit::Loader::generateMissingImage = true;

ImageShared Loader::image(std::string const& fileName) {
#ifdef USE_FREEIMAGE
  fipImage fimg;
  if (!fimg.load(fileName.c_str())) {
    std::string er = "File not found or unknown format: " + fileName;
    if (generateMissingImage) {
      std::cerr << er << "\n";
      return generateImage();
    } else {
      throw std::runtime_error(er.c_str());
      return nullptr;
    }
  }
  fimg.convertTo32Bits();
  auto img = std::make_shared<Image>();
  img->height = fimg.getHeight();
  img->width = fimg.getWidth();
  img->data = new unsigned int[img->height * img->width];

  memcpy(img->data, fimg.accessPixels(), sizeof(int) * img->width * img->height);
  return img;
#else
  // generate random color checkboard
  return generateImage();
#endif
}

ImageShared Loader::generateImage(int w, int h) {
  auto img = std::make_shared<Image>();
  img->width = w;
  img->height = h;
  img->data = new unsigned int[w * h];
  unsigned int colors[] = {
    0xFF000000,
    0xFFFF0000,
    0xFF00FF00,
    0xFF0000FF,
    0xFFFFFF00,
    0xFFFF00FF,
    0xFF00FFFF,
    0xFFFFFFFF
  };
  int c1 = rand() % 8;
  int c2 = rand() % 8;
  if (c2 == c1)c2 = (c2 + 1) % 8;
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      if (((x >> 4) & 1) == ((y >> 4) & 1))
        img->data[y * w + x] = colors[c1];
      else
        img->data[y * w + x] = colors[c2];
    }
  return img;
}
