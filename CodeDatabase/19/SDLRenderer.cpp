#include "SDLRenderer.h"

#include <algorithm>
#include <iostream>
#include <SDL_image.h>
#include <sstream>

#include "SDLWindow.h"


bool SDLRenderer::init(std::string title, int width, int height) {
    if (renderer_) {
        std::cerr << "SDL renderer already exists" << "\n";
        return false;
    }

    if (!window.create(title, width, height)) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << "\n";
        return false;
    }

    renderer_ = SDL_CreateRenderer(window.window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << "\n";
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    //SDL_RenderSetLogicalSize(renderer_,320,180);
    //SDL_RenderSetIntegerScale(renderer_, SDL_TRUE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    return true;
}

bool SDLRenderer::load_texture(const std::string &id, const std::string &path) {
    if (renderer_) {
        SDL_Texture *tex = IMG_LoadTexture(renderer_, path.c_str());
        if (!tex) {
            std::cerr << "Failed to load texture " << path << ": " << IMG_GetError() << "\n";
        } else {
            textures_[id] = tex;
            return true;
        }
    }
    return false;
}

bool SDLRenderer::tint_texture(const std::string &id, const Color &tint) const{
    if (!renderer_ || !textures_.contains(id))
        return false;
    SDL_Texture *tex = textures_.at(id);
    SDL_SetTextureColorMod(tex, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(tex, tint.a);
    return true;
}


bool SDLRenderer::draw_texture_centered(const std::string &id, int x, int y, int w, int h) const {
    int tex_w = 0;
    int tex_h = 0;
    if (h <= 0 && textures_.contains(id)) {
        SDL_QueryTexture(textures_.at(id), nullptr, nullptr, &tex_w, &tex_h);
        if (tex_w > 0 && tex_h > 0) {
            float aspect = static_cast<float>(tex_h) / tex_w;
            h = static_cast<int>(w * aspect);
        }
    }

    const int draw_x = x - w / 2;
    const int draw_y = y - h / 2;

    return draw_texture(id, draw_x, draw_y, w, h);
}
bool SDLRenderer::draw_texture(const std::string& id, int x, int y, int w, int h) const {
    if (!renderer_ || !textures_.contains(id))
        return false;

    SDL_Texture* texture = textures_.at(id);
    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &tex_w, &tex_h);

    if (tex_w == 0 || tex_h == 0)
        return false;

    if (h <= 0) {
        const float aspect = static_cast<float>(tex_h) / static_cast<float>(tex_w);
        h = static_cast<int>(w * aspect);
    }

    const SDL_Rect dest{x, y, w, h};
    SDL_RenderCopy(renderer_, texture, nullptr, &dest);
    return true;
}


bool SDLRenderer::load_font(const std::string &id, const std::string &path, const int size) {
    if (renderer_) {
        TTF_Font *font = TTF_OpenFont(path.c_str(), size);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
            return false;
        }
        fonts_[id] = font;
        return true;
    }
    return false;
}

int SDLRenderer::font_height(const std::string &id) const {
    if (fonts_.contains(id)) {
        const TTF_Font* font = fonts_.at(id);
        return TTF_FontLineSkip(font);
    }
    return 0;
}


bool SDLRenderer::draw_text(const std::string &text,const int x, const int y, const Color color, const std::string &font_id, const TextAlign align) const{
    if (!renderer_ || !fonts_.contains(font_id)) return false;

    TTF_Font* font = fonts_.at(font_id);
    const int lineSkip = TTF_FontLineSkip(font);

    std::stringstream ss(text);
    std::string line;
    int rowY = y;
    int drawn = 0;

    while (std::getline(ss, line, '\n')) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty()) {
            rowY += lineSkip;
            continue;
        }

        SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), {color.r, color.g, color.b, color.a});
        if (!surface) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << "\n";
            return false;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture) {
            std::cerr << "Failed to create texture from text surface: " << SDL_GetError() << "\n";
            SDL_FreeSurface(surface);
            return false;
        }

        int alignedX = x;
        if (align == TextAlign::Center) {
            alignedX = x - surface->w / 2;
        } else if (align == TextAlign::Right) {
            alignedX = x - surface->w;
        }

        SDL_Rect dst = { alignedX, rowY, surface->w, surface->h };
        SDL_RenderCopy(renderer_, texture, nullptr, &dst);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        rowY += lineSkip;
        drawn++;
    }

    return true;
}

int SDLRenderer::text_height(const std::string &text, const std::string &font_id) const {
    const int lines = std::count(text.begin(), text.end(), '\n') + 1;
    return lines * font_height(font_id);
}

int SDLRenderer::text_width(const std::string &text, const std::string &font_id) const {
    if (!fonts_.contains(font_id)) return 0;
    TTF_Font* font = fonts_.at(font_id);

    int maxWidth = 0;
    std::stringstream ss(text);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        int w = 0, h = 0;
        if (TTF_SizeText(font, line.c_str(), &w, &h) == 0) {
            maxWidth = std::max(maxWidth, w);
        }
    }

    return maxWidth;
}



void SDLRenderer::clear() const {
    if (renderer_) {
        SDL_RenderClear(renderer_);
    }
}

void SDLRenderer::present() {
    if (renderer_) {
        for (auto& [depth, func] : depth_queue_)
            func(*this);
        SDL_RenderPresent(renderer_);
    }
    depth_queue_.clear();
}

void SDLRenderer::set_draw_color(const Color color) const {
    if (renderer_)
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

void SDLRenderer::draw_rect(const int x, const int y, const int w, const int h, const bool filled) const {
    if (renderer_) {
        SDL_Rect rect = {x, y, w, h};
        if (filled)
            SDL_RenderFillRect(renderer_, &rect);
        else
            SDL_RenderDrawRect(renderer_, &rect);
    }
}

void SDLRenderer::draw_line(const int x1, const int y1, const int x2, const int y2) const {
    if (renderer_)
        SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}

void SDLRenderer::depth_render(double depth, std::function<void(const IRenderer&)> render_func) {
    depth_queue_.emplace(depth, std::move(render_func));
}


void SDLRenderer::destroy() {
    for (auto &[_, tex]: textures_) {
        if (tex) SDL_DestroyTexture(tex);
    }
    textures_.clear();
    for (auto &[_, font]: fonts_) {
        if (font) TTF_CloseFont(font);
    }
    fonts_.clear();

    depth_queue_.clear();

    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    window.destroy();
    TTF_Quit();
    SDL_Quit();
}

SDLRenderer::~SDLRenderer() {
    SDLRenderer::destroy();
}

int SDLRenderer::window_width() const {
    return window.width();
}
int SDLRenderer::window_height() const {
    return window.height();
}
