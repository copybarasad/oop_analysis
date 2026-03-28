#include "TextureGenerator.h"
#include <iostream>
#include <cmath>
#include <cstdint>

void TextureGenerator::loadAllTextures(std::map<std::string, sf::Texture>& textures) {
    std::map<char, sf::Color> p;
    p['.'] = sf::Color::Transparent;
    p['D'] = sf::Color(20, 20, 25); p['d'] = sf::Color(30, 30, 35);    
    p['G'] = sf::Color(15, 60, 15); p['g'] = sf::Color(25, 80, 25); p['h'] = sf::Color(10, 40, 10);    
    p['V'] = sf::Color(100, 255, 100); p['v'] = sf::Color(50, 200, 50);
    p['B'] = sf::Color(10, 10, 12); p['b'] = sf::Color(40, 40, 45); p['K'] = sf::Color(0, 0, 0);       
    p['W'] = sf::Color(20, 60, 180); p['w'] = sf::Color(40, 90, 220);   
    p['M'] = sf::Color(20, 20, 40); p['L'] = sf::Color(200, 200, 150); 
    p['c'] = sf::Color(50, 50, 60); p['C'] = sf::Color(80, 80, 100);  
    p['w'] = sf::Color(255, 255, 0);  
    
    p['R'] = sf::Color(220, 20, 20); p['Y'] = sf::Color(255, 215, 0); p['P'] = sf::Color(148, 0, 211);   
    p['A'] = sf::Color(0, 255, 255); p['S'] = sf::Color(100, 100, 100); p['s'] = sf::Color(160, 160, 160); 
    p['F'] = sf::Color(255, 224, 189); 
    
    p['T'] = sf::Color(101, 67, 33);  
    p['t'] = sf::Color(139, 69, 19);  
    
    p['k'] = sf::Color(50, 20, 20); p['O'] = sf::Color(255, 165, 0); 
    p['#'] = sf::Color::White;

        // --- MENU BACKGROUND (PIXEL ART LANDSCAPE) ---
    {
        sf::Image img;
        unsigned int w = 800, h = 600; 
        img.resize({w, h}, sf::Color::Black);
        
        // 1. НЕБО (Градиент от ночи к закату)
        for(unsigned int y=0; y<h; ++y) {
            float ratio = (float)y / h;
            for(unsigned int x=0; x<w; ++x) {
                sf::Color sky;
                if (ratio < 0.5f) { 
                    sky = sf::Color(10, 5, 30); // Глубокий космос
                    sky.r += (int)(ratio * 40);
                } else { 
                    sky = sf::Color(60, 20, 60); // Фиолетовый горизонт
                    sky.r += (int)(ratio * 100);
                    sky.g += (int)(ratio * 50);
                }
                // Звезды
                if (y < h/2 && rand() % 500 == 0) sky = sf::Color(255, 255, 200);
                img.setPixel({x, y}, sky);
            }
        }

        // 2. ЛУНА (Большая, желтая)
        int moonX = w/2, moonY = h/3, moonR = 60;
        for(int y=0; y<h; ++y) {
            for(int x=0; x<w; ++x) {
                int dx = x - moonX; int dy = y - moonY;
                if (dx*dx + dy*dy < moonR*moonR) {
                    img.setPixel({(unsigned int)x, (unsigned int)y}, sf::Color(255, 240, 200));
                }
            }
        }

        // 3. ГОРЫ (Процедурная генерация)
        int mountainHeight[800];
        float noise = 0;
        // Генерируем высоту гор
        for(int x=0; x<(int)w; ++x) {
            noise += (rand()%10 - 4.5f) * 0.5f;
            mountainHeight[x] = (int)(h - 150 + noise + std::sin(x * 0.01f) * 80);
        }
        // Рисуем горы
        for(int x=0; x<(int)w; ++x) {
            for(int y=mountainHeight[x]; y<(int)h; ++y) {
                // Цвет гор (темно-фиолетовый с затемнением к низу)
                sf::Color mCol = (x % 4 == 0) ? sf::Color(20, 10, 30) : sf::Color(30, 15, 45);
                img.setPixel({(unsigned int)x, (unsigned int)y}, mCol);
            }
        }

        sf::Texture tex;
        if(tex.loadFromImage(img)) {
            textures["title_bg"] = tex;
            textures["menu_bg"] = tex;
            textures["background"] = tex;
            textures["menu_art"] = tex; // <--- ВАЖНО: Именно это имя ищет меню!
        }
    }

    // --- UI GLOW ---
    {
        sf::Image img; unsigned int size = 64; img.resize({size, size}, sf::Color::Transparent);
        float center = size / 2.0f; float radius = size / 2.0f;
        for(unsigned int y=0; y<size; ++y) {
            for(unsigned int x=0; x<size; ++x) {
                float dx = (float)x - center; float dy = (float)y - center;
                float dist = std::sqrt(dx*dx + dy*dy);
                if(dist < radius) {
                    float alpha = 1.0f - (dist / radius); alpha = std::pow(alpha, 2.0f);
                    img.setPixel({x, y}, sf::Color(255, 255, 255, static_cast<std::uint8_t>(alpha * 255)));
                }
            }
        }
        sf::Texture tex; if(tex.loadFromImage(img)) { tex.setSmooth(true); textures["ui_glow"] = tex; }
    }

    // --- ICONS & ASSETS ---
    createTexture(textures, "spell_trap", 16, 16, { "......", ".S..S.", "SSSSSS", "SSSSSS", "SSSSSS", "......" }, p);
    createTexture(textures, "spell_ally", 16, 16, { ".GGGG.", ".GGGG.", "GGGGGG", "G.GG.G", "G....G", "G....G" }, p);
    
    // New Thorns Texture (Spikes)
    createTexture(textures, "thorns.png", 16, 16, { 
        "................", ". .  .   . . . .", " . .. . . . . . ", ".A.AA.A.A.A.AA.A", "KAKKAKKAKKAKKAKK", "KKKKKKKKKKKKKKKK", 
        "................", ". .  .   . . . .", " . .. . . . . . ", ".A.AA.A.A.A.AA.A", "KAKKAKKAKKAKKAKK", "KKKKKKKKKKKKKKKK",
        "................", "................", "................", "................" 
    }, p);

    // MISSING SPELL ICONS FIX
    createTexture(textures, "spell_thorns", 16, 16, { "..A..A..", ".AAAAAA.", "AAAAAAAA", ".AAAAAA.", "..AAAA..", ".A.AA.A.", "A..AA..A", "........" }, p);
    createTexture(textures, "spell_ice", 16, 16, { ".A.A...A.", " A.A..A..", ".AAAAAAA.", "..AAAAA..", "..AAAAA..", ".A.A.A.A.", "A..A.A..A", "........." }, p);
    createTexture(textures, "spell_lightning", 16, 16, { "..YY.....", ".YY......", "YY.......", "YYYYYYY..", "....YY...", "...YY....", "..YY.....", ".YY......" }, p);
    createTexture(textures, "spell_heal", 16, 16, { "...G.....", "...G.....", ".GGGGG...", "...G.....", "...G.....", ".........", "..GG.....", ".G..G...." }, p);
    createTexture(textures, "spell_shield", 16, 16, { "..AAAA...", ".A....A..", "A......A.", "A......A.", "A......A.", ".A....A..", "..AAAA...", "........." }, p);
    createTexture(textures, "spell_teleport", 16, 16, { ".PP..PP..", "P..PP..P.", "........", "..P..P...", ".P....P..", "P......P.", "........", "........." }, p);
    createTexture(textures, "spell_nova", 16, 16, { "...RRR...", "..R...R..", ".R.....R.", ".R.....R.", ".R.....R.", "..R...R..", "...RRR...", "........." }, p);
    createTexture(textures, "spell_drain", 16, 16, { "R.......R", ".R.....R.", "..R...R..", "...RRR...", "..R...R..", ".R.....R.", "R.......R", "........." }, p);
    createTexture(textures, "spell_poison", 16, 16, { "..GG.....", ".G..G....", "G....G...", ".G..G....", "..GG.....", "...G.....", "..G.G....", "........." }, p);
    createTexture(textures, "spell_time", 16, 16, { "YYYYYYYY", ".Y....Y.", "..Y..Y..", "...YY...", "..Y..Y..", ".Y....Y.", "YYYYYYYY", "........" }, p);
    createTexture(textures, "spell_meteor", 16, 16, { "....RR..", "...RRRR.", "..RRRRR.", ".RRRRRR.", "RRRRRRR.", ".R.R.R..", "..R.R...", "........" }, p);
    createTexture(textures, "spell_rage", 16, 16, { "R...R...", ".R.R....", "..R.....", ".R.R....", "R...R...", "........", "R.R.R...", "........" }, p);

    createTexture(textures, "floor_stone", 16, 16, { "DDDDdDDDDDDDDDDD", "DDdDDDDDDDdDDDDD", "DDDDDDDDDDDDDDDD", "dDDDDDDdDDDDDDdD", "DDDDDDDDDDDDDDDD", "DDDDdDDDDDDDDDDD", "DDDDDDDDDDDDdDDD", "DDdDDDDDDDDDDDDD", "DDDDDDDDdDDDDDDD", "DDDDDDDDDDDDDDDD", "DDdDDDDDDDDdDDDD", "DDDDDDDDDDDDDDDD", "DDDDdDDDDDDDDDDD", "DDDDDDDDDDDDDDDD", "DDDDDDdDDDDDDDDD", "DDDDDDDDDDDDDDdD" }, p);
    createTexture(textures, "floor_grass", 16, 16, { "GGGhGGGGGGGGgGGG", "GhGGGGGGgGGGGGGG", "GGGGGGhGGGGGGgGG", "GGgGGGGGGGGhGGGG", "GGGGGGGGgGGGGGGG", "GGhGGGGGGGGGGGGG", "GGGGGGgGGGGGGhGG", "GGGGGGGGGGgGGGGG", "GGgGGGGGGGGGGGGG", "GGGGGGhGGGGGGgGG", "GGGGGGGGGGGGGGGG", "GhGGGGGGgGGGGGGG", "GGGGGGGGGGhGGGGG", "GGgGGGGGGGGGGGGG", "GGGGGGhGGGGGGgGG", "GGGGGGGGGGGGGGGG" }, p);
    createTexture(textures, "floor_tex", 16, 16, { "DDDDDDDDDDDDDDDD", "DDDDDDDDDDDDDDDD", "DDDDDDDDDDDDDDDD", "DDDDDDDDDDDDDDDD", "DDDDDDDDDDDDDDDD", "DDDDDDDDDDDDDDDD", "DDDDDDDDDDDDDDDD", "DDDDDDDDDDDDDDDD" }, p);
    createTexture(textures, "wall_brick", 16, 16, { "BBBBbBBBBbBBBBBB", "BBBBbBBBBbBBBBBB", "KKKKKKKKKKKKKKKK", "BbBBBBbBBBBbBBBB", "BbBBBBbBBBBbBBBB", "KKKKKKKKKKKKKKKK", "BBBBbBBBBbBBBBBB", "BBBBbBBBBbBBBBBB", "KKKKKKKKKKKKKKKK", "BbBBBBbBBBBbBBBB", "BbBBBBbBBBBbBBBB", "KKKKKKKKKKKKKKKK", "BBBBbBBBBbBBBBBB", "BBBBbBBBBbBBBBBB", "KKKKKKKKKKKKKKKK", "BbBBBBbBBBBbBBBB" }, p);
    createTexture(textures, "wall_tex", 16, 16, { "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB" }, p);
    createTexture(textures, "water", 16, 16, { "WWWWWWWWWWWWWWWW", "WWWWwWWWWWWwWWWW", "WWwWWWWWWwWWWWWW", "WWWWWWWWWWWWWWWW", "WWWWWWwWWWWWWwWW", "WWWWwWWWWWWwWWWW", "WWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWW" }, p);
    createTexture(textures, "boss", 32, 32, { "................................", "...........RRRRRRRR.............", "........RRRRRRRRRRRRRR..........", "......RRRRRRRRRRRRRRRRRR........", ".....RRRRRRRRRRRRRRRRRRRR.......", "....KRRRRRYYRRRYYRRRYYRRRK......", "...KKRRRRYYYRYYYRRYYYRYRRKK.....", "..KKKRRRRYKYRYKYRRYKYRYRRKKK....", "..KKKRRRRRRRRRRRRRRRRRRRRKKK....", ".KKKKRRRRRRRRRRRRRRRRRRRRKKKK...", ".K..KRRRRRRRRRRRRRRRRRRRRK..K...", ".....RRRRRRRRRRRRRRRRRRRR.......", ".....RRR...RRR...RRR...RR.......", ".....RR.....R.....R.....R.......", "....KK.............K.............", "................................" }, p);
    createTexture(textures, "player", 16, 16, { ".....WW......", "....WWWW.....", "....FKKF.....", "...WWWWWW....", "..W.WWWW.W...", ".W..WWWW..W..", "b...WWWW...b.", "b...WWWW...b.", "b..WWWWWW..b.", "...WW..WW....", "..WW....WW...", "..BB....BB..." }, p);
    createTexture(textures, "orc", 16, 16, { ".............", "....ggggg....", "...ggrrgg....", "...gggggg....", "..gggggggg...", ".B.gggggg.B..", ".B.gggggg.B..", "...BBBBBB....", "...gg..gg....", "..BB....BB..." }, p);
    createTexture(textures, "archer", 16, 16, { ".....gg......", "....gggg.....", "...g.KK.g....", "..gggggggg...", ".s.gggggg.s..", "s..gggggg..s.", "s..gggggg..s.", "...gggggg....", "...gg..gg....", "..BB....BB..." }, p);
    createTexture(textures, "assassin", 16, 16, { ".............", ".....KKK.....", "....KRRRK....", "...KKKKKKK...", "..sKKKKKKKs..", ".s.KKKKKKK.s.", "...KK...KK...", "...KK...KK...", "..KK.....KK..", "............." }, p);
    createTexture(textures, "mage", 16, 16, { ".....PP......", "....PPPP.....", "...s.KK.s....", "..PPPPPPPP...", ".s.PPPPPP.s..", "s..PPPPPP..s.", "...PPPPPP....", "...PP..PP....", "..PP....PP...", "............." }, p);
    createTexture(textures, "golem", 16, 16, { "....SSSSS....", "..SSSSSSSSS..", ".SSSKSSS KSS.", ".SSSSSSSSSSS.", ".SSSSSSSSSSS.", ".SSSSSSSSSSS.", "..SS.....SS..", "..SS.....SS.." }, p);
    createTexture(textures, "tree", 16, 16, { ".....ggg.....", "....ggggg....", "...ggggggg...", "..ggggggggg..", ".ggggggggggg.", "....TTT......", "....TTT......", "....TTT......" }, p);
    createTexture(textures, "rock", 16, 16, { "....SSSSS....", "..SSSSSSSSS..", ".SSSSSSSSSSS.", "SSSSSSSSSSSSS", "SSSSSSSSSSSSS", ".SSSSSSSSSSS.", "..SSSSSSSSS..", "....SSSSS...." }, p);
    createTexture(textures, "grass", 16, 16, { "................", "................", "................", "................", "................", "...V...V........", "..vV...Vv...V...", ".vVv..vVv..vVv.." }, p);
    createTexture(textures, "flower", 16, 16, { "................", "......R.........", ".....RYR........", "......R.........", "......V.........", ".....vVv........" }, p);
    createTexture(textures, "mushroom", 16, 16, { ".....RRR.....", "...RRRRRRR...", "..RRRRRRRRR..", ".RRWRRWRRWRR.", ".RRRRRRRRRRR.", "...WWWWWWW...", ".....WWW.....", ".....WWW....." }, p);
    createTexture(textures, "stump", 16, 16, { "................", "....TTTTT.......", "...TKKKKKT......", "..TKKKKKKKT.....", "..TTTTTTTTT.....", "..TTTTTTTTT.....", ".TTTTTTTTTTT....", "TTTTTTTTTTTTT..." }, p);
    createTexture(textures, "crystal", 16, 16, { "......C.........", ".....CCC........", "....CCCCC.......", "...CCCCCCC......", "..CCCCCCCCC.....", "....CCCCC.......", ".....CCC........", "......C........." }, p);
    createTexture(textures, "crate", 16, 16, { "BBBBBBBBBBBBBBBB", "BKKKKKKKKKKKKKKB", "BKBBKKKKKKKKBBKB", "BKKBKKKKKKKKKBKB", "BKKBBKKKKKKBBKKB", "BKKKBKKKKBBKKKKB", "BKKKKBKKBKKKKKKB", "BKKKKKBBKKKKKKKB", "BKKKKKBBKKKKKKKB", "BKKKKBKKBKKKKKKB", "BKKKBKKKKBBKKKKB", "BKKBKKKKKKKKKBKB", "BKBBKKKKKKKKBBKB", "BKKKKKKKKKKKKKKB", "BBBBBBBBBBBBBBBB", "BBBBBBBBBBBBBBBB" }, p);
    createTexture(textures, "bones", 16, 16, { "................", ".....sss........", "....sKsKs.......", "....sssss.......", ".....sss........", "..s.......s.....", ".s.s.....s.s....", "................" }, p);
    createTexture(textures, "bullet_arrow", 16, 16, { ".......S........", ".......S........", ".......S........", ".......S........", ".......S........", "......SSS.......", ".....SSSSS......", ".......S........" }, p);
    createTexture(textures, "bullet_fireball", 16, 16, { ".....RRRR.......", "...RRYYYYRR.....", "..RYYYYYYYR.....", ".RYYYYYYYYYR....", ".RYYYYYYYYYR....", "..RYYYYYYYR.....", "...RRYYYYRR.....", ".....RRRR......." }, p);
    createTexture(textures, "bullet_orb", 16, 16, { ".....####.......", "...########.....", "..##########....", ".############...", ".############...", "..##########....", "...########.....", ".....####......." }, p);
    
    createTexture(textures, "tower", 32, 48, { "........SSSS........", ".......SSSSSS.......", "......SSSSSSSS......", "......SSPPSS......", "......SPPPPS......", "......SPPPPS......", "......SSPPSS......", "......SSSSSSSS......", ".....SSSSSSSSSS.....", "....SSSSSSSSSSSS....", "....SSSSSSSSSSSS....", "....SSSSSSSSSSSS....", "...SSSSSSSSSSSSSS...", "..SSSSSSSSSSSSSSSS..", "..SSSSSSSSSSSSSSSS..", "..SSSSSSSSSSSSSSSS..", "..SSSSSSSSSSSSSSSS..", "..SSSSSSSSSSSSSSSS..", "..SSSSSSSSSSSSSSSS..", "SSSSSSSSSSSSSSSSSSSS" }, p);
    
        // --- NEW SPELL ICONS ---
    createTexture(textures, "spell_ice", 16, 16, { ".A.A...A.", " A.A..A..", ".AAAAAAA.", "..AAAAA..", "..AAAAA..", ".A.A.A.A.", "A..A.A..A", "........." }, p);
    createTexture(textures, "spell_lightning", 16, 16, { "..YY.....", ".YY......", "YY.......", "YYYYYYY..", "....YY...", "...YY....", "..YY.....", ".YY......" }, p);
    createTexture(textures, "spell_heal", 16, 16, { "...G.....", "...G.....", ".GGGGG...", "...G.....", "...G.....", ".........", "..GG.....", ".G..G...." }, p);
    createTexture(textures, "spell_shield", 16, 16, { "..AAAA...", ".A....A..", "A......A.", "A......A.", "A......A.", ".A....A..", "..AAAA...", "........." }, p);
    createTexture(textures, "spell_teleport", 16, 16, { ".PP..PP..", "P..PP..P.", "........", "..P..P...", ".P....P..", "P......P.", "........", "........." }, p);
    createTexture(textures, "spell_nova", 16, 16, { "...RRR...", "..R...R..", ".R.....R.", ".R.....R.", ".R.....R.", "..R...R..", "...RRR...", "........." }, p);
    createTexture(textures, "spell_drain", 16, 16, { "R.......R", ".R.....R.", "..R...R..", "...RRR...", "..R...R..", ".R.....R.", "R.......R", "........." }, p);
    createTexture(textures, "spell_poison", 16, 16, { "..GG.....", ".G..G....", "G....G...", ".G..G....", "..GG.....", "...G.....", "..G.G....", "........." }, p);
    createTexture(textures, "spell_time", 16, 16, { "YYYYYYYY", ".Y....Y.", "..Y..Y..", "...YY...", "..Y..Y..", ".Y....Y.", "YYYYYYYY", "........" }, p);
    createTexture(textures, "spell_meteor", 16, 16, { "....RR..", "...RRRR.", "..RRRRR.", ".RRRRRR.", "RRRRRRR.", ".R.R.R..", "..R.R...", "........" }, p);
    createTexture(textures, "spell_rage", 16, 16, { "R...R...", ".R.R....", "..R.....", ".R.R....", "R...R...", "........", "R.R.R...", "........" }, p);
    
    // Ensure obstacle fallback
    if(textures.count("rock")) textures["obstacle.png"] = textures["rock"];
}

void TextureGenerator::createTexture(std::map<std::string, sf::Texture>& textures, const std::string& name, int w, int h, const std::vector<std::string>& pattern, const std::map<char, sf::Color>& palette) {
    sf::Image img;
    img.resize({(unsigned int)w, (unsigned int)h}, sf::Color::Transparent);
    for (int y = 0; y < (int)pattern.size() && y < h; ++y) {
        for (int x = 0; x < (int)pattern[y].size() && x < w; ++x) {
            char key = pattern[y][x];
            if (palette.count(key)) img.setPixel({(unsigned int)x, (unsigned int)y}, palette.at(key));
        }
    }
    sf::Texture tex; 
    (void)tex.loadFromImage(img);
    tex.setSmooth(false); 
    textures[name] = tex;
}