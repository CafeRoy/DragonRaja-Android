#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

const int ONEKB = 1024;
class ImageTexture {
 public:
  // 建構函式
  ImageTexture(SDL_Renderer* renderer);

  // 解構函式
  ~ImageTexture();

  // 載入圖片
  bool loadFromFile(const std::string& path);

  // 渲染圖片
  void render(int x, int y);

  //cpu render
  void cpuRender(int x, int y);

  // 設定 cpu渲染 surface
  void setSurface(SDL_Surface* screen_surface) { lpScreenSurface = screen_surface; }

  // 取得圖片寬度
  int getWidth() const { return mWidth; }

  // 取得圖片高度
  int getHeight() const { return mHeight; }

  // 取得surface
  SDL_Surface* getSurface() { return mSurface; }

  //取得texture
  SDL_Texture* getTexture() { return mTexture; }
  std::vector<char> file_data;
 private:
  // 釋放紋理
  void free();

  
  // 類別屬性
  SDL_Texture* mTexture;
  SDL_Surface* mSurface;
  int mWidth;
  int mHeight;
  SDL_Renderer* mRenderer;
  SDL_Surface* lpScreenSurface;
};