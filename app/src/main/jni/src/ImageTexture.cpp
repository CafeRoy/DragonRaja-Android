#include "StdAfx.h"
#include "ImageTexture.h"
// 建構函式實作
ImageTexture::ImageTexture(SDL_Renderer* renderer) {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  mRenderer = renderer;
  mSurface = NULL;
  lpScreenSurface = NULL;
  file_data.clear();
}

// 解構函式實作
ImageTexture::~ImageTexture() {
  // 確保物件銷毀時釋放紋理
  free();
}

// 載入圖片實作
bool ImageTexture::loadFromFile(const std::string& path) {
  // 載入前先釋放舊紋理 (如果有)
  free();
  std::ifstream infile(path, std::ios::binary | std::ios::ate); // 以二進位模式開啟，並將讀取指針移到檔案結尾

if (infile.is_open()) {
    std::streamsize size = infile.tellg();  // 獲取檔案大小
    infile.seekg(0, std::ios::beg);         // 將讀取指針移回檔案開頭

    file_data.resize(size);  // 調整緩衝區大小以容納整個檔案
    // 讀取整個檔案到緩衝區中
    if (infile.read(file_data.data(), size)) {
      SDL_Log(u8"成功讀取檔案，大小為 %lld 位元組", size);
    }
    infile.close();

    if (size > 25 * ONEKB) {
      SDL_Log(u8"讀取的檔案大小超過 25KB，可能會影響效能");
      return false;
    }
  }
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    SDL_Log("Unable to load image %s! IMG_Error: %s\n", path.c_str(),
           IMG_GetError());
    return false;
  } else {
    // 轉換 surface 為適合顯示的格式
    // 保存 surface 以供 cpu render 使用
    mSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGB565, 0);

    // 釋放原始載入的 surface
    SDL_FreeSurface(loadedSurface);

    if (mSurface == NULL) {
      SDL_Log("Unable to convert loaded surface to display format! SDL_Error: %s\n", SDL_GetError());
      return false;
    }
    // 取得圖片尺寸
    mWidth = mSurface->w;
    mHeight = mSurface->h;
    // 將 surface 轉換為 texture
    mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
    if (mTexture == NULL) {
      SDL_Log("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(),
             SDL_GetError());
    } else {
      SDL_Log("loadFromFile %s sucessful", path.c_str());
    }
  }

  // 回傳載入結果
  return mTexture != NULL;
}

// 渲染圖片實作
void ImageTexture::render(int x, int y) {
  // 設定紋理的渲染目標矩形
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};
  // 渲染紋理
  SDL_RenderCopy(mRenderer, mTexture, NULL, &renderQuad);
}

void ImageTexture::cpuRender(int x, int y) {
    
  if (mSurface == nullptr) {
    SDL_Log("Image Surface is null in cpuRender!");
    return;
  }
  if (lpScreenSurface == nullptr) {
      SDL_Log("lpScreenSurface is null in cpuRender!");
    return;
  }
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_BlitSurface(mSurface, NULL, lpScreenSurface, &renderQuad);
}

// 私有方法：釋放紋理
void ImageTexture::free() {
  if (mTexture != NULL) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }

  if (mSurface) {
    SDL_FreeSurface(mSurface);
    mSurface = NULL;
  }
}