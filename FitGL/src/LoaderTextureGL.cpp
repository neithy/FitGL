#include "LoaderGL.h"
#include "Loader.h"
#include "Image.h"
#include "geGL/StaticCalls.h"

using namespace fit;
using namespace ge::gl;

TextureShared Loader::texture(std::string const & fileName, bool generateMipmap,
  GLenum filterMin, GLenum filterMag, GLenum wrapR, GLenum wrapS) {
  auto img = image(fileName);
  auto tex = std::make_shared<Texture>(GL_TEXTURE_2D,GL_RGBA,0,img->width,img->height);
  tex->setData2D(img->data, GL_BGRA, GL_UNSIGNED_BYTE);
  if (generateMipmap)tex->generateMipmap();
  tex->texParameteri(GL_TEXTURE_MIN_FILTER, filterMin);
  tex->texParameteri(GL_TEXTURE_MAG_FILTER, filterMag);
  tex->texParameteri(GL_TEXTURE_WRAP_R, wrapR);
  tex->texParameteri(GL_TEXTURE_WRAP_S, wrapS);
  return tex;
}

TextureShared Loader::cubemap(std::string const(&fileNames)[6], bool generateMipmap, GLenum filterMin, GLenum filterMag, GLenum wrapR, GLenum wrapS) {
  ImageShared img[6];
  for (int i = 0; i < 6; i++) {
    img[i]=image(fileNames[i]);    
  }
  auto tex = std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, GL_RGBA, 0, img[0]->width, img[0]->height);
  for (int i = 0; i < 6; i++) {
    tex->setData2D(img[i]->data, GL_BGRA, GL_UNSIGNED_BYTE, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
  }
  if (generateMipmap)tex->generateMipmap();
  tex->texParameteri(GL_TEXTURE_MIN_FILTER, filterMin);
  tex->texParameteri(GL_TEXTURE_MAG_FILTER, filterMag);
  tex->texParameteri(GL_TEXTURE_WRAP_R, wrapR);
  tex->texParameteri(GL_TEXTURE_WRAP_S, wrapS);
  return tex;
}